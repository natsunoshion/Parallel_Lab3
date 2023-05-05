#include <bits/stdc++.h>
#include "random.h"
#include <pthread.h>  // Pthread编程

using namespace std;

#define NUM_THREADS 4

int thread_count;
pthread_barrier_t barrier;

// 用于线程函数传参：数组A，线程号id和当前行k
typedef struct {
    float** A;
    int id;
    int k;
    int n;
} ThreadArgs;

// 并行函数
void* thread_func(void* arg) {
    // 传参
    ThreadArgs* thread_arg = (ThreadArgs*)arg;
    float** A = thread_arg->A;
    int id = thread_arg->id;
    int k = thread_arg->k;
    int n = thread_arg->n;

    // 除法
    // A[k][k, n)对应列的除法进行分块，第arg号线程会处理第arg块除法
    // 单个线程负责第id块的除法，也就是对下标为[start, end)的元素进行除法
    int start = k + id * ((n-k) / NUM_THREADS);
    int end = k + (id+1) * ((n-k) / NUM_THREADS);
    if (id + 1 == NUM_THREADS) {  // 最后一轮补全
        end = n;
    }
    for (int i=start; i<end; i++) {
        A[k][i] /= A[k][k];
    }
    // 同步线程
    pthread_barrier_wait(&barrier);

    // 消去第A[k+1, n)行的第k列元素，同样，分块处理
    // 第k行不能合在一起考虑了，第k列还可以
    start = k+1 + id * ((n-k-1) / NUM_THREADS);
    end = k+1 + (id+1) * ((n-k-1) / NUM_THREADS);
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

// 普通高斯消去，LU分解
void LU(float** A, int N) {
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_ids[NUM_THREADS];

    // barrier初始化，用于同步线程
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    for (int k=0; k<N; k++) {
        // 初始化线程的id
        for (int i=0; i<NUM_THREADS; i++) {
            thread_ids[i] = {A, i, k, N};
        }
        for (int i=0; i<NUM_THREADS; i++) {
            pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
        }
        for (int i=0; i<NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    pthread_barrier_destroy(&barrier);
}

int main() {
    int N;
    vector<int> size = {200, 500, 1000, 2000, 3000};
    for (int i=0; i<5; i++) {
        // 设置问题规模
        N = size[i];

        // 初始化二维数组
        float** A = new float*[N];
        for (int i=0; i<N; i++) {
            A[i] = new float[N];
        }

        // 使用随机数重置数组
        reset(A, N);

        // 使用C++11的chrono库来计时
        auto start = chrono::high_resolution_clock::now();
        LU(A, N);
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
        cout << "Size = " << N << ": " << diff.count() << "ms" << endl;
        // print(A, N);
        // break;
    }
    return 0;
}