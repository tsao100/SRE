/*
 *  serial_auth_debug.c - 完整调试版
 *  Build: cl /AS /Ox serial_auth_debug.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long calc_auth_debug(long A, long B, int flag)
{
    unsigned long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long temp1, temp2;

    /* 计算中间值 */
    E36C = (unsigned long)(A / 10000L);
    E370 = (unsigned long)(A % 10000L);
    E374 = (E36C % 100UL) + (B % 100UL) * 100UL;
    E378 = (E36C / 100UL) + (B / 100UL) * 100UL;

    printf("=== INPUT ===\n");
    printf("A=%ld (0x%lX), B=%ld (0x%lX), flag=%d\n", A, A, B, B, flag);
    printf("E36C=%lu (0x%lX)\n", E36C, E36C);
    printf("E370=%lu (0x%lX)\n", E370, E370);
    printf("E374=%lu (0x%lX)\n", E374, E374);
    printf("E378=%lu (0x%lX)\n\n", E378, E378);

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += 2UL;
    var_4 = edi;
    
    printf("=== STEP 1 ===\n");
    printf("E370 %% 256 = %lu\n", edx);
    printf("E370 / 256 = %lu (0x%lX)\n", eax, eax);
    printf("var_C = %lu\n", var_C);
    printf("edi = %lu ^ 0x41 + 2 = %lu (0x%lX)\n", eax, edi, edi);
    printf("var_4 = 0x%lX\n\n", var_4);

    /* Step 2 */
    temp1 = var_4 << 23;
    temp2 = var_4 << 15;
    var_8 = temp1 + temp2;
    
    printf("=== STEP 2 ===\n");
    printf("var_4 << 23 = 0x%lX\n", temp1);
    printf("var_4 << 15 = 0x%lX\n", temp2);
    printf("var_8 = 0x%lX\n\n", var_8);

    /* Step 3 */
    eax = (E374 / 256UL) ^ 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;
    
    printf("=== STEP 3 ===\n");
    printf("E374 / 256 = %lu (0x%lX)\n", E374 / 256UL, E374 / 256UL);
    printf("(E374/256) ^ 0x4D = 0x%lX\n", (E374 / 256UL) ^ 0x4DUL);
    printf("eax = 0x%lX\n", eax);
    printf("edi = var_4 + eax = 0x%lX\n", edi);
    printf("var_10 = 0x%lX\n\n", var_10);

    /* Step 4 - 多种方式尝试 */
    printf("=== STEP 4 (Testing multiple methods) ===\n");
    
    /* 方法1: 直接 >> 16 */
    temp1 = var_10 >> 16;
    printf("Method 1: var_10 >> 16 = 0x%lX\n", temp1);
    
    /* 方法2: 两次 >> 8 */
    temp1 = var_10 >> 8;
    temp2 = temp1 >> 8;
    printf("Method 2: (var_10 >> 8) >> 8 = 0x%lX\n", temp2);
    
    /* 方法3: / 65536 */
    temp1 = var_10 / 65536UL;
    printf("Method 3: var_10 / 65536 = 0x%lX\n", temp1);
    
    /* 使用方法2 */
    temp1 = var_10 >> 8;
    edi = temp1 >> 8;
    edi += var_10;
    eax = var_10 + edi;
    edi = eax;
    
    printf("Using Method 2:\n");
    printf("edi = (temp2) + var_10 = 0x%lX\n", edi - var_10 + temp2);
    printf("eax = var_10 + edi = 0x%lX\n", eax);
    printf("edi = 0x%lX\n\n", edi);

    /* Step 5 */
    eax = edi;
    temp1 = (eax & 0xFFFFUL) ^ 0xACADUL;
    eax = (eax & 0xFFFF0000UL) | temp1;
    edi = (var_C ^ 0x32UL) + eax;
    
    printf("=== STEP 5 ===\n");
    printf("low 16 bits = 0x%lX\n", edi & 0xFFFF);
    printf("low 16 ^ 0xACAD = 0x%lX\n", temp1);
    printf("eax after xor = 0x%lX\n", eax);
    printf("var_C ^ 0x32 = 0x%lX\n", var_C ^ 0x32UL);
    printf("edi = 0x%lX\n\n", edi);

    /* Step 7 */
    if (flag) {
        printf("=== STEP 7 (flag=1) ===\n");
        var_14 = edi;
        edi <<= 10;
        printf("edi << 10 = 0x%lX\n", edi);
        
        edx = E378 % 256UL;
        eax = E378 / 256UL;
        printf("E378 / 256 = 0x%lX, E378 %% 256 = 0x%lX\n", eax, edx);
        
        eax ^= edx;
        printf("eax ^ edx = 0x%lX\n", eax);
        
        temp1 = (eax & 0xFFUL) ^ 0xB1UL;
        eax = (eax & 0xFFFFFF00UL) | temp1;
        printf("low 8 ^ 0xB1 = 0x%lX, eax = 0x%lX\n", temp1, eax);
        
        eax += edi;
        printf("eax + edi = 0x%lX\n", eax);
        
        edi = var_14 + eax;
        printf("var_14 + eax = 0x%lX\n\n", edi);
    } else {
        printf("=== STEP 7 SKIPPED (flag=0) ===\n\n");
    }

    /* Step 8 */
    var_4 = edi;
    if (edi & 0x80000000UL) {
        printf("=== STEP 8 (negative) ===\n");
        edi = (edi >> 1) | 0x80000000UL;
    } else {
        printf("=== STEP 8 (positive) ===\n");
        edi >>= 1;
    }
    printf("edi >> 1 = 0x%lX\n", edi);
    edi += var_4;
    printf("edi + var_4 = 0x%lX\n\n", edi);

    /* Step 9 */
    eax = E374 & 0xFFUL;
    printf("=== STEP 9 ===\n");
    printf("E374 & 0xFF = 0x%lX (%lu)\n", eax, eax);
    printf("E374 = %ld (signed)\n", (long)E374);
    
    if ((long)E374 < 0L && eax != 0UL) {
        printf("E374 < 0 and low byte != 0, subtracting 0x100\n");
        eax -= 0x100UL;
        printf("eax after -= 0x100 = 0x%lX\n", eax);
    }
    
    temp1 = (eax & 0xFFUL) ^ 0xD2UL;
    eax = (eax & 0xFFFFFF00UL) | temp1;
    printf("low 8 ^ 0xD2 = 0x%lX, eax = 0x%lX\n", temp1, eax);
    
    eax += edi;
    printf("eax + edi = 0x%lX\n", eax);

    printf("\n=== FINAL RESULT ===\n");
    printf("Authorization code: 0x%08lX\n", eax);
    
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
    B = (long)(part1 & 0xFFF);
    A = part2;

    printf("\n");
    auth = calc_auth_debug(A, B, flag);
    printf("\nThe authorization code is %08lX.\n", auth);

    return 0;
}
