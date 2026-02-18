#include <stdio.h>
#include <string.h>

long A, B;
long high, low;
long mid1, mid2;
int flag = 0;

long generate_code(void)
{
    long eax, edx, ecx;
    long edi, temp;

    /* === 對應 sub_3A698 === */

    eax = low;
    edx = eax / 256;
    edi = edx;

    edi ^= 0x41;
    edi += mid1;

    temp = edi;
    edi <<= 23;

    eax = temp << 15;
    eax += edi;

    edx = mid2 / 256;
    edx ^= 0x4D;

    eax += edx;
    eax++;

    edi = temp + eax;
    edi = (edi >> 16) + edi;
    eax = edi + temp;

    eax ^= 0xACAD;

    if (flag)
    {
        long extra = temp << 10;
        extra ^= 0xB1;
        eax += extra;
    }

    edi = (eax >> 1) + eax;
    eax = edi;

    eax ^= 0xD2;

    return eax;
}

int main(void)
{
    char input[64];

    printf("Enter serial no: ");
    scanf("%63s", input);

    if (sscanf(input, "%ld-%ld", &A, &B) != 2)
    {
        printf("Format error\n");
        return 1;
    }

    if (strchr(input, '-') == NULL)
        flag = 1;

    high = B / 10000;
    low  = B % 10000;

    /* === 完整還原 main() 拆解邏輯 === */

    {
        long x = high & 0xFFF;
        long q = x / 100;
        long r = x % 100;

        mid1 = r * 100 + (low / 100);
        mid2 = q * 100 + (low % 100);
    }

    printf("The authorization code is %08lX.\n",
           generate_code());

    return 0;
}