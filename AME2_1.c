/*
 *  serial_auth.c - MSC 6.0 16-bit 最終修正版
 *  組譯語言是32-bit，但編譯器是16-bit，使用 long(32bit) 模擬
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 計算授權碼 - 精確模擬 32-bit 組合語言 */
static unsigned long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;

    /* 中間值 */
    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /* Step 1: E370 / 256 */
    edx = (unsigned long)(E370 % 256L);
    eax = (unsigned long)(E370 / 256L);
    var_C = edx;
    edi = eax;
    edi ^= 0x41UL;
    edi += 2UL;
    var_4 = edi;

    /* Step 2: (var_4 << 23) + (var_4 << 15) */
    edi = var_4 << 23;
    eax = var_4 << 15;
    eax += edi;
    var_8 = eax;

    /* Step 3: E374 / 256 */
    eax = (unsigned long)(E374 / 256L);
    eax ^= 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4: edi = var_10 + (var_10>>16) + var_10 */
    edi = (var_10 >> 16) + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5: imul eax, esi(=0), K; xor eax,edi; xor ax,0ACADh */
    eax = 0UL;          /* esi = 0 */
    eax ^= edi;
    /* xor ax - 只影響低 16 位 */
    eax = (eax & 0xFFFF0000UL) | ((eax & 0x0000FFFFUL) ^ 0xACADUL);
    
    edi = var_C;
    edi ^= 0x32UL;
    edi += eax;

    /* Step 7: 條件分支 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        
        edx = (unsigned long)(E378 % 256L);
        eax = (unsigned long)(E378 / 256L);
        eax ^= edx;
        /* xor al - 只影響低 8 位 */
        eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xB1UL);
        eax += edi;
        edi = var_14 + eax;
    }

    /* Step 8: 算術右移 */
    var_4 = edi;
    if (edi & 0x80000000UL)
        edi = (edi >> 1) | 0x80000000UL;
    else
        edi >>= 1;
    edi += var_4;

    /* Step 9: 符號擴展低字節 */
    eax = (unsigned long)(E374 & 0xFFL);  /* movzx eax, al */
    
    if (E374 < 0L) {                       /* and edx,edx; jge */
        if (eax != 0UL) {                  /* and eax,eax; jz */
            eax -= 0x100UL;                /* sub eax, 100h */
        }
    }
    
    /* xor al, 0D2h - 只 XOR 低 8 位 */
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
    if (scanf("%s", serial) != 1) {
        printf("Input error!\n");
        return 1;
    }

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    /* 判斷分隔符位置決定 flag */
    flag = (serial[2] != '-') ? 1 : 0;
    
    B = (long)((unsigned)part1 & 0x0FFFU);
    A = part2;

    auth = calc_auth(A, B, flag);
    
    /* MSC 6.0 的 printf 格式化 */
    printf("The authorization code is %08lX.\n", auth);

    return 0;
}
