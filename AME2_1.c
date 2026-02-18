/* BIT-PERFECT RECONSTRUCTION
   MSC 6.0 compatible
*/

#include <stdio.h>
#include <string.h>

/* 模擬 32-bit */
typedef long s32;
typedef unsigned long u32;

/* 模擬整個資料段 (只要足夠大) */
unsigned char mem[0x4000];

/* 模擬原始 label offset (根據你貼的資料段相對順序) */
#define BASE        0x1000
#define OFF_DED0    (BASE + 0x200)
#define OFF_E32C    (BASE + 0x300)
#define OFF_E330    (BASE + 0x304)
#define OFF_E334    (BASE + 0x308)
#define OFF_E338    (BASE + 0x30C)
#define OFF_DECC    (BASE + 0x210)

/* 強制 32-bit wrap */
u32 U32(u32 x) { return x & 0xFFFFFFFFUL; }

/* 寫入 32-bit */
void W32(u32 addr, u32 val)
{
    *(u32 *)(mem + (addr - BASE)) = U32(val);
}

/* 讀取 32-bit */
u32 R32(u32 addr)
{
    return *(u32 *)(mem + (addr - BASE));
}

/* 核心 sub_3A698 */
u32 generate(void)
{
    s32 eax, edx, ecx;
    s32 edi;
    s32 tmp4, tmpC, tmp10;

    /* mov eax, off_E330
       mov eax, [eax - 33h] */
    eax = R32(OFF_E330 - 0x33);

    ecx = 0x100;
    edx = eax / ecx;

    edi = edx;
    edi ^= 0x41;

    eax = R32(OFF_DEC0 - 0x114);
    edi += eax;

    tmp4 = edi;

    edi = U32(edi << 23);

    eax = U32(tmp4 << 15);
    eax = U32(eax + edi);

    tmp10 = eax;

    eax = R32(OFF_E334 - 0x53);

    edx = eax / ecx;
    eax = U32(edx ^ 0x4D);

    eax = U32(eax + tmp10 + 1);

    edi = U32(tmp4 + eax);

    tmpC = edi;

    edi = (edi >> 16) + tmpC;
    eax = U32(tmpC + edi);

    eax ^= 0xACAD;

    /* flag 測試 */
    if (R32(OFF_DECC - 0xC5) & 0x10)
    {
        s32 extra = tmpC << 10;
        extra ^= 0xB1;
        eax = U32(eax + extra);
    }

    edi = (eax >> 1) + eax;
    eax = U32(edi ^ 0xD2);

    return eax;
}

int main(void)
{
    long A, B;
    char input[64];

    printf("Enter serial no: ");
    scanf("%63s", input);

    if (sscanf(input, "%ld-%ld", &A, &B) != 2)
    {
        printf("Format error\n");
        return 1;
    }

    /* 模擬 main() 內的拆解 */

    s32 high = B / 10000;
    s32 low  = B % 10000;

    W32(OFF_E32C - 0x1D, high);
    W32(OFF_E330 - 0x33, low);

    /* 低 12 bits */
    s32 x = high & 0xFFF;
    s32 q = x / 100;
    s32 r = x % 100;

    s32 mid1 = r * 100 + (low / 100);
    s32 mid2 = q * 100 + (low % 100);

    W32(OFF_E334 - 0x53, mid1);
    W32(OFF_E338 - 0x61, mid2);

    /* flag */
    if (strchr(input, '-') == NULL)
        W32(OFF_DECC - 0xC5, 0x10);
    else
        W32(OFF_DECC - 0xC5, 0);

    printf("The authorization code is %08lX.\n",
           generate());

    return 0;
}