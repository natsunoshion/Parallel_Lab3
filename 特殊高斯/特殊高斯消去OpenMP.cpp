#include <bits/stdc++.h>

using namespace std;

// 对应于数据集三个参数：矩阵列数，非零消元子行数，被消元行行数
#define num_columns 254
#define num_elimination_rows 106
#define num_eliminated_rows 53

// 使用bitset进行存储，R：消元子，E：被消元行
bitset<num_columns> R[10000];  // R[i]记录了首项为i（下标从0开始记录）的消元子行
                               // 所以不能直接用num_elimination_rows设置数组大小
bitset<num_columns> E[num_eliminated_rows];

// 找到当前被消元行的首项
int lp(bitset<5> temp) {
    int k = 4;
    while (temp[k]==0 && k>=0) {
        k--;
    }
    return k;
}

// 特殊高斯消去法并行OpenMP实现，假设每一轮取5列消元子/被消元行出来
void solve() {
    int n = num_columns - 1;
    while (n >= 4) {
        // 每一轮计算n~n-4这5列的消元子，也就是min_R = n-4, max_R = n
        // 使用位运算，先左移num_columns-n-1位让所需要的5位变为最高位，然后再右移num_columns-5位，清除掉低位
        bitset<5> R_temp[10000];
        bitset<5> E_temp[num_eliminated_rows];
        for (int i=0; i<10000; i++) {
            R_temp[i] = (bitset<5>) ((R[i]<<(num_columns-n-1)) >> (num_columns-5)).to_ulong();
        }
        for (int i=0; i<num_eliminated_rows; i++) {
            E_temp[i] = (bitset<5>) ((E[i]<<(num_columns-n-1)) >> (num_columns-5)).to_ulong();
        }
        // 记录这5列的消元操作
        vector<pair<int, int>> records;
        // 遍历被消元行
        for (int i=0; i<num_eliminated_rows; i++) {
            bool is_eliminated = 0;
            // 消元，并记录操作，这里记录使用vector<pair<int, int>>存储，第一个int记录被消元行操作的行号，第二个int记录首项所在的列号
            // 遍历消元子的行，对当前被消元行消元
            for (int j=4; j>=0; j--) {
                // 有对应消元子那就消去
                // j从高向低检查
                if (R_temp[j+n-4].any()) {
                    if (lp(E_temp[i]) == j) {
                        E_temp[i] ^= R_temp[j+n-4];
                        is_eliminated = 1;
                        // 记录(行号，消元子首项)
                        records.emplace_back(i, j+n-4);
                    }
                }
            }
            if (!is_eliminated) {
                // 不为空行则升格，为空行则无视
                if (!E_temp[i].none()) {
                    // 记录升格
                    R[lp(E_temp[i]) + n - 4] = E[i];
                }
                continue;
            }
        }
        // 接下来，对后n-4列进行并行计算，按照records中的记录进行多线程操作（由于刚刚没有存回去，所以这里剩下有n列）
        // 每4段一组进行并行化，不断从records中取列
        for (auto pair : records) {
            int row = pair.first;
            int leader = pair.second;
            int m;
            // 下一轮m可以是-1，代表不用操作了，所以循环条件为m>=4
            // OpenMP多线程并行化
            #pragma omp parallel for simd schedule(guided, 1)
            for (m=n-5; m>=4; m-=5) {
                // 被消元行
                bitset<5> a1_bit = (bitset<5>) ((E[row]<<(num_columns-m-1)) >> (num_columns-5)).to_ulong();
                // 消元子
                bitset<5> b1_bit = (bitset<5>) ((R[leader]<<(num_columns-m-1)) >> (num_columns-5)).to_ulong();
                // 异或
                bitset<5> result = a1_bit ^ b1_bit;
                // 存储回被消元行的bitset
                for (int i=0; i<num_eliminated_rows; i++) {
                    for (int j=0; j<5; j++) {
                        // 注意不要反了，由于bitset的顺序，是E[row][m] = result[4]
                        E[row].set(m-j, result[4-j]);
                    }
                }
            }
            // 剩下的直接一个一个异或就可以了，使用掩码
            for (; m>=0; m--) {
                std::bitset<num_columns> mask;
                mask.reset();
                mask.set(m, 1);
                E[row] ^= (R[leader]&mask);
            }
        }
        n -= 5;
    }
    // 最后，n不到5了，直接遍历消去，不用记录
    bitset<5> R_temp[10000];
    bitset<5> E_temp[num_eliminated_rows];
    // 取出[n, 0]，进行消去
    for (int i=0; i<10000; i++) {
        R_temp[i] = (bitset<5>) ((R[i]<<(num_columns-n-1)) >> (num_columns-n-1)).to_ulong();
    }
    for (int i=0; i<num_eliminated_rows; i++) {
        E_temp[i] = (bitset<5>) ((E[i]<<(num_columns-n-1)) >> (num_columns-n-1)).to_ulong();
    }
    // 遍历被消元行
    for (int i=0; i<num_eliminated_rows; i++) {
        bool is_eliminated = 0;
        // 遍历消元子的行
        for (int j=4; j>=0; j--) {
            if (R_temp[j+n-4].any()) {
                if (lp(E_temp[i]) == j) {
                    E_temp[i] ^= R_temp[j+n-4];
                    is_eliminated = 1;
                }
            }
        }
        if (!is_eliminated) {
            // 不为空行则升格，为空行则舍去
            if (!E_temp[i].none()) {
                R[lp(E_temp[i]) + n - 4] = E[i];
            }
            continue;
        }
    }
    // 存储回去
    for (int i=0; i<num_eliminated_rows; i++) {
        for (int j=0; j<n+1; j++) {
            E[i].set(j, E_temp[i][j]);
        }
    }
}

