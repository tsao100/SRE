/*
 *  serial_auth.c
 *  序列號 → 授權碼產生器
 *  Build: MSC 6.0  (cl /AS /Ox serial_auth.c)
 *         16-bit DOS COM/EXE，int=16bit，long=32bit
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*------------------------------------------------------------------
 * 計算授權碼
 *   A    : 序列號後半段 (long)
 *   B    : 序列號前半段 & 0x0FFF (long)
 *   flag : serial[2] != '-' 時為 1（決定額外運算路徑）
 *------------------------------------------------------------------*/
static long calc_auth(long A, long B, int flag)
{
    long E36C, E370, E374, E378;
    long var_4, var_8, var_C, var_10, var_14;
    long edi, eax;

    /*-- Step 0: 中間值 ------------------------------------------*/
    E36C = A / 10000L;
    E370 = A % 10000L;
    E374 = (E36C % 100L) + (B % 100L) * 100L;
    E378 = (E36C / 100L) + (B / 100L) * 100L;

    /*-- Step 1: 處理 E370 (A%10000) ----------------------------
     *  asm: idiv 256 → edi=商, edx=餘; xor di,41h; edi+=2
     *  因 E370<10000，商<40，高 16 位元為 0，
     *  故 xor di 等同 xor edi（低 16 位即全部）
     *-----------------------------------------------------------*/
    edi   = E370 / 256L;
    var_C = E370 % 256L;          /* 儲存餘數供 Step 6 使用 */
    edi  ^= 0x41L;
    edi  += 2L;                   /* dword_DEB8 = 2 */
    var_4 = edi;

    /*-- Step 2: var_8 = var_4 << 23 + var_4 << 15 -------------
     *  asm: shl edi,17h ; shl eax,0Fh ; add eax,edi
     *  = var_4 * (2^23 + 2^15) = var_4 * 8421376
     *-----------------------------------------------------------*/
    var_8 = (var_4 << 23L) + (var_4 << 15L);

    /*-- Step 3: 處理 E374 --------------------------------------
     *  asm: idiv 256 → 商<256; xor al,4Dh; +var_8+1
     *-----------------------------------------------------------*/
    eax    = E374 / 256L;         /* 商 < 256，xor al = xor eax */
    eax   ^= 0x4DL;
    eax   += var_8 + 1L;
    edi    = var_4 + eax;
    var_10 = edi;

    /*-- Step 4: 合成 var_10 的高/低字 -------------------------
     *  asm: sar edi,10h ; add edi,[var_10]
     *       mov eax,[var_10] ; add eax,edi
     *       xchg eax,edi
     *  → edi = 2*var_10 + (var_10>>16)
     *-----------------------------------------------------------*/
    edi = (var_10 >> 16L) + var_10;
    eax = var_10 + edi;           /* = 2*var_10 + (var_10>>16) */
    edi = eax;

    /*-- Step 5: imul esi(=0)→0; xor eax,edi; xor ax,0xACAD --
     *  esi 恆為 0，所以 imul 結果為 0
     *  xor ax 僅影響低 16 位
     *-----------------------------------------------------------*/
    eax = edi;                    /* 0 ^ edi = edi */
    eax = (eax & 0xFFFF0000L) | ((eax & 0x0000FFFFL) ^ 0xACADL);

    /*-- Step 6: xor var_C(E370%256) 與 0x32，加 eax ----------
     *  asm: mov edi,[var_C] ; xor di,32h ; add edi,eax
     *-----------------------------------------------------------*/
    edi  = var_C ^ 0x32L;
    edi += eax;

    /*-- Step 7 (選用): serial[2]!='-' 時的額外運算 -----------
     *  asm: shl edi,0Ah; idiv 256; xor eax,edx; xor al,0B1h
     *-----------------------------------------------------------*/
    if (flag) {
        var_14 = edi;
        edi  <<= 10L;

        eax  = (E378 / 256L) ^ (E378 % 256L);
        eax  = (eax & 0xFFFFFF00L) | ((eax & 0xFFL) ^ 0xB1L);
        eax += edi;
        edi  = var_14 + eax;
    }

    /*-- Step 8: edi = edi + (edi>>1) --------------------------
     *  asm: mov [var_4],edi ; sar edi,1 ; add edi,[var_4]
     *  = X + floor(X/2)，MSC 6.0 signed long >> 為算術右移
     *-----------------------------------------------------------*/
    var_4 = edi;
    edi >>= 1;                    /* 算術右移（保號） */
    edi  += var_4;

    /*-- Step 9: 取 E374 有號低位元組 XOR 0xD2，加 edi --------
     *  asm: movzx eax,al ; (若E374<0且低位≠0: sub eax,100h)
     *       xor al,0D2h ; add eax,edi
     *  = sign_extend_byte(E374 & 0xFF) ^ 0xD2，再 + edi
     *-----------------------------------------------------------*/
    {
        long low = E374 & 0xFFL;
        if (E374 < 0L && low != 0L)
            low -= 0x100L;        /* 有號低位元組的符號延伸 */
        eax = low;
        eax = (eax & 0xFFFFFF00L) | ((eax & 0xFFL) ^ 0xD2L);
    }
    eax += edi;

    /*-- Step 10: esi=0 → edi=0<<16 ^ eax = eax (授權碼) ------*/
    return eax;
}

/*==================================================================*/
int main(void)
{
    char  serial[20];
    int   part1;              /* %d  : 前半段，16-bit int (MSC 6.0) */
    long  part2;              /* %ld : 後半段，32-bit long           */
    int   n, flag;
    long  A, B, auth;

    /*-- 提示並讀入序列號 ----------------------------------------*/
    printf("Enter serial no: ");
    scanf("%s", serial);

    /*-- 解析 "NNNNN-MMMMMM" 格式 -------------------------------*/
    n = sscanf(serial, "%d-%ld", &part1, &part2);
    if (n != 2) {
        exit(1);
    }

    /*-- 判斷分隔符位置 -----------------------------------------
     *  serial[2] == '-' → 前半段為 2 位數 → 不走額外路徑
     *  serial[2] != '-' → 前半段 ≥ 3 位數 → 走額外路徑
     *-----------------------------------------------------------*/
    flag = (serial[2] != '-') ? 1 : 0;

    B    = (long)((unsigned int)part1 & 0x0FFFU); /* 12 位元遮罩 */
    A    = part2;

    /*-- 產生授權碼並輸出 ----------------------------------------*/
    auth = calc_auth(A, B, flag);

    printf("The authorization code is %08.0lX.\n", auth);

    return 0;
}
