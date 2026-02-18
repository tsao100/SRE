/*
 *  serial_auth.c - 精确内存布局版
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 数据区结构（从 DEAC 到 E3D9）===== */
typedef struct {
    unsigned long dword_DEAC[2];    /* +0x00 */
    unsigned long dword_DEB4;       /* +0x08 */
    unsigned long dword_DEB8;       /* +0x0C: 值为2 */
    unsigned long dword_DEBC;       /* +0x10 */
    /* ... 省略中间大量数据 ... */
    unsigned char padding[0x2D0];   /* 填充到正确位置 */
    unsigned long data_E370;        /* E370 相对 DEAC 的偏移 */
    unsigned long data_E374;        /* E374 */
    unsigned long data_E378;        /* E378 */
    unsigned char flag_byte;        /* 标志 */
} DataArea;

static DataArea g_data;

static void init_data(void)
{
    memset(&g_data, 0, sizeof(g_data));
    g_data.dword_DEB8 = 2UL;
}

static unsigned long calc_auth(void)
{
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long E370, E374, E378;
    unsigned char flag;

    /* 从数据区读取 */
    E370 = g_data.data_E370;
    E374 = g_data.data_E374;
    E378 = g_data.data_E378;
    flag = g_data.flag_byte;

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += g_data.dword_DEB8;
    var_4 = edi;

    /* Step 2 */
    edi = var_4 << 23;
    eax = var_4 << 15;
    var_8 = eax + edi;

    /* Step 3 */
    eax = (E374 / 256UL) ^ 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 */
    {
        unsigned long temp1, temp2;
        temp1 = var_10 >> 8;
        temp2 = temp1 >> 8;  /* 分两次移位 */
        edi = temp2 + var_10;
        eax = var_10 + edi;
        edi = eax;
    }

    /* Step 5 */
    eax = edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;

    /* Step 7 */
    if (flag & 0x10) {
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
    int part1, n;
    long part2, A, B, E36C;

    init_data();

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    B = (long)(part1 & 0xFFF);
    A = part2;
    E36C = A / 10000L;

    /* 写入数据区 */
    g_data.data_E370 = (unsigned long)(A % 10000L);
    g_data.data_E374 = (unsigned long)((E36C % 100L) + (B % 100L) * 100L);
    g_data.data_E378 = (unsigned long)((E36C / 100L) + (B / 100L) * 100L);
    g_data.flag_byte = (serial[2] != '-') ? 0x10 : 0x00;

    printf("The authorization code is %08lX.\n", calc_auth());

    return 0;
}
