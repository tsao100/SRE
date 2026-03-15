CLS
SCREEN 12

'---------------------------------------
' 測試向量資料 (模擬 KANDAT 讀出)
'---------------------------------------

DIM DATS(20)

'筆畫1
DATS(1) = 0 * 10000 + 5 * 100 + 5
DATS(2) = 1 * 10000 + 20 * 100 + 5
DATS(3) = 1 * 10000 + 20 * 100 + 20
DATS(4) = 1 * 10000 + 5 * 100 + 20
DATS(5) = 1 * 10000 + 5 * 100 + 5

'筆畫2
DATS(6) = 0 * 10000 + 10 * 100 + 10
DATS(7) = 1 * 10000 + 15 * 100 + 15

NDAT = 7

'---------------------------------------
' 繪圖起始位置
'---------------------------------------

IXS = 200
IYS = 200

'---------------------------------------
' 繪圖
'---------------------------------------

FOR J = 1 TO NDAT

IP = DATS(J)

IPEN = INT(IP / 10000) + 1
YKANO = (IP MOD 100) + IYS
XKANO = (IP MOD 10000) / 100 + IXS

IF J = 1 THEN
    PX = XKANO
    PY = YKANO
    PSET (PX, PY)
ELSE

    IF IPEN = 1 THEN
        PSET (XKANO, YKANO)
    ELSE
        LINE (PX, PY)-(XKANO, YKANO)
    END IF

    PX = XKANO
    PY = YKANO

END IF

NEXT J

PRINT "KANJI VECTOR TEST"
END