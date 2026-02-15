include	dosx.ah

assume cs : _text
assume es : nothing, ss : nothing, ds : _text, fs : nothing, gs : nothing


; Segment type : Pure code
_text	segment	para public use32 'code'

aPkSer db 'Enter AutoCAD R12 serial no: ', 00H
aShowCode db	'The authorization code is %08.0lX.', 0AH, 00H
aS		db '%s',00H
aTemp   db 14h dup(0)
aDLD	db '%d-%ld',0h

dword_DEAC dd 6AA602B0h, 0
dword_DEB4 dd 1
dword_DEB8 dd 2
word_DEBC dw 1
word_DEBE dw 0
off_DEC0 dd (offset dword_DFC8)+4
off_DEC4 dd (offset dword_E090)+0C8h
dd offset dword_E154
off_DECC dd offset dword_DFA6 + 3
off_DED0 dd offset byte_DF51
byte_DED4 db 2 dup(31h)
byte_DED6 db 30h
db 2Dh
dd 30333031h
db 34h
byte_DEDD db 31h
db 31h
db 31h
byte_DEE0 db 0
align 4
off_DEE4 dd 0
dword_DEE8 dd 9D3A6Fh
dword_DEEC dd 0
dword_DEF0 dd 0
word_DEF4 dw 0
align 4
dd 9 dup(0)
word_DF1C dw 0
word_DF1E dw 0
dd 7 dup(0)
db 2 dup(0)
word_DF3E dw 0
dd 4 dup(0)
db 0
byte_DF51 db 3 dup(0)
align 8
dword_DF58 dd 0
dword_DF5C dd 0
dword_DF60 dd 0
dword_DF64 dd 1, 0
db 2 dup(0)
dword_DF6E dd 0
word_DF72 dw 1
dword_DF74 dd 0
dword_DF78 dd 0
unk_DF7C db    0
db    0
db    0
db    0
db 0Ah, 0
dword_DF82 dd 0
dword_DF86 dd 0
dword_DF8A dd 0
dword_DF8E dd 0
dword_DF92 dd 0
dword_DF96 dd 0
align 4
off_DF9C dd offset unk_DF7C
db 27h, 0
dword_DFA2 dd 0
dword_DFA6 dd 0
align 4
dd 4 dup(0)
off_DFBC dd offset off_DF9C
db 3Eh, 0
word_DFC2 dw 0
align 8
dword_DFC8 dd 0

dword_E090 dd 3 dup(0)
off_E09C dd 0E07Ch
db 8, 0
dword_E0A2 dd 0
align 4
dd 5 dup(0)
off_E0BC dd offset off_E09C
db 2, 0
dword_E0C2 dd 0
align 4
dd 5 dup(0)
dd offset off_E0BC
db 2 dup(0)
dword_E0E2 dd 0
align 4
dd 5 dup(0)
dword_E0FC dd 0
db 0Ah, 0
dword_E102 dd 0
dword_E106 dd 0
dword_E10A dd 0
dword_E10E dd 0
dword_E112 dd 0
dword_E116 dd 0
align 4
off_E11C dd offset dword_E0FC
db 8, 0
dword_E122 dd 0
align 4
dd 5 dup(0)
off_E13C dd offset off_E11C
db 2, 0
dword_E142 dd 0
align 4
dd 3 dup(0)
dword_E154 dd 2 dup(0)
dd offset off_E13C
db 2 dup(0)
dword_E162 dd 0
dw 0
dd 5 dup(0)
db    0
db    0
db    0
db    0
db 0Ah, 0
dword_E182 dd 0
dword_E186 dd 0
dword_E18A dd 0
dword_E18E dd 0
dword_E192 dd 0
dword_E196 dd 0
align 4
off_E19C dd 0
db 46h, 0
word_E1A2 dw 0
dd 6 dup(0)
off_E1BC dd offset off_E19C
db 2, 0
dword_E1C2 dd 0
align 4
dd 5 dup(0)
dd offset off_E1BC
db 2 dup(0)
dword_E1E2 dd 0
align 4
dd 0Dh dup(0)
dword_E21C dd 0
dword_E220 dd 0
dword_E224 dd 0
dd 0
dd 3FF00000h, 9 dup(0)
dd 3FF00000h, 9 dup(0)
dd 3FF00000h, 9 dup(0)
dd 3FF00000h, 0
dd 0BFF00000h, 9 dup(0)
dd 3FF00000h, 9 dup(0)
dd 3FF00000h, 9 dup(0)
dd 3FF00000h
off_E328 dd 0
off_E32C dd (offset off_E388)+1
off_E330 dd offset byte_E3A3
off_E334 dd (offset dbl_E3C0)+7
off_E338 dd offset byte_E3D9
off_E33C dd 0; "AME_SOL"
off_E340 dd 0; "AME_NIL"
off_E344 dd 0; "AME_FRZ"
word_E348 dw 0
align 10h
db 8Dh, 13h
dword_E352 dd 0
align 4
dd 5 dup(0)
dd 0
dd 0; ")\t%lf\t%lf"
dd 0
dd 0
dword_E37C dd 0
dword_E380 dd 0
off_E384 dd 01832D4h
off_E388 dd 0; "BOX (%lf, %lf, %lf)"
off_E38C dd  1809A4h
off_E390 dd 0; "ds_invoke"
off_E394 dd 0; "OBJ"
dword_E398 dd 0
dword_E39C dd 0
byte_E3A0 db 0
byte_E3A1 db 1
db 0
byte_E3A3 db 0
dword_E3A4 dd 0
dword_E3A8 dd 0
dword_E3AC dd 0
dbl_E3B0 dq 0.0
dbl_E3B8 dq 0.0
dbl_E3C0 dq 0.0
dbl_E3C8 dq 0.0
dword_E3D0 dd 7
word_E3D4 dw 0
align 4
db 0
byte_E3D9 dd 0


