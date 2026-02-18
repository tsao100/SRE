/*
 *  serial_auth.c  (MSC 6.0修正版)
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    long var_4, var_8, var_C, var_10, var_14;
    long edi, eax;
    int quot, rem;

    /* 中間值 */
    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /* Step 1: E370 / 256 */
    quot = (int)(E370 / 256L);
    rem  = (int)(E370 % 256L);
    var_C = rem;
    edi = ((long)quot ^ 0x41L) + 2L;
    var_4 = edi;

    /* Step 2: var_4 << 23 + var_4 << 15 */
    var_8 = (var_4 << 23) + (var_4 << 15);

    /* Step 3: 處理 E374 */
    quot = (int)(E374 / 256L);
    eax = ((long)quot ^ 0x4DL) + var_8 + 1L;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4: edi = var_10 + (var_10>>16) + var_10 */
    edi = (var_10 >> 16) + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5: XOR ax, 0xACAD */
    eax = edi;
    eax = (eax & 0xFFFF0000L) | ((eax & 0x0000FFFFL) ^ 0xACADL);
    edi = (var_C ^ 0x32L) + eax;

    /* Step 7: 條件分支 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        
        quot = (int)(E378 / 256L);
        rem  = (int)(E378 % 256L);
        eax = ((long)quot ^ (long)rem) ^ 0xB1L;
        eax += edi;
        edi = var_14 + eax;
    }

    /* Step 8: 算術右移 + 自加 (3*edi/2) */
    var_4 = edi;
    /* 
     * MSC 6.0: signed long >> 是算術右移
     * 但為了確保跨平台，手動實現
     */
    if (edi & 0x80000000L) {
        edi = ((unsigned long)edi >> 1) | 0x80000000L;
    } else {
        edi = edi >> 1;
    }
    edi += var_4;

    /* Step 9: 低字節符號擴展 XOR 0xD2 */
    {
        long low = E374 & 0xFFL;
        /* 原組合語言:
         * movzx eax, al
         * test edx, edx (E374)
         * jge skip
         * test eax, eax
         * jz skip
         * sub eax, 100h
         * skip: xor al, 0xD2
         */
        if (E374 < 0L && low != 0L) {
            low -= 0x100L;
        }
        /* 確保低字節範圍 */
        while (low < 0L) low += 0x100L;
        while (low >= 0x100L) low -= 0x100L;
        
        eax = low ^ 0xD2L;
    }
    eax += edi;

    /* Step 10: esi=0, edi=0^eax=eax */
    return eax;
}

int main(void)
{
    char  serial[20];
    int   part1;
    long  part2;
    int   n, flag;
    long  A, B, auth;

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    flag = (serial[2] != '-') ? 1 : 0;
    B = (long)((unsigned int)part1 & 0x0FFFU);
    A = part2;

    auth = calc_auth(A, B, flag);

    printf("The authorization code is %08.0lX.\n", (unsigned long)auth);

    return 0;
}
