/*
 *  serial_auth.c - 修正 Step 4 移位問題
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>

static unsigned long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;

    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /* Step 1 */
    edx = (unsigned long)(E370 % 256L);
    eax = (unsigned long)(E370 / 256L);
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += 2UL;
    var_4 = edi;

    /* Step 2 */
    edi = var_4 << 23;
    eax = var_4 << 15;
    eax += edi;
    var_8 = eax;

    /* Step 3 */
    eax = (unsigned long)(E374 / 256L);
    eax ^= 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 - 修正移位問題 */
    /* 方法1: 用除法代替移位 */
    edi = (var_10 / 65536UL) + var_10;
    
    /* 方法2: 分兩次移位（擇一使用）
    edi = var_10 >> 8;
    edi = edi >> 8;
    edi += var_10;
    */
    
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = 0UL ^ edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = var_C ^ 0x32UL;
    edi += eax;

    /* Step 7 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        edx = (unsigned long)(E378 % 256L);
        eax = (unsigned long)(E378 / 256L);
        eax ^= edx;
        eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xB1UL);
        eax += edi;
        edi = var_14 + eax;
    }

    /* Step 8 */
    var_4 = edi;
    if (edi & 0x80000000UL)
        edi = (edi >> 1) | 0x80000000UL;
    else
        edi >>= 1;
    edi += var_4;

    /* Step 9 */
    eax = (unsigned long)(E374 & 0xFFL);
    if (E374 < 0L && eax != 0UL)
        eax -= 0x100UL;
    eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xD2UL);
    eax += edi;

    return eax;
}

int main(void)
{
    char serial[20];
    int part1, n, flag;
    long part2, A, B;
    unsigned long auth;

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    flag = (serial[2] != '-') ? 1 : 0;
    B = (long)((unsigned)part1 & 0xFFFU);
    A = part2;

    auth = calc_auth(A, B, flag);
    printf("The authorization code is %08lX.\n", auth);

    return 0;
}
