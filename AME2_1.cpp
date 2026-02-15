#include <iostream>
#include <cstdio>
#include <cstdint>

using namespace std;

/*
    對應 sub_3A698 的核心演算法
*/
uint32_t generateAuthorizationCode(uint32_t part1, uint32_t part2)
{
    const uint32_t CONST1 = 0x41;
    const uint32_t CONST2 = 0x4D;
    const uint32_t CONST3 = 0xACAD;
    const uint32_t CONST4 = 0xD2;
    const uint32_t CONST5 = 0x32;

    uint32_t v1, v2, v3, v4;
    uint32_t result;

    // ===== 第一段運算 =====
    v1 = (part2 % 0x100) ^ CONST1;
    v1 += part1;

    v2 = (v1 << 23) + (v1 << 15);

    // ===== 第二段運算 =====
    v3 = (part2 / 0x100) ^ CONST2;
    v3 += v2;
    v3 += 1;

    v4 = v1 + v3;

    uint32_t temp = (v4 >> 16) + v4;

    result = temp ^ 0x075BCD15;     // 123456789 decimal
    result ^= CONST3;

    uint32_t mix = ((part2 & 0xFFFF) ^ CONST5);
    result += mix;

    // ===== 最後混合 =====
    result = (result >> 1) + result;

    uint32_t low = (part2 & 0xFF) ^ CONST4;
    result ^= low;

    return result;
}

int main()
{
    cout << "Enter AutoCAD R12 serial no: ";

    unsigned int part1;
    unsigned int part2;

    if (scanf("%u-%u", &part1, &part2) != 2)
    {
        cout << "Invalid format.\n";
        return 1;
    }

    uint32_t auth = generateAuthorizationCode(part1, part2);

    printf("The authorization code is %08X.\n", auth);

    return 0;
}