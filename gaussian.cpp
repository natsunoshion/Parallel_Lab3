#include <bits/stdc++.h>  // 使用万能头文件stdc++.h，注意，有的IDE可能不支持
#include "random.h"  // 使用自制的随机库

using namespace std;

// 普通高斯消去，LU分解
void LU(vector<vector<double>>& A, int N) {
    for (int k=0; k<N; k++) {
        for (int j=k+1; j<N; j++) {
            A[k][j] = A[k][j] / A[k][k];
        }
        A[k][k] = 1.0;
        for (int i=k+1; i<N; i++) {
            for (int j=k+1; j<N; j++) {
                A[i][j] = A[i][j] - A[k][j]*A[i][k];
            }
            A[i][k] = 0;
        }
    }
}

int main() {
    int N;
    vector<int> size = {200, 500, 1000, 2000, 3000};
    for (int i=0; i<5; i++) {
        // 设置问题规模
        N = size[i];

        // 使用MatrixXd产生随机可逆矩阵
        MatrixXd A = generate_invertible_matrix(N);

        // MatrixXd转为二维数组
        vector<vector<double>> A_vec(N, vector<double>(N));
        for (int i=0; i<A.rows(); i++) {
            for (int i=0; i<A.rows(); i++) {
                for (int j=0; j<A.cols(); j++) {
                    A_vec[i][j] = A(i, j);
                }
            }
        }

        // 使用C++11的chrono库来计时
        auto start = chrono::high_resolution_clock::now();
        LU(A_vec, N);
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
        cout << "Size = " << N << ": " << diff.count() << "ms" << endl;
        // print(A_vec, N);
        // break;
    }
    return 0;
}