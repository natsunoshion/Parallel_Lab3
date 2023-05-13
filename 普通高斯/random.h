#include <bits/stdc++.h>

using namespace std;

static mt19937 gen(time(nullptr));  // 以时间作为种子

// 生成0到1之间的浮点随机数
float get_random_float() {
    uniform_real_distribution<> dis(0.0, 1.0);  // 均匀分布
    return dis(gen);
}

// 生成[0, n)的随机整数，用于随机线性组合
int get_random_int(int n) {
    uniform_int_distribution<> dis(0, n-1);  // 均匀分布，[0, n-1]，这很坑
    return dis(gen);
}

void print(float** m, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << m[i][j] << ' ';
        }
        cout << endl;
    }
}

// 重新产生随机数矩阵
// 尝试过几天，事实上全部random才是最佳选择
void reset(float** m, int n) {
    // // 先产生一个上三角矩阵
    // for (int i=0; i<n; i++) {
    //     for (int j=0; j<i; j++) {
    //         m[i][j] = 0;
    //     }
    //     m[i][i] = 1.0;
    //     for (int j=i+1; j<n; j++) {
    //         m[i][j] = get_random_float();
    //     }
    // }
    // // print(m, n);
    // // cout << endl;
    // // 随机组合法（目的是不产生精度问题），进行线性组合
    // // 组合次数不能太大，否则指数级增长会让数超出float表示范围
    // for (int time=0; time<n*n; time++) {
    //     // 第i行+=/-=第j行-第k行
    //     int i = get_random_int(n);
    //     int j = get_random_int(n);
    //     int k = get_random_int(n);
    //     // cout << i << ' ' << j << endl;
    //     for (int l=0; l<n; l++) {
    //         if (time % 2 == 0) {
    //             m[i][l] += m[j][l] - m[k][l];
    //         } else {
    //             m[i][l] -= m[j][l] - m[k][l];
    //         }
    //     }
    //     // if (k > 80*200) {
    //         // print(m, n);
    //         // cout << endl;
    //     // }
    // }
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            m[i][j] = get_random_float();
        }
    }
}