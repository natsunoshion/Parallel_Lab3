#include <bits/stdc++.h>
#include "random.h"
#include <pthread.h>  // Pthread编程

using namespace std;

#define NUM_THREADS 4

// 都放静态存储区，节省内存
float** A;
int n;

// 用于线程函数传参：线程号id和当前行k
typedef struct {
    int id;
    int k;
} ThreadArgs;

// 并行函数
void* thread_func(void* arg) {
    // 传参
    ThreadArgs* thread_arg = (ThreadArgs*)arg;
    int id = thread_arg->id;
    int k = thread_arg->k;

    // 消去第[k+1, n)行的第k列元素，同样，分块处理
    // 第k行不能合在一起考虑了，第k列还可以
    int start = k+1 + id * ((n-k-1) / NUM_THREADS);
    int end = k+1 + (id+1) * ((n-k-1) / NUM_THREADS);
    if (id + 1 == NUM_THREADS) {
        end = n;
    }
    for (int i=start; i<end; i++) {
        for (int j=k; j<n; j++) {
            A[i][j] -= A[i][k] * A[k][j];
        }
    }
    pthread_exit(NULL);
}

// Pthread动态线程
void LU() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_ids[NUM_THREADS];

    for (int k=0; k<n; k++) {
        // 串行除法
        for (int i=k; i<n; i++) {
            A[k][i] /= A[k][k];
        }
        // 多线程并行减法，此方法缺点：频繁创建线程，开销大
        // 初始化线程的id
        for (int i=0; i<NUM_THREADS; i++) {
            thread_ids[i] = {i, k};
        }
        for (int i=0; i<NUM_THREADS; i++) {
            pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
        }
        for (int i=0; i<NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }
}

int main() {
    vector<int> size = {200, 500, 1000, 2000, 3000};
    for (int i=0; i<5; i++) {
        // 设置问题规模
        n = size[i];

        // 初始化二维数组
        A = new float*[n];
        for (int i=0; i<n; i++) {
            A[i] = new float[n];
        }

        // 使用随机数重置数组
        reset(A, n);

        // 使用C++11的chrono库来计时
        auto start = chrono::high_resolution_clock::now();
        LU();
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
        cout << "Size = " << n << ": " << diff.count() << "ms" << endl;

        // print(A, n);
        // break;

        // 释放二维数组A的空间
        for (int i = 0; i < n; i++) {
            delete[] A[i];
        }
        delete[] A;
    }
    return 0;
}