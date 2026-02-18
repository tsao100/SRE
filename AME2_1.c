/*
 *  serial_auth.c - 使用全局数据区版本
 *  Build: cl /AS /Ox serial_auth.c
 */

#include <stdio.h>
#include <stdlib.h>

/* ====== 全局数据区（模拟组合语言的数据段）====== */

/* 关键数据存储位置 */
static unsigned long data_E36C = 0;   /* A / 10000 */
static unsigned long data_E370 = 0;   /* A % 10000 */
static unsigned long data_E374 = 0;   /* 合成值1 */
static unsigned long data_E378 = 0;   /* 合成值2 */
static unsigned char data_flag = 0;   /* 条件标志 */

static unsigned long data_DEB8 = 2;   /* 常数 */
static unsigned long data_result = 0; /* 最终授权码 */

/* ====== 计算授权码（读取全局数据区）====== */
static void calc_auth(void)
{
    unsigned long var_4, var_8, var_C, var_10, var_14;
    unsigned long edi, eax, edx;
    unsigned long E370, E374, E378;
    unsigned char flag;

    /* 从全局数据区读取 */
    E370 = data_E370;
    E374 = data_E374;
    E378 = data_E378;
    flag = data_flag;

    /* Step 1 */
    edx = E370 % 256UL;
    eax = E370 / 256UL;
    var_C = edx;
    edi = eax ^ 0x41UL;
    edi += data_DEB8;  /* +2 */
    var_4 = edi;

    /* Step 2 */
    edi = var_4 << 23;
    eax = var_4 << 15;
    var_8 = eax + edi;

    /* Step 3 */
    eax = E374 / 256UL;
    eax ^= 0x4DUL;
    eax += var_8 + 1UL;
    edi = var_4 + eax;
    var_10 = edi;

    /* Step 4 - 使用除法避免移位bug */
    edi = (var_10 / 65536UL) + var_10;
    eax = var_10 + edi;
    edi = eax;

    /* Step 5 */
    eax = 0UL ^ edi;
    eax = (eax & 0xFFFF0000UL) | ((eax & 0xFFFFUL) ^ 0xACADUL);
    edi = var_C ^ 0x32UL;
    edi += eax;

    /* Step 7 - 条件分支 */
    if (flag & 0x10) {
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

    /* 写回全局数据区 */
    data_result = eax;
}

/* ====== 预处理序列号（写入全局数据区）====== */
static void preprocess_serial(long A, long B, unsigned char sep_flag)
{
    long E36C;
    unsigned long temp1, temp2;
    
    /* 计算基础中间值 */
    E36C = A / 10000L;
    data_E36C = (unsigned long)E36C;
    data_E370 = (unsigned long)(A % 10000L);
    
    /* 
     * 计算 E374: (E36C % 100) + (B % 100) * 100
     * 组合语言用 lea 实现乘法：
     *   lea edx, [edx+edx*4]  ; *5
     *   lea edx, [edx+edx*4]  ; *5 -> *25
     *   lea edx, ds:0[edx*4]  ; *4 -> *100
     */
    temp1 = (unsigned long)(B % 100L);
    temp1 = (temp1 + temp1 * 4UL);     /* *5 */
    temp1 = (temp1 + temp1 * 4UL);     /* *25 */
    temp1 = temp1 * 4UL;               /* *100 */
    
    temp2 = (unsigned long)(E36C % 100L);
    data_E374 = temp2 + temp1;
    
    /* 
     * 计算 E378: (E36C / 100) + (B / 100) * 100
     */
    temp1 = (unsigned long)(B / 100L);
    temp1 = (temp1 + temp1 * 4UL);     /* *5 */
    temp1 = (temp1 + temp1 * 4UL);     /* *25 */
    temp1 = temp1 * 4UL;               /* *100 */
    
    temp2 = (unsigned long)(E36C / 100L);
    data_E378 = temp2 + temp1;
    
    /* 设置条件标志 */
    data_flag = sep_flag ? 0x10 : 0x00;
}

/* ====== 主程序 ====== */
int main(void)
{
    char serial[20];
    int part1, n;
    long part2, A, B;
    unsigned char sep_flag;

    printf("Enter serial no: ");
    if (scanf("%s", serial) != 1) {
        printf("Input error!\n");
        return 1;
    }

    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        printf("Invalid format!\n");
        return 1;
    }

    /* 检查分隔符位置（模拟 byte_DED6 检查）*/
    sep_flag = (serial[2] != '-') ? 1 : 0;

    /* 准备输入参数 */
    B = (long)((unsigned)part1 & 0x0FFFU);
    A = part2;

    /* 预处理：计算中间值并写入数据区 */
    preprocess_serial(A, B, sep_flag);

    /* 计算授权码（从数据区读取） */
    calc_auth();

    /* 输出结果 */
    printf("The authorization code is %08lX.\n", data_result);

    return 0;
}
