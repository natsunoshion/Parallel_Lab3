#include <bits/stdc++.h>
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

// // 生成0到1之间的浮点随机数
// float get_random_float() {
//     static mt19937 gen(time(nullptr));  // 以时间作为种子
//     uniform_real_distribution<> dis(0.0, 1.0);  // 均匀分布
//     return dis(gen);
// }

// // 重新产生随机数矩阵
// // 采用先产生上三角矩阵，然后不同行之间线性组合的策略，保证矩阵满秩
// void reset(float** m, int n) {
//     // 先产生一个上三角矩阵
//     for (int i=0; i<n; i++) {
//         for (int j=0; j<n; j++) {
//             m[i][j] = 0;
//         }
//         m[i][i] = 1.0;
//         for (int j=i+1; j<n; j++) {
//             m[i][j] = get_random_float();
//         }
//     }
//     // 向下进行线性组合
//     for (int k=0; k<n; k++) {
//         for (int i=k+1; i<n; i++) {
//             // 第i行 += 第k行
//             for (int j=0; j<n; j++) {
//                 m[i][j] += m[k][j];
//             }
//         }
//     }
// }

// 产生随机可逆n*n的矩阵
MatrixXd generate_invertible_matrix(int n) {
    // // 产生n*n随机矩阵
    // MatrixXd A = MatrixXd::Random(n, n);

    // // Compute the QR decomposition of A
    // HouseholderQR<MatrixXd> qr(A);

    // // Extract the upper triangular matrix R from the QR decomposition
    // MatrixXd R = qr.matrixQR().triangularView<Upper>();

    // // Make the diagonal entries of R positive
    // for (int i=0; i<n; i++) {
    //     if (R(i, i) < 0) {
    //         R.row(i) *= -1;
    //     }
    // }

    // // Compute the invertible matrix A = Q * R
    // MatrixXd Q = qr.householderQ();
    // MatrixXd A_inv = R.inverse() * Q.transpose();

    // return A_inv;
    // n*n矩阵
    MatrixXd A(n, n);

    // 直到随机产生一个可逆的n*n矩阵为止
    while (true) {
        A = MatrixXd::Random(n, n);
        if (A.determinant() != 0) {
            break;
        }
    }
    return A;
}

void print(vector<vector<double>> m, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << m[i][j] << ' ';
        }
        cout << endl;
    }
}