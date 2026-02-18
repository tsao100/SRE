/* MSC 6.0 exact behavioral reconstruction */

#include <stdio.h>
#include <string.h>

/* 全域資料 */
long gA;      /* sscanf 第一段 */
long gB;      /* sscanf 第二段 */
int  gFlag = 0;


/* 這函式完全模擬 sub_3A698 */
long generate_code(void)
{
    long eax, edx, ecx;
    long edi;
    long temp4, temp8, tempC, temp10;

    /* -------- 第一段 -------- */
    eax = gB;
    ecx = 256;
    edx = eax / ecx;        /* idiv 100h */
    edi = edx;

    edi ^= 0x41;

    temp4 = edi;

    edi <<= 23;

    eax = temp4 << 15;
    eax += edi;

    /* -------- 第二段 -------- */
    edx = gA / 256;
    edx ^= 0x4D;

    eax += edx;
    eax++;

    edi = temp4 + eax;
    temp10 = edi;

    edi = (edi >> 16) + temp10;
    eax = temp10 + edi;

    /* 模擬 imul eax, esi, 75BCD15h
       但 esi = 0 所以結果 = 0 */
    eax = 0 ^ eax;

    eax ^= 0xACAD;

    /* 特殊旗標 */
    if (gFlag)
    {
        long extra = temp10 << 10;
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

    if (sscanf(input, "%ld-%ld", &gA, &gB) != 2)
    {
        printf("Format error!\n");
        return 1;
    }

    /* 判斷是否含 '-' */
    if (strchr(input, '-') == NULL)
        gFlag = 1;

    printf("The authorization code is %08lX.\n",
           generate_code());

    return 0;
}