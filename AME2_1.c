/*
 *  serial_auth.c - 最终验证版
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>

static unsigned long calc_auth(long A, long B, int flag)
{
    unsigned long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax;

    /* main 中的计算 */
    E36C = (unsigned long)(A / 10000L);
    E370 = (unsigned long)(A % 10000L);
    E374 = (E36C % 100UL) + ((B % 100UL) * 100UL);
    E378 = (E36C / 100UL) + ((B / 100UL) * 100UL);

    /* Step 1 */
    var_C = E370 % 256UL;
    edi = (E370 / 256UL) ^ 0x41UL;
    edi += 2UL;
    var_4 = edi;

    /* Step 2 */
    var_8 = (var_4 << 23) + (var_4 << 15);

    /* Step 3 */
    eax = (E374 / 256UL) ^ 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 */
    if (var_10 & 0x80000000UL)
        edi = ((var_10 >> 16) | 0xFFFF0000UL);
    else
        edi = var_10 >> 16;
    edi += var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;

    /* Step 7 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        eax = (E378 / 256UL) ^ (E378 % 256UL);
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
    eax = E374 & 0xFFUL;
    if ((long)E374 < 0L && eax != 0UL)
        eax -= 0x100UL;
    eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xD2UL);
    
    return eax + edi;
}

int main(void)
{
    char serial[20];
    int part1, n, flag;
    long part2;

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    flag = (serial[2] != '-') ? 1 : 0;
    
    printf("The authorization code is %08lX.\n", 
           calc_auth(part2, (long)(part1 & 0xFFF), flag));

    return 0;
}