extrn	printf : near; C library routine
extrn	scanf : near
extrn	sscanf : near
extrn	strcpy : near

ads_getvar      proc near

arg_0 equ dword ptr  8
arg_4 equ dword ptr  12h

    push    ebp
    mov     ebp, esp
	lea		eax, aTemp
	push	eax
	mov		[ebp + arg_4], eax
	lea		eax, aS
	push	eax
	call    scanf
	add		esp,8
	mov		eax, 13ECh
	leave
	ret
ads_getvar   endp


sub_3A1D4 proc near

	call    sub_3A3E8
	call    sub_3A698
leave
ret
sub_3A1D4 endp

sub_3A3E8 proc near

	mov     dword_DEB4, 1
;	leave
	ret
sub_3A3E8 endp

sub_3A698 proc near

var_14 equ dword ptr - 14h
var_10 equ dword ptr - 10h
var_C equ dword ptr - 0Ch
var_8 equ dword ptr - 8
var_4 equ dword ptr - 4

push    ebp
mov     ebp, esp
sub     esp, 14h
push    ebx
push    esi
push    edi
sub     esi, esi
mov     ebx, esi
mov     eax, off_E330
mov     eax, [eax - 33h]
cdq
mov     ecx, 100h
idiv    ecx
xchg    eax, edi
xor di, 41h
mov     eax, off_DEC0
add     edi, [eax - 114h]
mov[ebp + var_4], edi
shl     edi, 17h
mov     eax, [ebp + var_4]
shl     eax, 0Fh
add     eax, edi
mov     edi, off_E334
mov[ebp + var_8], eax
mov     eax, [edi - 53h]
mov[ebp + var_C], edx
cdq
idiv    ecx
xor al, 4Dh
add     eax, [ebp + var_8]
inc     eax
mov     edi, [ebp + var_4]
add     edi, eax
mov[ebp + var_10], edi
sar     edi, 10h
add     edi, [ebp + var_10]
mov     eax, [ebp + var_10]
add     eax, edi
xchg    eax, edi
imul    eax, esi, 75BCD15h
xor eax, edi
xor ax, 0ACADh
mov     edi, [ebp + var_C]
xor di, 32h
add     edi, eax
mov     eax, off_DECC
test    byte ptr[eax - 0C4h], 10h
jz      short loc_3A740
mov[ebp + var_14], edi
shl     edi, 0Ah
mov     eax, off_E338
mov     eax, [eax - 61h]
cdq
idiv    ecx
xor eax, edx
xor al, 0B1h
add     eax, edi
mov     edi, [ebp + var_14]
add     edi, eax

