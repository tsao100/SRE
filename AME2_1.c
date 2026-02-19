/* keygen.c */
/* MSC 6.0 16-bit compatible */

#include <stdio.h>
#include <stdlib.h>

unsigned long generate_code(unsigned long serial)
{
    unsigned long A, B, C;
    unsigned long edi;

    /* split serial */
    A = serial / 10000UL;
    B = serial % 10000UL;
    C = serial % 4096UL;

    /* algorithm */
    edi = B;

    edi += 0x6AA602B0UL;
    edi &= 0xFFFFFFFFUL;

    edi *= 0x075BCD15UL;
    edi &= 0xFFFFFFFFUL;

    edi += 0x0000ACADUL;
    edi &= 0xFFFFFFFFUL;

    edi *= 0x4DUL;
    edi &= 0xFFFFFFFFUL;

    edi *= 0xB1UL;
    edi &= 0xFFFFFFFFUL;

    edi += C;
    edi &= 0xFFFFFFFFUL;

    edi *= 0xD2UL;
    edi &= 0xFFFFFFFFUL;

    return edi;
}

int main(void)
{
    char input[32];
    unsigned long serial;
    unsigned long code;

    printf("Enter serial (format XXX-YYYYYYYY): ");
    scanf("%31s", input);

    /* 取 '-' 後面的數字 */
    serial = strtoul(strchr(input, '-') + 1, NULL, 10);

    code = generate_code(serial);

    printf("Authorization Code = %08lX\n", code);

    return 0;
}