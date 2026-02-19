#include <stdio.h>

unsigned long keygen(long part2)
{
    long Q1, R1;
    long low12;
    long Q2, R2;
    long R3, Q3;

    unsigned long A, C;
    unsigned long v4, mix1, v10;
    unsigned long eax, edi;
    long edx;

    /* ---------- main 邏輯 ---------- */

    Q1 = part2 / 10000L;
    R1 = part2 % 10000L;

    low12 = part2 & 0x0FFFL;

    Q2 = low12 / 100;
    R2 = low12 % 100;

    R3 = Q1 % 100;
    Q3 = Q1 / 100;

    A = R1;
    C = R3 + 100 * R2;

    /* ---------- sub_3A698 ---------- */

    edi = (A / 256);
    edi ^= 0x41;
    edi += 1;

    v4 = edi;

    mix1 = (v4 << 23) + (v4 << 15);

    eax = (C / 256);
    eax ^= 0x4D;

    eax += mix1;
    eax += 1;

    edi = v4 + eax;

    v10 = edi;

    edi = (long)v10 >> 16;
    edi += v10;

    eax = v10 + edi;

    /* 這裡原本有 imul eax,esi,... 但 esi=0 所以忽略 */

    eax ^= 0;          /* no-op */
    /* 只 XOR 低16bit */
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFF) ^ 0xACAD);

    edx = C;

    edx ^= 0x32;

    eax += edx;

    /* branch 多半不進 */

    edi = eax;

    edi = (edi >> 1) + edi;

    eax = C;

    edx = eax;

    eax &= 0xFF;

    if ((long)edx < 0 && eax != 0)
        eax -= 0x100;

    eax ^= 0xD2;

    eax += edi;

    return eax;
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