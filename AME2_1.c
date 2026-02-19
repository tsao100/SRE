/*  MSC 6.0 16-bit DOS version  */

#include <stdio.h>

/* ------------------------------ */
/*  授權碼產生核心演算法          */
/* ------------------------------ */

unsigned long GenerateCode(int part1, unsigned long part2)
{
    unsigned long t1;
    unsigned long t2;
    unsigned long t3;
    unsigned long t4;
    unsigned long result;
    unsigned long temp;
    unsigned long quotient;
    unsigned long remainder;

    /* ------------------------------------ */
    /*  第一段：處理 part2                  */
    /* ------------------------------------ */

    quotient  = part2 / 256UL;
    remainder = part2 % 256UL;

    t1 = quotient ^ 0x41UL;
    t1 += 0x009D3A6FUL;

    /* ------------------------------------ */
    /*  巨大位移混合                         */
    /* ------------------------------------ */

    t2 = (t1 << 23) + (t1 << 15);

    /* ------------------------------------ */
    /*  混入 part1                          */
    /* ------------------------------------ */

    quotient  = part1 / 256UL;
    remainder = part1 % 256UL;

    t3 = (quotient ^ 0x4DUL);
    t3 = t3 + t2 + 1UL;

    t4 = t1 + t3;

    /* ------------------------------------ */
    /*  第二層混淆                          */
    /* ------------------------------------ */

    temp = (t4 >> 16);
    temp += t4;

    result = t4 + temp;

    result ^= 0xACADUL;

    /* ------------------------------------ */
    /*  第三層混淆                          */
    /* ------------------------------------ */

    temp = (result << 10);

    quotient  = part1 / 256UL;
    remainder = part1 % 256UL;

    temp += (quotient ^ remainder ^ 0xB1UL);

    result += temp;

    /* ------------------------------------ */
    /*  最終混合                            */
    /* ------------------------------------ */

    temp = result >> 1;
    result += temp;

    result += ((part1 & 0xFF) ^ 0xD2UL);

    return result;
}

/* ------------------------------ */
/*  主程式                        */
/* ------------------------------ */

int main(void)
{
    int part1;
    unsigned long part2;
    unsigned long code;

    printf("Enter serial no (e.g. 110-10304111): ");
    scanf("%d-%lu", &part1, &part2);

    code = GenerateCode(part1, part2);

    printf("The authorization code is %08lX\n", code);

    return 0;
}