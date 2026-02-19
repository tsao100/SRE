#include <stdio.h>
#include <stdlib.h>

static unsigned long calc_auth_debug(long A, long B, int flag)
{
    unsigned long E36C, E370, E374, E378;
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx, temp1, temp2;

    E36C = (unsigned long)(A / 10000L);
    E370 = (unsigned long)(A % 10000L);
    E374 = (E36C % 100UL) + (B % 100UL) * 100UL;
    E378 = (E36C / 100UL) + (B / 100UL) * 100UL;

    printf("INPUT: A=%ld, B=%ld, flag=%d\n", A, B, flag);
    printf("E36C=%lu, E370=%lu, E374=%lu, E378=%lu\n\n", E36C, E370, E374, E378);

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = (eax ^ 0x41UL) + 2UL;
    var_4 = edi;
    printf("Step1: var_4=0x%lX\n", var_4);

    /* Step 2 */
    var_8 = (var_4 << 23) + (var_4 << 15);
    printf("Step2: var_8=0x%lX\n", var_8);

    /* Step 3 */
    eax = (E374 / 256UL) ^ 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;
    printf("Step3: var_10=0x%lX\n", var_10);

    /* Step 4 - 详细输出 */
    temp1 = var_10 >> 8;
    temp2 = temp1 >> 8;
    printf("Step4: var_10>>16 = 0x%lX\n", temp2);
    
    edi = temp2 + var_10;
    printf("Step4: edi = temp2+var_10 = 0x%lX + 0x%lX = 0x%lX\n", 
           temp2, var_10, edi);
    
    eax = var_10 + edi;
    printf("Step4: eax = var_10+edi = 0x%lX + 0x%lX = 0x%lX\n", 
           var_10, edi, eax);
    
    edi = eax;
    printf("Step4: final edi = 0x%lX\n\n", edi);

    /* Step 5 */
    eax = (edi & 0xFFFF0000UL) | ((edi & 0xFFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;
    printf("Step5: edi=0x%lX\n\n", edi);

    /* Step 7 */
    if (flag) {
        var_14 = edi;
        edi <<= 10;
        edx = E378 % 256UL;
        eax = E378 / 256UL;
        eax ^= edx;
        eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xB1UL);
        eax += edi;
        edi = var_14 + eax;
        printf("Step7: edi=0x%lX\n\n", edi);
    }

    /* Step 8 */
    var_4 = edi;
    if (edi & 0x80000000UL)
        edi = (edi >> 1) | 0x80000000UL;
    else
        edi >>= 1;
    edi += var_4;
    printf("Step8: edi=0x%lX\n\n", edi);

    /* Step 9 */
    eax = E374 & 0xFFUL;
    if ((long)E374 < 0L && eax != 0UL)
        eax -= 0x100UL;
    eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xD2UL);
    eax += edi;
    printf("Step9: result=0x%lX\n\n", eax);

    return eax;
}

int main(void)
{
    unsigned long r1, r2, r3;
    
    r1 = calc_auth_debug(10524473L, 110L, 1);
    printf("Result: 0x%08lX (expect 0x631CBEB9)\n\n", r1);
    
    r2 = calc_auth_debug(10304111L, 110L, 1);
    printf("Result: 0x%08lX (expect 0x6AA602B0)\n\n", r2);
    
    r3 = calc_auth_debug(10058907L, 117L, 1);
    printf("Result: 0x%08lX (expect 0xF258421D)\n\n", r3);
    
    return 0;
}
