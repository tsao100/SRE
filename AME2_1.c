#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 序列號結構
typedef struct {
    int part1;      // 破折號前的數字
    long part2;     // 破折號後的數字
    int valid;      // 是否有效
} SerialNumber;

// 解析序列號（格式: XXXX-XXXXXX）
SerialNumber parse_serial(const char* input) {
    SerialNumber sn = {0, 0, 0};
    
    // 檢查是否包含破折號
    if (strchr(input, '-') == NULL) {
        printf("錯誤：序列號格式無效，必須包含破折號 (例如: 1234-567890)\n");
        return sn;
    }
    
    // 解析兩個數字
    if (sscanf(input, "%d-%ld", &sn.part1, &sn.part2) != 2) {
        printf("錯誤：無法解析序列號\n");
        return sn;
    }
    
    sn.valid = 1;
    return sn;
}

// 生成授權碼的核心算法
unsigned long generate_auth_code(SerialNumber sn) {
    // 預處理：拆分第一個數字
    int quotient1 = sn.part1 / 10000;      // 商
    int remainder1 = sn.part1 % 10000;     // 餘數
    
    // 預處理：處理第二個數字（取低12位）
    int part2_low12 = sn.part2 & 0xFFF;
    
    // 計算三個中間值
    int value_A = (remainder1 % 100) * 100 + (quotient1 % 100);
    int value_B = (part2_low12 / 100) * 100 + (quotient1 % 100);
    int value_C = (part2_low12 % 100) * 100 + (quotient1 % 100);
    
    // === 第一階段：基礎變換 ===
    unsigned long step1 = (remainder1 / 0x100) ^ 0x41;
    step1 += quotient1;
    
    unsigned long step2 = (step1 << 23) + (step1 << 15);
    
    // === 第二階段：混合運算 ===
    unsigned long step3 = (value_A / 0x100) ^ 0x4D;
    step3 = step3 + step2 + 1;
    
    unsigned long step4 = step1 + step3;
    
    // === 第三階段：符號擴展和組合 ===
    long signed_step4 = (long)step4;  // 轉換為有符號數
    signed_step4 = (signed_step4 >> 16) + step4;  // 算術右移
    unsigned long step5 = step4 + signed_step4;
    
    // === 第四階段：魔術數混淆 ===
    unsigned long step6 = 0x75BCD15 * 0;  // 原始代碼中 esi=0
    step6 ^= step5;
    step6 ^= 0xACAD;
    
    unsigned long step7 = (remainder1 / 0x100) % 0x100;
    step7 ^= 0x32;
    step7 += step6;
    
    // === 第五階段：條件分支（檢查是否有額外標誌）===
    // 原代碼中有個標誌位檢查，這裡簡化處理
    // 如果需要完整實現，可以添加條件判斷
    int has_special_flag = 0;  // 根據需要設置
    
    if (has_special_flag) {
        unsigned long temp = step7 << 10;
        unsigned long extra = (value_B / 0x100) ^ (value_B % 0x100);
        extra ^= 0xB1;
        step7 = step7 + temp + extra;
    }
    
    // === 第六階段：最終混合 ===
    long signed_step7 = (long)step7;
    signed_step7 = (signed_step7 >> 1) + step7;
    
    unsigned long step8 = (unsigned char)value_C ^ 0xD2;
    step8 += signed_step7;
    
    // 最終結果
    unsigned long auth_code = (0 << 16) ^ step8;
    
    return auth_code;
}

// 主程序
int main(void) {
    char input[64];
    
    printf("======================================\n");
    printf("   序列號授權碼生成器\n");
    printf("======================================\n");
    printf("請輸入序列號 (格式: XXXX-XXXXXX): ");
    
    if (scanf("%63s", input) != 1) {
        printf("輸入錯誤！\n");
        return 1;
    }
    
    // 解析序列號
    SerialNumber sn = parse_serial(input);
    if (!sn.valid) {
        return 1;
    }
    
    // 生成授權碼
    unsigned long auth_code = generate_auth_code(sn);
    
    // 輸出結果
    printf("\n授權碼: %08lX\n", auth_code);
    printf("======================================\n");
    
    return 0;
}
