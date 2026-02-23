#include <stdio.h>

unsigned long keygen(int part1, long part2)
{
    long Q1, R1, Q1Q, Q1R;
    long low12;
    long Q2, R2;
    long R3, Q3;

    unsigned long A, C, D;
    unsigned long v4, mix1, v10, v14, v18, v1C;
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

    Q1Q = Q1 / 0x64;  //A

    Q1R = Q1 % 0x64; //5

    v18 = ((part1 & 0xFFF) % 0x64) * 0x64; //6A4
    v1C = (part1 / 0x64) * 0x64; //

    C = v18 + Q1R; //6A9L
    D = v1C + Q1Q; //6E


    /* ---------- sub_3A698 ---------- */

    edi = (A / 256);
    edi ^= 0x41;
    edi += 1;

    v4 = edi;        //v4=0x64

    mix1 = (v4 << 23) + (v4 << 15);

    eax = (C / 256); //C=1705=0x6A9
    eax ^= 0x4D;

    eax += mix1;
    eax += 1;        //eax=3232004C

    edi = v4 + eax;     //edi=323200B0

    v10 = edi;

    edi = (long)v10 >> 16;
    edi += v10;

    eax = v10 + edi;

    /* 這裡原本有 imul eax,esi,... 但 esi=0 所以忽略 */

    eax ^= 0xACAD;         //Here ok. EIP=576
    /* 只 XOR 低16bit */
    //eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFF) ^ 0xACAD);

    edx = R1 % 256; //edx=CB

    edx ^= 0x32; //0x45 for AME 1.0

    eax += edx;

    /* branch 多半不進 */

    edi = eax;

    v14 = edi;

    edi = (edi << 0x0A);

    eax = D;

    edx = (eax % 0x100); 

    eax = 0;

    eax ^= edx;

    eax ^= 0xB1;

    eax += edi;

	edi = v14;

	edi += eax;

	v4 = edi;

    edi = (((long) edi) >> 1) + v4; //EIP=5B2

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

    printf("Enter AutoCAD serial (XXX-YYYYYYYY): ");

    if (scanf("%d-%ld", &part1, &part2) != 2)
        return 1;
    //part1 = 110;
    //part2 = 10524473;

    result = keygen(part1, part2);

    printf("AME 2.1 Authorization Code = %08lX\n", result);

    return 0;
}