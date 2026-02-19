#include <stdio.h>

unsigned long keygen(long part2)
{
    long Q1, R1;
    long low12;
    long Q2, R2;
    long R3, Q3;

    unsigned long A, C;
    unsigned long v4, mix1, v10;
    unsigned long t1, t2;
    long t3;

    /* ---------- main() 第一段 ---------- */
    Q1 = part2 / 10000L;
    R1 = part2 % 10000L;

    /* ---------- main() 第二段 ---------- */
    low12 = part2 & 0x0FFFL;

    Q2 = low12 / 100;
    R2 = low12 % 100;

    R3 = Q1 % 100;
    Q3 = Q1 / 100;

    A = (unsigned long)R1;
    C = (unsigned long)(R3 + 100 * R2);

    /* ---------- sub_3A698 ---------- */

    t1 = (A / 256) ^ 0x41;
    v4 = t1 + 1;

    mix1 = (v4 << 23) + (v4 << 15);

    t2 = (C / 256) ^ 0x4D;

    v10 = v4 + mix1 + t2;

    v10 += 1;

    v10 += (v10 >> 16);

    v10 ^= 0x0000ACAD;

    t3 = (long)(C & 0xFF);

    if (t3 >= 0x80)
        t3 -= 0x100;

    t3 ^= 0xD2;

    return v10 + t3;
}

int main()
{
    int part1;
    long part2;
    unsigned long result;

    printf("Enter serial (XXX-YYYYYYYY): ");

    if (scanf("%d-%ld", &part1, &part2) != 2)
        return 1;

    result = keygen(part2);

    printf("Authorization Code = %08lX\n", result);

    return 0;
}