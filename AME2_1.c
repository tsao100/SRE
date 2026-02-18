/*
 * Serial Number to Authorization Code Generator
 * Target: MSC 6.0, DOS 16-bit
 * 
 * Algorithm: Converts input serial (format: XXXX-YYYYYYYY) to authorization code
 * All bit operations preserve exact semantics from assembly
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

/* ===== 全域變數定義 ===== */

/* 輸入緩衝區與解析結果 */
static char serial_input[20];           /* 序號輸入緩衝區 */
static unsigned int serial_part1;       /* 序號第一部分 (dash前) - 無符號 */
static unsigned long serial_part2;      /* 序號第二部分 (dash後) - 無符號 */
static char dash_char;                  /* 分隔符 */
static unsigned char error_flag = 0;    /* 錯誤旗標 */
static unsigned char validation_flag = 0; /* 驗證旗標 */

/* 中間計算值 */
static unsigned long calc_val1;         /* 計算值1 */
static unsigned long calc_val2;         /* 計算值2 */
static unsigned long calc_val3;         /* 計算值3 */
static unsigned long calc_val4;         /* 計算值4 */

/* 常數 */
static unsigned long constant1 = 0x6AA602B0UL;  /* 初始常數 */
static unsigned long enable_flag = 1;           /* 啟用旗標 */

/* ===== 函數宣告 ===== */
static void init_system(void);
static unsigned long generate_auth_code(void);
static void get_serial_input(void);
static void parse_serial(void);
static void calculate_intermediates(void);

/* ===== 主程式 ===== */
int main(void)
{
    unsigned long auth_code;
    
    printf("Enter serial no: ");
    
    /* 取得並解析序號 */
    get_serial_input();
    parse_serial();
    
    /* 計算中間值 */
    calculate_intermediates();
    
    /* 初始化系統 */
    init_system();
    
    /* 生成授權碼 */
    auth_code = generate_auth_code();
    
    return 0;
}

/* 
 * 取得序號輸入
 * 對應 ads_getvar 函數
 */
static void get_serial_input(void)
{
    scanf("%s", serial_input);
}

/*
 * 解析序號
 * 對應 sub_3A7B4 函數
 * 格式: XXXX-YYYYYYYY (例如: 1234-10000000)
 */
static void parse_serial(void)
{
    int result;
    
    /* 解析格式 "%d-%ld" */
    result = sscanf(serial_input, "%d-%ld", &serial_part1, &serial_part2);
    
    /* 檢查解析結果 */
    if (result != 2) {
        error_flag = 1;
        return;
    }
    
    /* 檢查分隔符 (應為 '-') */
    if (strchr(serial_input, '-') == NULL) {
        validation_flag |= 0x10;  /* 設定驗證失敗旗標 */
    }
    
    /* 錯誤處理 */
    if ((validation_flag & 0x10) == 0 && error_flag != 0) {
        printf("Invalid serial format\n");
        exit(1);
    }
    
    if ((validation_flag & 0x10) != 0) {
        printf("Invalid serial format\n");
        exit(1);
    }
}

/*
 * 計算中間值
 * 對應 main 函數中的計算部分
 * 
 * 算法說明:
 * 1. 將 serial_part2 除以 10000 得到 calc_val1
 * 2. 將 serial_part2 模 10000 得到 calc_val2
 * 3. 對 serial_part1 進行複雜運算得到 calc_val3 和 calc_val4
 */
static void calculate_intermediates(void)
{
    unsigned long temp1, temp2;
    long quotient, remainder;
    
    /* === 計算 calc_val1 和 calc_val2 === */
    /* serial_part2 / 10000 (0x2710) */
    quotient = (long)serial_part2 / 10000L;
    calc_val1 = (unsigned long)quotient;
    
    /* serial_part2 % 10000 */
    remainder = (long)serial_part2 % 10000L;
    calc_val2 = (unsigned long)remainder;
    
    /* === 計算 calc_val3 === */
    /* temp1 = (serial_part1 & 0xFFF) % 100 */
    temp1 = (serial_part1 & 0x0FFFUL);
    temp1 = temp1 % 100UL;
    
    /* temp1 = temp1 * 25 (使用 LEA 指令的模擬: [edx+edx*4]*5) */
    /* LEA edx, [edx+edx*4] => edx = edx * 5 */
    /* LEA edx, [edx+edx*4] => edx = edx * 5 */
    /* LEA edx, [edx*4] => edx = edx * 4 + 某值,但此處為0 */
    /* 實際上: temp1 = temp1 * 5 * 5 = temp1 * 25 */
    temp1 = (temp1 + temp1 * 4);  /* temp1 * 5 */
    temp1 = (temp1 + temp1 * 4);  /* temp1 * 5 */
    temp1 = temp1 * 4;            /* temp1 * 4 */
    
    /* temp2 = calc_val1 % 100 */
    temp2 = calc_val1 % 100UL;
    
    /* calc_val3 = temp1 + temp2 */
    calc_val3 = temp1 + temp2;
    
    /* === 計算 calc_val4 === */
    /* temp1 = (serial_part1 & 0xFFF) / 100 */
    temp1 = (serial_part1 & 0x0FFFUL);
    temp1 = temp1 / 100UL;
    
    /* temp1 = temp1 * 25 (同上) */
    temp1 = (temp1 + temp1 * 4);  /* temp1 * 5 */
    temp1 = (temp1 + temp1 * 4);  /* temp1 * 5 */
    temp1 = temp1 * 4;            /* temp1 * 4 */
    
    /* temp2 = calc_val1 / 100 */
    temp2 = calc_val1 / 100UL;
    
    /* calc_val4 = temp1 + temp2 */
    calc_val4 = temp1 + temp2;
}

