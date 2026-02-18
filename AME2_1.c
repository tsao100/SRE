/*
 *  serial_auth.c - 精确内存布局模拟版
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 数据区完整布局（0xDEAC ~ 0xE3D9）===== */
static unsigned char data_segment[0x600];

/* 各数据项相对于 data_segment 起始的偏移 */
#define BASE_OFFSET    0xDEAC
#define OFF_DEAC       (0xDEAC - BASE_OFFSET)
#define OFF_DEB8       (0xDEB8 - BASE_OFFSET)
#define OFF_DEE4       (0xDEE4 - BASE_OFFSET)  /* 序列号第一段 */
#define OFF_DEE8       (0xDEE8 - BASE_OFFSET)  /* 序列号第二段 */
#define OFF_E008       (0xE008 - BASE_OFFSET)  /* 标志字节位置 */
#define OFF_E30F       (0xE30F - BASE_OFFSET)  /* E36C 写入位置 */
#define OFF_E370       (0xE370 - BASE_OFFSET)  /* A % 10000 */
#define OFF_E374       (0xE374 - BASE_OFFSET)  /* 合成值1 */
#define OFF_E378       (0xE378 - BASE_OFFSET)  /* 合成值2 */
#define OFF_DF74       (0xDF74 - BASE_OFFSET)  /* 结果写入位置 */

/* 读写数据区的宏 */
#define READ_DWORD(off)  (*(unsigned long*)(data_segment + (off)))
#define WRITE_DWORD(off, val)  (*(unsigned long*)(data_segment + (off)) = (val))
#define READ_BYTE(off)   (*(unsigned char*)(data_segment + (off)))
#define WRITE_BYTE(off, val)   (*(unsigned char*)(data_segment + (off)) = (val))

/* 初始化数据区（编译时确定的值） */
static void init_data_segment(void)
{
    memset(data_segment, 0, sizeof(data_segment));
    
    /* dword_DEB8 = 2 */
    WRITE_DWORD(OFF_DEB8, 2UL);
}

/* 计算授权码 */
static unsigned long calc_auth(void)
{
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long E370, E374, E378;
    unsigned char flag_byte;

    /* 从数据区读取 */
    E370 = READ_DWORD(OFF_E370);
    E374 = READ_DWORD(OFF_E374);
    E378 = READ_DWORD(OFF_E378);
    flag_byte = READ_BYTE(OFF_E008);

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += READ_DWORD(OFF_DEB8);  /* +2 */
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

    /* Step 4 - 分步移位 */
    {
        unsigned long temp;
        temp = var_10 >> 8;
        temp = temp >> 8;
        edi = temp + var_10;
        eax = var_10 + edi;
        edi = eax;
    }

    /* Step 5 */
    eax = edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;

    /* Step 7 */
    if (flag_byte & 0x10) {
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
    eax += edi;

    /* 写入结果到数据区 */
    WRITE_DWORD(OFF_DF74, eax);
    
    return eax;
}

/* 预处理序列号（对应 main 函数） */
static void preprocess_serial(long A, long B, int is_long_prefix)
{
    long E36C;
    unsigned long temp;

    E36C = A / 10000L;

    /* 写入 E36C */
    WRITE_DWORD(OFF_E30F, (unsigned long)E36C);

    /* 写入 E370 = A % 10000 */
    WRITE_DWORD(OFF_E370, (unsigned long)(A % 10000L));

    /* 计算并写入 E374 */
    temp = (unsigned long)((E36C % 100L) + (B % 100L) * 100L);
    WRITE_DWORD(OFF_E374, temp);

    /* 计算并写入 E378 */
    temp = (unsigned long)((E36C / 100L) + (B / 100L) * 100L);
    WRITE_DWORD(OFF_E378, temp);

    /* 设置标志 */
    WRITE_BYTE(OFF_E008, is_long_prefix ? 0x10 : 0x00);
}

int main(void)
{
    char serial[20];
    int part1, n, is_long_prefix;
    long part2, A, B;
    unsigned long auth;

    /* 初始化数据区 */
    init_data_segment();

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    /* 判断前半段长度 */
    is_long_prefix = (serial[2] != '-') ? 1 : 0;

    B = (long)(part1 & 0xFFF);
    A = part2;

    /* 预处理并写入数据区 */
    preprocess_serial(A, B, is_long_prefix);

    /* 计算授权码 */
    auth = calc_auth();

    printf("The authorization code is %08lX.\n", auth);

    return 0;
}
