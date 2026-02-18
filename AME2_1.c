/*
 *  serial_auth.c - 修正版
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>

static unsigned long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax;

    /* 中間值 */
    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /* Step 1 */
    edi = ((unsigned long)(E370 / 256L) ^ 0x41UL) + 2UL;
    var_C = (unsigned long)(E370 % 256L);
    var_4 = edi;

    /* Step 2 */
    var_8 = (var_4 << 23) + (var_4 << 15);

    /* Step 3 */
    eax = ((unsigned long)(E374 / 256L) ^ 0x4DUL) + var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 */
    edi = (var_10 >> 16) + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = (edi & 0xFFFF0000UL) | ((edi & 0x0000FFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;

    /* Step 7 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        eax = ((unsigned long)(E378 / 256L) ^ (unsigned long)(E378 % 256L));
        eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xB1UL);
        edi = var_14 + eax + edi;
    }

    /* Step 8 */
    var_4 = edi;
    if (edi & 0x80000000UL)
        edi = (edi >> 1) | 0x80000000UL;
    else
        edi >>= 1;
    edi += var_4;

    /* Step 9 - 關鍵修正：保持 sub eax,100h 後的高位 */
    {
        unsigned long low = (unsigned long)(E374 & 0xFFL);
        if (E374 < 0L && low != 0UL) {
            low -= 0x100UL;  /* 變成 0xFFFFFF?? */
        }
        /* xor al 只影響低8位 */
        eax = (low & 0xFFFFFF00UL) | ((low & 0xFFUL) ^ 0xD2UL);
        eax += edi;
    }

    return eax;
}

int main(void)
{
    char serial[20];
    int part1, n, flag;
    long part2, A, B;
    unsigned long auth;

    printf("Enter serial no: ");
    if (scanf("%s", serial) != 1) {
        printf("Input error!\n");
        return 1;
    }

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    flag = (serial[2] != '-') ? 1 : 0;
    B = (long)((unsigned)part1 & 0x0FFFU);
    A = part2;

    auth = calc_auth(A, B, flag);
    printf("The authorization code is %08lX.\n", auth);

    return 0;
}