loc_3A740:
mov[ebp + var_4], edi
sar     edi, 1
add     edi, [ebp + var_4]
mov     eax, off_E334
mov     eax, [eax - 53h]
mov     edx, eax
movzx   eax, al
and edx, edx
jge     short loc_3A762
and eax, eax
jz      short loc_3A762
sub     eax, 100h

loc_3A762:
xor al, 0D2h
add     eax, edi
mov     edi, esi
shl     edi, 10h
xor edi, eax
mov     eax, off_DEC4
mov[eax - 1E4h], edi

push	edi
lea		eax, aShowCode
push	eax
call	printf

mov		ax, 4c01h
int		21h


mov     eax, 1
pop     edi
pop     esi
pop     ebx
leave
ret
sub_3A698 endp

sub_3A7B4 proc near

var_20 equ byte ptr - 20h
var_1A equ dword ptr - 1Ah

push    ebp
mov     ebp, esp
sub     esp, 20h
lea     eax, [ebp + var_1A]
push    eax
call    ads_getvar
add     esp, 4
cmp     eax, 13ECh
jnz     short loc_3A7FF
push[ebp + var_1A]
push    offset byte_DED4
call    strcpy
add     esp, 8
mov     eax, [ebp + var_1A]
and eax, eax
jz      short loc_3A7FF
push    eax
; call    sub_1598A4
mov[ebp + var_1A], 0
add     esp, 4

loc_3A7FF:
mov     eax, off_DED0
sub     eax, 69h; 'i'
push    eax
mov     eax, off_DECC
sub     eax, 0C5h
push    eax
push    offset aDLD
push    offset byte_DED4
call    sscanf

loc_3A822:
add     esp, 10h     ;Has debug to this line
cmp     eax, 2
jz      short loc_3A833
mov     byte_DEDD, 1
jmp     short loc_3A848

loc_3A833:
cmp     byte_DED6, 2Dh; '-'
jz      short loc_3A848
mov     eax, off_DECC
or byte ptr[eax - 0C4h], 10h

loc_3A848:
mov     eax, off_DECC
test    byte ptr[eax - 0C4h], 10h
jnz     short loc_3A85F
cmp     byte_DEDD, 0
jnz     short loc_3A876

loc_3A85F:
mov     eax, off_DECC
test    byte ptr[eax - 0C4h], 10h
jz      short locret_3A890
cmp     byte_DEE0, 0
jz      short locret_3A890

loc_3A876:
mov     eax, off_E384
push    dword ptr[eax + 980h]
; call    ads_printf
push    1
; call    ads_exit
add     esp, 8

locret_3A890:
leave
ret
sub_3A7B4 endp


; int __cdecl main(int argc, const char** argv, const char** envp)
main proc near

var_18 equ dword ptr - 18h
var_1C equ dword ptr - 1Ch

push    ebp
mov     ebp, esp
sub     esp, 18h

lea		eax, aPkSer
push	eax
call    printf
add		esp, 4

call    sub_3A7B4
mov     eax, off_DED0
mov     eax, [eax-69h]
cdq
mov     ecx, 2710h
idiv    ecx
mov     edx, off_E32C
mov     [edx-1Dh], eax
mov     eax, off_DED0
mov     eax, [eax-69h]
cdq
idiv    ecx
mov     eax, off_E330
mov     [eax-33h], edx
mov     eax, off_DECC
mov     eax, [eax-0C5h]
and     eax, 0FFFh
cdq
mov     ecx, 64h ; 'd'
idiv    ecx
lea     edx, [edx+edx*4]
lea     edx, [edx+edx*4]
lea     edx, ds:0[edx*4]
mov     eax, off_E32C
mov     eax, [eax-1Dh]
mov     [ebp+var_18], edx
cdq
idiv    ecx
add     edx, [ebp+var_18]
mov     eax, off_E334
mov     [eax-53h], edx
mov     eax, off_DECC
mov     eax, [eax-0C5h]
and     eax, 0FFFh
cdq
idiv    ecx
lea     eax, [eax+eax*4]
lea     eax, [eax+eax*4]
lea     eax, ds:0[eax*4]
mov     edx, off_E32C
mov     [ebp+var_1C], eax
mov     eax, [edx-1Dh]
cdq
idiv    ecx
add     eax, [ebp+var_1C]
mov     edx, off_E338
mov     [edx-61h], eax
call    sub_3A1D4
main endp


public	_start_
_start_	proc	near
call    main
_start_           endp

_text ends


end _start_
