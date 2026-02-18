/*
 *  serial_auth.c - 從 Python 直譯版
 *  Build: cl /AS /Ox serial_auth.c
 *  MSC 6.0: int=16bit, long=32bit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * 精確對應 Python calc_auth_correct() 函式
 */
static unsigned long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long esi;

    /* 中間值計算 */
    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /* ESI = 0 (組合語言: sub esi, esi) */
    esi = 0UL;

    /* Step 1 */
    edx = (unsigned long)(E370 % 256L);
    eax = (unsigned long)(E370 / 256L);
    var_C = edx;
    edi = eax;
    edi ^= 0x41UL;
    edi = edi + 2UL;  /* 自動 & 0xFFFFFFFF (unsigned long) */
    var_4 = edi;

    /* Step 2 */
    edi = var_4 << 23;
    eax = var_4 << 15;
    eax = eax + edi;
    var_8 = eax;

    /* Step 3 */
    /* var_C_temp = E374 % 256L;  -- 未使用，省略 */
    eax = (unsigned long)(E374 / 256L);
    eax ^= 0x4DUL;
    eax = eax + var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 */
    edi = (var_10 >> 16) + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = 0UL;
    eax ^= edi;
    /* xor ax, 0ACADh - 只影響低 16 位 */
    eax = (eax & 0xFFFF0000UL) | ((eax & 0x0000FFFFUL) ^ 0xACADUL);
    
    edi = var_C;
    edi ^= 0x32UL;
    edi = edi + eax;

    /* Step 7: 條件分支 */
    if (flag) {
        var_14 = edi;
        edi = edi << 10;
        
        edx = (unsigned long)(E378 % 256L);
        eax = (unsigned long)(E378 / 256L);
        eax ^= edx;
        /* xor al, 0B1h - 只影響低 8 位 */
        eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xB1UL);
        eax = eax + edi;
        edi = var_14 + eax;
    }

    /* Step 8: 算術右移 */
    var_4 = edi;
    if (edi & 0x80000000UL) {
        edi = (edi >> 1) | 0x80000000UL;
    } else {
        edi = edi >> 1;
    }
    edi = edi + var_4;

    /* Step 9: 關鍵步驟 */
    /* edx = E374 if E374 >= 0 else E374 + 0x100000000 */
    /* -- 這行在 C 中不需要，因為我們只用來判斷符號 */
    
    eax = (unsigned long)(E374 & 0xFFL);  /* movzx eax, al */
    
    if (E374 < 0L) {                       /* and edx,edx; jge */
        if (eax != 0UL) {                  /* and eax,eax; jz */
            eax = eax - 0x100UL;           /* sub eax, 100h */
            /* unsigned 減法下溢：0x?? - 0x100 = 0xFFFFFF?? */
        }
    }
    
    /* xor al, 0D2h - 只 XOR 低 8 位 */
    eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xD2UL);
    eax = eax + edi;

    return eax;
}

/*
 * 主程式
 */
int main(void)
{
    char serial[20];
    int part1;
    long part2;
    int n, flag;
    long A, B;
    unsigned long auth;

    /* 輸入序列號 */
    printf("Enter serial no: ");
    if (scanf("%s", serial) != 1) {
        printf("Input error!\n");
        return 1;
    }

    /* 解析格式 */
    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    /* 判斷分隔符位置 */
    flag = (serial[2] != '-') ? 1 : 0;

    /* 準備參數 */
    B = (long)((unsigned int)part1 & 0x0FFFU);
    A = part2;

    /* 計算授權碼 */
    auth = calc_auth(A, B, flag);

    /* 輸出結果 */
    printf("The authorization code is %08lX.\n", auth);

    return 0;
}
