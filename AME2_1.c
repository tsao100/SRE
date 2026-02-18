/* SERIAL.C
   Microsoft C 6.0 compatible
   16-bit DOS program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 全域變數 (對應原 ASM 全域區) */

unsigned long g_part1;      /* 萬位群 */
unsigned long g_part2;      /* 千位以下 */
unsigned long g_mid1;
unsigned long g_mid2;
int g_flag_special = 0;


/* --------------------------------------------------
   核心授權碼演算法
   (對應 sub_3A698)
-------------------------------------------------- */
unsigned long generate_auth_code(void)
{
    unsigned long edi = 0;
    unsigned long eax = 0;
    unsigned long edx = 0;
    unsigned long temp1;
    unsigned long extra;

    /* 第一階段 */
    edx = g_part2 / 256;
    edi = edx;

    edi ^= 0x41;
    edi += g_mid1;

    temp1 = edi;

    edi <<= 23;

    /* 第二階段 */
    eax = temp1 << 15;
    eax += edi;

    /* 第三階段 */
    edx = g_mid2 / 256;
    edx ^= 0x4D;

    eax += edx;
    eax++;

    edi = temp1 + eax;

    edi = (edi >> 16) + edi;
    eax = edi + temp1;

    /* magic constant */
    eax ^= 0xACAD;

    /* 特殊模式 */
    if (g_flag_special)
    {
        extra = edi << 10;
        extra ^= 0xB1;
        eax += extra;
    }

    /* 最終混合 */
    edi = (eax >> 1) + eax;
    eax = edi;

    eax ^= 0xD2;

    return eax;
}


/* --------------------------------------------------
   解析輸入
-------------------------------------------------- */
int parse_serial(char *input)
{
    int partA = 0;
    long partB = 0;
    int full;

    if (sscanf(input, "%d-%ld", &partA, &partB) != 2)
        return 0;

    if (strchr(input, '-') == NULL)
        g_flag_special = 1;

    full = partA;

    g_part1 = full / 10000;
    g_part2 = full % 10000;

    /* 模擬原始拆分邏輯 */
    g_mid1 = (g_part2 % 100) * 20 + (g_part1 % 100);
    g_mid2 = (g_part1 % 100) * 20 + (g_part2 % 100);

    return 1;
}


/* --------------------------------------------------
   主程式
-------------------------------------------------- */
int main(void)
{
    char input[64];
    unsigned long auth;

    printf("Enter serial no: ");
    scanf("%63s", input);

    if (!parse_serial(input))
    {
        printf("Invalid serial format!\n");
        return 1;
    }

    auth = generate_auth_code();

    printf("The authorization code is %08lX.\n", auth);

    return 0;
}