#include <bits/stdc++.h>  // 使用万能头文件stdc++.h，注意，有的IDE可能不支持
#include "random.h"  // 使用自制的随机库

using namespace std;

// 都放静态存储区，节省内存
float** A;
int n;

// 普通高斯消去，LU分解
void LU() {
    // cout << "start LU" << endl;
    for (int k=0; k<n; k++) {
        for (int j=k+1; j<n; j++) {
            A[k][j] = A[k][j] / A[k][k];
        }
        A[k][k] = 1.0;
        for (int i=k+1; i<n; i++) {
            for (int j=k+1; j<n; j++) {
                A[i][j] = A[i][j] - A[k][j]*A[i][k];
            }
            A[i][k] = 0;
        }
        // print(A, n);
        // cout << endl;
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
        reset(A, n);
        // print(A, n);

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