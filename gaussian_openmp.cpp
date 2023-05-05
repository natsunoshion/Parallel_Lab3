#include <bits/stdc++.h>  // 使用万能头文件stdc++.h，注意，有的IDE可能不支持
#include "random.h"  // 使用自制的随机库

using namespace std;

#define NUM_THREADS 4

// OpenMP高斯消去，LU分解
void LU(float** A, int N) {
    // 循环外创建线程，避免线程反复创建销毁，影响程序性能
    float tmp;
    int i, j, k;
    bool parallel = true;
    // print(A, N);
    #pragma omp parallel if(parallel), num_threads(NUM_THREADS), private(i, j, k, tmp)
    for (k=0; k<N; k++) {
        // 串行除法
        #pragma omp single
        {
            tmp = A[k][k];
            // 由于用了tmp，所以A[k][k]也可以合在循环变量中了
            for (j=k; j<N; j++) {
                A[k][j] /= tmp;
            }
        }
        // print(A, N);
        // cout << endl;

        // 使用OpenMP并行化行消去
        // 使用OpemMP 4.0的SIMD方法进行自动向量化、循环展开
        #pragma omp for simd
        for (i=k+1; i<N; i++) {
            tmp = A[i][k];
            for (j=k; j<N; j++) {
                A[i][j] -= A[k][j] * tmp;
            }
        }
    }
}

int main() {
    int N;
    vector<int> size = {200, 500, 1000, 2000, 3000};
    for (int i=0; i<5; i++) {
        // 设置问题规模
        N = size[i];

        // // 使用MatrixXd产生随机可逆矩阵
        // MatrixXd A = generate_invertible_matrix(N);

        // // MatrixXd转为二维数组
        // vector<vector<double>> A_vec(N, vector<double>(N));
        // for (int i=0; i<A.rows(); i++) {
        //     for (int i=0; i<A.rows(); i++) {
        //         for (int j=0; j<A.cols(); j++) {
        //             A_vec[i][j] = A(i, j);
        //         }
        //     }
        // }

        // 初始化二维数组
        float** A = new float*[N];
        for (int i=0; i<N; i++) {
            A[i] = new float[N];
        }
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