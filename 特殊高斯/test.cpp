#include <iostream>
#include "bm/bm.h"
#include "bm/bmalgo.h"


int main() {
    // 创建BitMagic位图
    bm::bvector<> bits;
    bits.set_bit(0);
    bits.set_bit(2);
    bits.set_bit(4);
    bits.set_bit(6);
    bits.set_bit(8);
    bits.set_bit(10);
    bits.set_bit(12);
    bits.set_bit(14);

    // 获取指定范围内的位
    int start = 0;
    int end = 13;
    bm::bvector<> result;
    result.copy_range(bits, start, end);
    // bits.insert(1, result);

    // 输出结果
    std::cout << "Bits from position " << start << " to " << end << ": ";
    for (int i = start; i <= end; i++) {
        std::cout << result.get_bit(i);
    }
    std::cout << std::endl;

    return 0;
}
