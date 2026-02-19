/* keygen_msc6.c */
/* Microsoft C 6.0 - 16 bit DOS */

#include <stdio.h>

unsigned long mul32(unsigned long a, unsigned long b)
{
    /* 保證 32-bit wraparound */
    unsigned long r;
    r = a * b;
    return r;    /* MSC6 會自動取低 32-bit */
}

unsigned long generate_code(unsigned long serial)
{
    unsigned long B, C;
    unsigned long edi;

    B = serial % 10000UL;
    C = serial % 4096UL;

    edi = B;

    edi += 0x6AA602B0UL;
    edi = mul32(edi, 0x075BCD15UL);

    edi += 0x0000ACADUL;
    edi = mul32(edi, 0x4DUL);

    edi = mul32(edi, 0xB1UL);

    edi += C;
    edi = mul32(edi, 0xD2UL);

    return edi;
}

int main(void)
{
    char input[40];
    int prefix;
    unsigned long serial;
    unsigned long code;

    printf("Enter serial (XXX-YYYYYYYY): ");
    scanf("%39s", input);

    /* 用 sscanf 解析 */
    if (sscanf(input, "%d-%lu", &prefix, &serial) != 2)
    {
        printf("Invalid format!\n");
        return 1;
    }

    code = generate_code(serial);

    printf("Authorization Code = %08lX\n", code);

    return 0;
}