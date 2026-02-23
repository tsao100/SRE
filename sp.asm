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
mov     eax, [edi - C]
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
mov     eax, [eax - D]
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
mov     eax, [eax - C]
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
sub     eax, part2; 'i'
push    eax
mov     eax, off_DECC
sub     eax, part1
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
mov     eax, [eax-part2]
cdq
mov     ecx, 2710h
idiv    ecx
mov     edx, off_E32C
mov     [edx-Q1], eax
mov     eax, off_DED0
mov     eax, [eax-part2]
cdq
idiv    ecx
mov     eax, off_E330
mov     [eax-33h], edx
mov     eax, off_DECC
mov     eax, part1 ;part1=117d=75h
and     part1, 0FFFh
cdq
mov     ecx, 64h ; 'd'
idiv    ecx ;-->edx=11d
lea     edx, [edx+edx*4]
lea     edx, [edx+edx*4]
lea     edx, ds:0[edx*4]
mov     eax, off_E32C
mov     eax, [eax-Q1]
mov     [ebp+var_18], edx=11h*64h
cdq
idiv    ecx
add     edx=Q1R, [ebp+var_18]
mov     eax, off_E334
mov     [eax-C], edx=6A9
mov     eax, off_DECC
mov     eax, [eax-part1]
and     eax, 0FFFh
cdq
idiv    ecx
lea     eax, [eax+eax*4]
lea     eax, [eax+eax*4]
lea     eax, ds:0[eax*4]
mov     edx, off_E32C
mov     [ebp+var_1C], eax=0x64
mov     eax, [edx-Q1]
cdq
idiv    ecx
add     eax, [ebp+var_1C]
mov     edx, off_E338
mov     [edx-D], eax=0x6E
call    sub_3A1D4
main endp