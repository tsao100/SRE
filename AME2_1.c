/* MSC 6.0 COMPATIBLE VERSION */

#include <stdio.h>
#include <string.h>

typedef long s32;
typedef unsigned long u32;

/* 模擬資料段 */
unsigned char mem[0x4000];

/* ====== OFFSET 定義 ====== */

#define BASE        0x1000

#define OFF_DEC0    (BASE + 0x100)
#define OFF_DEC4    (BASE + 0x104)
#define OFF_DECC    (BASE + 0x10C)

#define OFF_E32C    (BASE + 0x300)
#define OFF_E330    (BASE + 0x304)
#define OFF_E334    (BASE + 0x308)
#define OFF_E338    (BASE + 0x30C)

/* ====== 32-bit wrap ====== */

u32 U32(u32 x)
{
    return x & 0xFFFFFFFFUL;
}

void W32(u32 addr, u32 val)
{
    *(u32 *)(mem + (addr - BASE)) = U32(val);
}

u32 R32(u32 addr)
{
    return *(u32 *)(mem + (addr - BASE));
}

/* ====== sub_3A698 ====== */

u32 generate(void)
{
    s32 eax;
    s32 edx;
    s32 ecx;
    s32 edi;

    s32 tmp4;
    s32 tmpC;
    s32 tmp10;
    s32 extra;

    ecx = 0x100;

    eax = R32(OFF_E330 - 0x33);
    edx = eax / ecx;

    edi = edx;
    edi = edi ^ 0x41;

    eax = R32(OFF_DEC0 - 0x114);
    edi = edi + eax;

    tmp4 = edi;

    edi = U32(edi << 23);

    eax = U32(tmp4 << 15);
    eax = U32(eax + edi);

    tmp10 = eax;

    eax = R32(OFF_E334 - 0x53);

    edx = eax / ecx;
    eax = edx ^ 0x4D;

    eax = U32(eax + tmp10 + 1);

    edi = tmp4 + eax;

    tmpC = edi;

    edi = (edi >> 16) + tmpC;
    eax = tmpC + edi;

    eax = eax ^ 0xACAD;

    if (R32(OFF_DECC - 0xC5) & 0x10)
    {
        extra = tmpC << 10;
        extra = extra ^ 0xB1;
        eax = U32(eax + extra);
    }

    edi = (eax >> 1) + eax;
    eax = edi ^ 0xD2;

    return U32(eax);
}

/* ====== main ====== */

int main(void)
{
    long A;
    long B;

    s32 high;
    s32 low;
    s32 x;
    s32 q;
    s32 r;
    s32 mid1;
    s32 mid2;

    char input[64];
    char *p;

    printf("Enter serial no: ");
    scanf("%63s", input);

    if (sscanf(input, "%ld-%ld", &A, &B) != 2)
    {
        printf("Format error\n");
        return 1;
    }

    high = B / 10000;
    low  = B % 10000;

    W32(OFF_E32C - 0x1D, high);
    W32(OFF_E330 - 0x33, low);

    x = high & 0xFFF;

    q = x / 100;
    r = x % 100;

    mid1 = r * 100 + (low / 100);
    mid2 = q * 100 + (low % 100);

    W32(OFF_E334 - 0x53, mid1);
    W32(OFF_E338 - 0x61, mid2);

    p = strchr(input, '-');

    if (p == NULL)
        W32(OFF_DECC - 0xC5, 0x10);
    else
        W32(OFF_DECC - 0xC5, 0);

    printf("The authorization code is %08lX.\n",
           generate());

    return 0;
}