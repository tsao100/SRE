/*
 *  serial_auth.c - 完整指针访问模拟版本
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 数据区定义 ===== */
static unsigned char data_segment[0x600];

#define DATA_BASE 0xDEAC

/* 数据区地址 */
#define ADDR_DEAC  0xDEAC
#define ADDR_DEB4  0xDEB4
#define ADDR_DEB8  0xDEB8
#define ADDR_DEBC  0xDEBC
#define ADDR_DEC0  0xDEC0  /* 指针位置 */
#define ADDR_DEC4  0xDEC4  /* 指针位置 */
#define ADDR_DECC  0xDECC  /* 指针位置 */
#define ADDR_DED0  0xDED0  /* 指针位置 */
#define ADDR_DED4  0xDED4
#define ADDR_E32C  0xE32C  /* 指针位置 */
#define ADDR_E330  0xE330  /* 指针位置 */
#define ADDR_E334  0xE334  /* 指针位置 */
#define ADDR_E338  0xE338  /* 指针位置 */

/* 数据位置（通过指针计算得出的实际地址） */
#define ADDR_DEB8_DATA  0xDEB8   /* off_DEC0 - 0x114 */
#define ADDR_DEE5_FLAG  0xDEE5   /* off_DECC - 0xC4 */
#define ADDR_E36C_DATA  0xE36C   /* off_E32C - 0x1D */
#define ADDR_E370_DATA  0xE370   /* off_E330 - 0x33 */
#define ADDR_E374_DATA  0xE374   /* off_E334 - 0x53 */
#define ADDR_E378_DATA  0xE378   /* off_E338 - 0x61 */
#define ADDR_DF74_RESULT 0xDF74  /* off_DEC4 - 0x1E4 */

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
    
    /* 常量 */
    WRITE_DWORD(ADDR_DEAC, 0x6AA602B0UL);
    WRITE_DWORD(ADDR_DEAC + 4, 0UL);
    WRITE_DWORD(ADDR_DEB4, 1UL);
    WRITE_DWORD(ADDR_DEB8, 2UL);
    WRITE_WORD(ADDR_DEBC, 1);
    
    /* 指针初始化（编译时确定的地址） */
    WRITE_DWORD(ADDR_DEC0, 0xDFCCUL);   /* (offset dword_DFC8)+4 */
    WRITE_DWORD(ADDR_DEC4, 0xE158UL);   /* (offset dword_E090)+0C8h */
    WRITE_DWORD(ADDR_DECC, 0xDFA9UL);   /* offset dword_DFA6 + 3 */
    WRITE_DWORD(ADDR_DED0, 0xDF51UL);   /* offset byte_DF51 */
    WRITE_DWORD(ADDR_E32C, 0xE389UL);   /* (offset off_E388)+1 */
    WRITE_DWORD(ADDR_E330, 0xE3A3UL);   /* offset byte_E3A3 */
    WRITE_DWORD(ADDR_E334, 0xE3C7UL);   /* (offset dbl_E3C0)+7 */
    WRITE_DWORD(ADDR_E338, 0xE3D9UL);   /* offset byte_E3D9 */
}

/* ===== 计算授权码（精确模拟指针访问）===== */
static unsigned long calc_auth(void)
{
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long E370, E374, E378;
    unsigned long ptr_val;
    unsigned char flag_byte;
    unsigned long temp1, temp2;

    /* 模拟：mov eax, off_E330; mov eax, [eax - 33h] */
    ptr_val = READ_DWORD(ADDR_E330);
    E370 = READ_DWORD(ptr_val - 0x33);
    
    /* 模拟：mov edi, off_E334; mov eax, [edi - 53h] */
    ptr_val = READ_DWORD(ADDR_E334);
    E374 = READ_DWORD(ptr_val - 0x53);
    
    /* 模拟：mov eax, off_E338; mov eax, [eax - 61h] */
    ptr_val = READ_DWORD(ADDR_E338);
    E378 = READ_DWORD(ptr_val - 0x61);
    
    /* 模拟：mov eax, off_DECC; test byte ptr[eax-0C4h], 10h */
    ptr_val = READ_DWORD(ADDR_DECC);
    flag_byte = READ_BYTE(ptr_val - 0xC4);

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    
    /* 模拟：mov eax, off_DEC0; add edi, [eax - 114h] */
    ptr_val = READ_DWORD(ADDR_DEC0);
    edi += READ_DWORD(ptr_val - 0x114);
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
    /* 再次读取 E374：mov eax, off_E334; mov eax, [eax - 53h] */
    ptr_val = READ_DWORD(ADDR_E334);
    E374 = READ_DWORD(ptr_val - 0x53);
    
    eax = E374 & 0xFFUL;
    if ((long)E374 < 0L && eax != 0UL)
        eax -= 0x100UL;
    eax = (eax & 0xFFFFFF00UL) | ((eax & 0xFFUL) ^ 0xD2UL);
    eax += edi;

    /* 模拟：mov eax, off_DEC4; mov [eax - 1E4h], edi */
    ptr_val = READ_DWORD(ADDR_DEC4);
    WRITE_DWORD(ptr_val - 0x1E4, eax);
    
    return eax;
}

/* ===== 预处理序列号（模拟 main 函数）===== */
static void preprocess_serial(long A, long B, int is_long_prefix)
{
    long E36C;
    unsigned long ptr_val;

    E36C = A / 10000L;

    /* 模拟：mov edx, off_E32C; mov [edx-1Dh], eax */
    ptr_val = READ_DWORD(ADDR_E32C);
    WRITE_DWORD(ptr_val - 0x1D, (unsigned long)E36C);
    
    /* 模拟：mov eax, off_E330; mov [eax-33h], edx */
    ptr_val = READ_DWORD(ADDR_E330);
    WRITE_DWORD(ptr_val - 0x33, (unsigned long)(A % 10000L));
    
    /* 模拟：mov eax, off_E334; mov [eax-53h], edx */
    ptr_val = READ_DWORD(ADDR_E334);
    WRITE_DWORD(ptr_val - 0x53, (unsigned long)((E36C % 100L) + (B % 100L) * 100L));
    
    /* 模拟：mov edx, off_E338; mov [edx-61h], eax */
    ptr_val = READ_DWORD(ADDR_E338);
    WRITE_DWORD(ptr_val - 0x61, (unsigned long)((E36C / 100L) + (B / 100L) * 100L));
    
    /* 模拟：mov eax, off_DECC; or byte ptr[eax-0C4h], 10h */
    ptr_val = READ_DWORD(ADDR_DECC);
    if (is_long_prefix)
        WRITE_BYTE(ptr_val - 0xC4, READ_BYTE(ptr_val - 0xC4) | 0x10);
}

/* ===== 主程序 ===== */
int main(void)
{
    char serial[20];
    int part1, n, is_long_prefix;
    long part2, A, B;
    unsigned long ptr_val;

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

    /* 模拟：mov eax, off_DEC4; mov eax, [eax - 1E4h] */
    ptr_val = READ_DWORD(ADDR_DEC4);
    printf("The authorization code is %08lX.\n", READ_DWORD(ptr_val - 0x1E4));

    return 0;
}
