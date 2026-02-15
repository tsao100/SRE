/* AutoCAD R12 Authorization Generator
   MSC 6.0 compatible version
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long  DWORD;
typedef unsigned int   WORD;

/* ============================================================
   核心演算法（對應 sub_3A698）
   ============================================================ */
DWORD generate_code(DWORD part1, DWORD part2)
{
    DWORD v1, v2, v3, v4;
    DWORD result;
    DWORD temp;

    /* ===== 第一段 ===== */
    v1 = (part2 % 0x100L) ^ 0x41L;
    v1 += part1;

    v2 = (v1 << 23) + (v1 << 15);

    /* ===== 第二段 ===== */
    v3 = (part2 / 0x100L) ^ 0x4DL;
    v3 += v2;
    v3 += 1;

    v4 = v1 + v3;

    temp = (v4 >> 16) + v4;

    result = temp ^ 0x075BCD15L;   /* 123456789 decimal */
    result ^= 0xACADL;

    result += ((part2 & 0xFFFFL) ^ 0x32L);

    /* ===== 最終混合 ===== */
    result = (result >> 1) + result;

    result ^= ((part2 & 0xFFL) ^ 0xD2L);

    return result;
}

/* ============================================================
   主程式（對應 main）
   ============================================================ */
int main(void)
{
    DWORD part1;
    DWORD part2;
    DWORD auth;

    printf("Enter AutoCAD R12 serial no: ");

    if (scanf("%lu-%lu", &part1, &part2) != 2)
    {
        printf("Invalid format.\n");
        return 1;
    }

    auth = generate_code(part1, part2);

    printf("The authorization code is %08lX.\n", auth);

    return 0;
}