void print() {
    for (int i=0; i<num_eliminated_rows; i++) {
        // cout << i << ':';
        for (int j=num_columns-1; j>=0; j--) {
            if (E[i][j] == 1) {
                cout << j << ' ';
            }
        }
        cout << endl;
    }
}

int main() {
    // 读入消元子
    ifstream file_R;
    char buffer[10000] = {0};
    // file_R.open("/home/data/Groebner/测试样例1 矩阵列数130，非零消元子22，被消元行8/消元子.txt");
    file_R.open("R.txt");
    if (file_R.fail()) {
        cout << "读入失败" << endl;
    }
    while (file_R.getline(buffer, sizeof(buffer))) {
        // 每一次读入一行，消元子每32位记录进一个int中
        int bit;
        stringstream line(buffer);
        int first_in = 1;

        // 消元子的索引是其首项
        int index;
        while (line >> bit) {
            if (first_in) {
                first_in = 0;
                index = bit;
            }

            // 将第index行的消元子bitset对应位 置1
            R[index][bit] = 1;
        }
    }
    file_R.close();
//--------------------------------
    // 读入被消元行
    ifstream file_E;
    // file_E.open("/home/data/Groebner/测试样例1 矩阵列数130，非零消元子22，被消元行8/被消元行.txt");
    file_E.open("E.txt");

    // 被消元行的索引就是读入的行数
    int index = 0;
    while (file_E.getline(buffer, sizeof(buffer))) {
        // 每一次读入一行，消元子每32位记录进一个int中
        int bit;
        stringstream line(buffer);
        while (line >> bit) {
            // 将第index行的消元子bitset对应位 置1
            E[index][bit] = 1;
        }
        index++;
    }
//--------------------------------
    // 使用C++11的chrono库来计时
    auto start = chrono::high_resolution_clock::now();
    solve();
    auto end = chrono::high_resolution_clock::now();
    auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
    cout << diff.count() << "ms" << endl;
//--------------------------------
    // 验证结果正确性
    // print();
    return 0;
}