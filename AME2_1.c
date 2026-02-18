/*
 *  serial_auth.c - 完整数据区初始化版本
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 数据区定义 ===== */
static unsigned char data_segment[0x600];

#define DATA_BASE 0xDEAC

/* 关键地址 */
#define ADDR_DEAC  0xDEAC
#define ADDR_DEB4  0xDEB4
#define ADDR_DEB8  0xDEB8
#define ADDR_DEBC  0xDEBC
#define ADDR_DEBE  0xDEBE
#define ADDR_DEC0  0xDEC0
#define ADDR_DEC4  0xDEC4
#define ADDR_DECC  0xDECC
#define ADDR_DED0  0xDED0
#define ADDR_DED4  0xDED4  /* 字符串缓冲区 */
#define ADDR_DED6  0xDED6
#define ADDR_DEDD  0xDEDD
#define ADDR_DEE0  0xDEE0
#define ADDR_DEE4  0xDEE4  /* sscanf 第一个输出 */
#define ADDR_DEE8  0xDEE8  /* sscanf 第二个输出 */
#define ADDR_E008  0xE008  /* 标志字节 */
#define ADDR_E36C  0xE36C
#define ADDR_E370  0xE370
#define ADDR_E374  0xE374
#define ADDR_E378  0xE378
#define ADDR_DF74  0xDF74

#define ADDR_TO_IDX(addr) ((addr) - DATA_BASE)

#define READ_DWORD(addr)  (*(unsigned long*)(data_segment + ADDR_TO_IDX(addr)))
#define WRITE_DWORD(addr, val)  (*(unsigned long*)(data_segment + ADDR_TO_IDX(addr)) = (val))
#define READ_WORD(addr)   (*(unsigned short*)(data_segment + ADDR_TO_IDX(addr)))
#define WRITE_WORD(addr, val)   (*(unsigned short*)(data_segment + ADDR_TO_IDX(addr)) = (val))
#define READ_BYTE(addr)   (*(unsigned char*)(data_segment + ADDR_TO_IDX(addr)))
#define WRITE_BYTE(addr, val)   (*(unsigned char*)(data_segment + ADDR_TO_IDX(addr)) = (val))

/* ===== 初始化数据区 ===== */
static void init_data_segment(void)
{
    memset(data_segment, 0, sizeof(data_segment));
    
    /* 编译时确定的常量 */
    WRITE_DWORD(ADDR_DEAC, 0x6AA602B0UL);  /* dword_DEAC */
    WRITE_DWORD(ADDR_DEAC + 4, 0UL);
    
    WRITE_DWORD(ADDR_DEB4, 1UL);           /* dword_DEB4 = 1 */
    WRITE_DWORD(ADDR_DEB8, 2UL);           /* dword_DEB8 = 2 */
    WRITE_WORD(ADDR_DEBC, 1);              /* word_DEBC = 1 */
    WRITE_WORD(ADDR_DEBE, 0);              /* word_DEBE = 0 */
    
    /* 指针初始化（这些是编译时计算的地址，在 C 中可以不初始化，
       因为我们直接用绝对地址访问）*/
    
    /* 初始字符串数据（这可能是调试残留） */
    WRITE_BYTE(ADDR_DED4, 0x31);      /* '1' */
    WRITE_BYTE(ADDR_DED4 + 1, 0x31);  /* '1' */
    WRITE_BYTE(ADDR_DED6, 0x30);      /* '0' */
    WRITE_BYTE(ADDR_DED6 + 1, 0x2D);  /* '-' */
    /* "1030" 以小端序存储 */
    WRITE_BYTE(ADDR_DED6 + 2, 0x31);  /* '1' */
    WRITE_BYTE(ADDR_DED6 + 3, 0x30);  /* '0' */
    WRITE_BYTE(ADDR_DED6 + 4, 0x33);  /* '3' */
    WRITE_BYTE(ADDR_DED6 + 5, 0x30);  /* '0' */
    WRITE_BYTE(ADDR_DED6 + 6, 0x34);  /* '4' */
    WRITE_BYTE(ADDR_DEDD, 0x31);      /* '1' */
    WRITE_BYTE(ADDR_DEDD + 1, 0x31);  /* '1' */
    WRITE_BYTE(ADDR_DEDD + 2, 0x31);  /* '1' */
    WRITE_BYTE(ADDR_DEE0, 0x00);      /* 字符串结束符 */
    
    /* 初始的测试数据（可能是调试残留）*/
    WRITE_DWORD(ADDR_DEE4, 0UL);
    WRITE_DWORD(ADDR_DEE8, 0x9D3A6FUL);  /* = 10304111 */
}

/* ===== 计算授权码 ===== */
static unsigned long calc_auth(void)
{
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long E370, E374, E378;
    unsigned char flag_byte;
    unsigned long temp1, temp2;

    /* 从数据区读取 */
    E370 = READ_DWORD(ADDR_E370);
    E374 = READ_DWORD(ADDR_E374);
    E378 = READ_DWORD(ADDR_E378);
    flag_byte = READ_BYTE(ADDR_E008);

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += READ_DWORD(ADDR_DEB8);
    var_4 = edi;

    /* Step 2 */
    temp1 = var_4 << 23;
    temp2 = var_4 << 15;
    var_8 = temp1 + temp2;

    /* Step 3 */
    eax = (E374 / 256UL) ^ 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 */
    temp1 = var_10 >> 8;
    temp2 = temp1 >> 8;
    edi = temp2 + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = (var_C ^ 0x32UL) + eax;

    /* Step 7 */
    if (flag_byte & 0x10) {
        var_14 = edi;
        edi <<= 10;
        edx = E378 % 256UL;
        eax = E378 / 256UL;
        eax ^= edx;
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

    WRITE_DWORD(ADDR_DF74, eax);
    return eax;
}

/* ===== 预处理序列号 ===== */
static void preprocess_serial(long A, long B, int is_long_prefix)
{
    long E36C;

    E36C = A / 10000L;

    WRITE_DWORD(ADDR_E36C, (unsigned long)E36C);
    WRITE_DWORD(ADDR_E370, (unsigned long)(A % 10000L));
    WRITE_DWORD(ADDR_E374, (unsigned long)((E36C % 100L) + (B % 100L) * 100L));
    WRITE_DWORD(ADDR_E378, (unsigned long)((E36C / 100L) + (B / 100L) * 100L));
    WRITE_BYTE(ADDR_E008, is_long_prefix ? 0x10 : 0x00);
}

/* ===== 主程序 ===== */
int main(void)
{
    char serial[20];
    int part1, n, is_long_prefix;
    long part2, A, B;

    init_data_segment();

    printf("Enter serial no: ");
    scanf("%s", serial);

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    is_long_prefix = (serial[2] != '-') ? 1 : 0;
    B = (long)(part1 & 0xFFF);
    A = part2;

    preprocess_serial(A, B, is_long_prefix);
    calc_auth();

    printf("The authorization code is %08lX.\n", READ_DWORD(ADDR_DF74));

    return 0;
}