/*
 * 初始化系統
 * 對應 sub_3A1D4 -> sub_3A3E8
 */
static void init_system(void)
{
    enable_flag = 1;
}

/*
 * 生成授權碼
 * 對應 sub_3A698 函數
 * 
 * 這是核心算法,使用複雜的位運算生成授權碼
 * 
 * 算法步驟:
 * 1. 取得 calc_val2,除以 256,異或 0x41
 * 2. 加上常數 constant1,進行多次移位和加法
 * 3. 取得 calc_val3,除以 256,異或 0x4D
 * 4. 結合前面結果,進行更多位運算
 * 5. 根據 validation_flag 決定是否執行額外運算
 * 6. 最後組合成授權碼
 */
static unsigned long generate_auth_code(void)
{
    unsigned long result = 0;
    unsigned long temp1, temp2, temp3, temp4;
    long signed_temp;
    unsigned long quotient, remainder;
    unsigned char byte_val;
    union {
        unsigned long ul;
        long sl;
    } conv;  /* 用於有符號/無符號轉換 */
    
    /* ESI = 0, EBX = 0 初始化 */
    temp1 = 0;  /* esi */
    temp2 = 0;  /* ebx */
    
    /* === 第一部分:處理 calc_val2 === */
    /* EAX = calc_val2 */
    conv.ul = calc_val2;
    
    /* CDQ: 符號擴展 EAX 到 EDX:EAX */
    /* 由於我們知道這是無符號值,但組語使用 IDIV (有符號除法) */
    signed_temp = (long)conv.ul;
    
    /* IDIV 256: 有符號除法 */
    quotient = (unsigned long)((long)conv.ul / 256L);
    remainder = (unsigned long)((long)conv.ul % 256L);
    
    /* XCHG EAX, EDI => EDI = quotient */
    temp3 = quotient;  /* edi */
    
    /* XOR DI, 41h: 對低 16 位元異或 0x41 */
    temp3 = (temp3 & 0xFFFF0000UL) | ((temp3 & 0xFFFFUL) ^ 0x0041UL);
    
    /* ADD EDI, constant1 */
    temp3 = temp3 + constant1;
    
    /* var_4 = EDI */
    temp4 = temp3;  /* var_4 */
    
    /* SHL EDI, 17h: 左移 23 位元 */
    temp3 = temp3 << 23;
    
    /* EAX = var_4; SHL EAX, 0Fh: 左移 15 位元 */
    result = temp4 << 15;
    
    /* ADD EAX, EDI */
    result = result + temp3;
    
    /* var_8 = EAX */
    temp3 = result;  /* var_8, 重用 temp3 */
    
    /* === 第二部分:處理 calc_val3 === */
    /* EAX = calc_val3 */
    conv.ul = calc_val3;
    
    /* var_C = EDX (remainder from previous division) */
    /* 但這裡 remainder 被保存下來 */
    
    /* CDQ; IDIV 256 */
    quotient = (unsigned long)((long)conv.ul / 256L);
    remainder = (unsigned long)((long)conv.ul % 256L);
    
    /* XOR AL, 4Dh: 對最低位元組異或 0x4D */
    byte_val = (unsigned char)(quotient & 0xFFUL);
    byte_val ^= 0x4D;
    quotient = (quotient & 0xFFFFFF00UL) | (unsigned long)byte_val;
    
    /* ADD EAX, var_8 */
    quotient = quotient + temp3;
    
    /* INC EAX */
    quotient = quotient + 1UL;
    
    /* EDI = var_4; ADD EDI, EAX */
    result = temp4 + quotient;
    
    /* var_10 = EDI */
    temp3 = result;  /* var_10 */
    
    /* SAR EDI, 10h: 算術右移 16 位元(有符號) */
    conv.ul = result;
    conv.sl = conv.sl >> 16;  /* 有符號右移 */
    result = conv.ul;
    
    /* ADD EDI, var_10 */
    result = result + temp3;
    
    /* EAX = var_10; ADD EAX, EDI */
    temp3 = temp3 + result;
    
    /* XCHG EAX, EDI => EDI = temp3, EAX = result */
    temp4 = result;
    result = temp3;
    temp3 = temp4;
    
    /* IMUL EAX, ESI, 75BCD15h: EAX = 0 * 0x75BCD15 = 0 */
    temp4 = temp1 * 0x75BCD15UL;  /* temp1 (esi) = 0 */
    
    /* XOR EAX, EDI */
    temp4 = temp4 ^ temp3;
    
    /* XOR AX, 0ACADh: 對低 16 位元異或 */
    temp4 = (temp4 & 0xFFFF0000UL) | ((temp4 & 0xFFFFUL) ^ 0xACADUL);
    
    /* EDI = var_C (remainder from calc_val3 division) */
    /* 這裡需要取回之前保存的 remainder */
    /* 根據組語,var_C 保存的是第一次除法的 remainder */
    temp3 = remainder;  /* 這應該是第一個除法的 remainder */
    
    /* XOR DI, 32h */
    temp3 = (temp3 & 0xFFFF0000UL) | ((temp3 & 0xFFFFUL) ^ 0x0032UL);
    
    /* ADD EDI, EAX */
    result = temp3 + temp4;
    
    /* === 第三部分:條件處理 === */
    /* TEST byte ptr [validation_flag], 10h */
    if ((validation_flag & 0x10) != 0) {
        /* var_14 = EDI */
        temp3 = result;
        
        /* SHL EDI, 0Ah: 左移 10 位元 */
        result = result << 10;
        
        /* EAX = calc_val4 */
        conv.ul = calc_val4;
        
        /* CDQ; IDIV 256 */
        quotient = (unsigned long)((long)conv.ul / 256L);
        remainder = (unsigned long)((long)conv.ul % 256L);
        
        /* XOR EAX, EDX */
        quotient = quotient ^ remainder;
        
        /* XOR AL, 0B1h */
        byte_val = (unsigned char)(quotient & 0xFFUL);
        byte_val ^= 0xB1;
        quotient = (quotient & 0xFFFFFF00UL) | (unsigned long)byte_val;
        
        /* ADD EAX, EDI */
        quotient = quotient + result;
        
        /* EDI = var_14; ADD EDI, EAX */
        result = temp3 + quotient;
    }
    
    /* === 第四部分:最終處理 === */
    /* var_4 = EDI */
    temp3 = result;
    
    /* SAR EDI, 1: 算術右移 1 位元 */
    conv.ul = result;
    conv.sl = conv.sl >> 1;  /* 有符號右移 */
    result = conv.ul;
    
    /* ADD EDI, var_4 */
    result = result + temp3;
    
    /* EAX = calc_val3 */
    conv.ul = calc_val3;
    
    /* EDX = EAX */
    temp3 = conv.ul;
    
    /* MOVZX EAX, AL: 零擴展最低位元組 */
    temp4 = conv.ul & 0xFFUL;
    
    /* AND EDX, EDX: 測試是否為負 */
    /* JGE: 如果 >= 0 則跳過 */
    if ((long)temp3 < 0) {
        /* AND EAX, EAX: 測試是否為 0 */
        if (temp4 != 0) {
            /* SUB EAX, 100h */
            temp4 = temp4 - 0x100UL;
        }
    }
    
    /* XOR AL, 0D2h */
    byte_val = (unsigned char)(temp4 & 0xFFUL);
    byte_val ^= 0xD2;
    temp4 = (temp4 & 0xFFFFFF00UL) | (unsigned long)byte_val;
    
    /* ADD EAX, EDI */
    result = temp4 + result;
    
    /* EDI = ESI; SHL EDI, 10h */
    temp3 = temp1 << 16;  /* temp1 (esi) = 0 */
    
    /* XOR EDI, EAX */
    result = temp3 ^ result;
    
    /* === 輸出結果 === */
    printf("The authorization code is %08.0lX.\n", result);
    
    /* 終止程式 (DOS interrupt) */
    {
        union REGS regs;
        regs.h.ah = 0x4C;
        regs.h.al = 0x01;
        int86(0x21, &regs, &regs);
    }
    
    return result;
}
