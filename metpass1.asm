IDEAL
P386N
MODEL flat,C
DATASEG

char_order      db '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?- '

CODESEG

PUBLIC  ConvertPassword6bit8bit
PUBLIC  ConvertPassword8bit6bit
PUBLIC  ConvertPasswordASCBin
PUBLIC  ConvertPasswordBinASC
PUBLIC  DecodePassword
PUBLIC  EncodePassword
PUBLIC  CalcCheckSum

EXTRN   BINpassword:byte
EXTRN   ASCpassword:byte
EXTRN   ResultPassword:byte

PROC    ConvertPassword6bit8bit

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset BINpassword
mov     edi,offset ResultPassword
mov     cl,6
@@10:
mov     al,[byte ptr esi]
shl     al,2
mov     ah,[byte ptr esi+1]
shr     ah,4
or      al,ah
stosb

mov     al,[byte ptr esi+1]
shl     al,4
mov     ah,[byte ptr esi+2]
shr     ah,2
or      al,ah
stosb

mov     al,[byte ptr esi+2]
shl     al,6
or      al,[byte ptr esi+3]
stosb

add     esi,4
dec     cl
jnz     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    ConvertPassword6bit8bit

PROC    ConvertPassword8bit6bit

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset ResultPassword
mov     edi,offset BINpassword
mov     cl,6
@@10:
mov     al,[byte ptr esi]
shr     al,2
stosb

mov     al,[byte ptr esi]
and     al,00000011b
shl     al,4
mov     ah,[byte ptr esi+1]
shr     ah,4
or      al,ah
stosb

mov     al,[byte ptr esi+1]
and     al,00001111b
shl     al,2
mov     ah,[byte ptr esi+2]
shr     ah,6
or      al,ah
stosb

mov     al,[byte ptr esi+2]
and     al,00111111b
stosb

add     esi,3
dec     cl
jnz     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    ConvertPassword8bit6bit

PROC    DecodePassword

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset ResultPassword
mov     dl,[byte ptr esi + 16]
stc
@@10:
mov     al,[byte ptr esi + 15]
mov     ebx,0Fh
@@20:
rcl     [byte ptr esi + ebx],1
dec     ebx
jns     @@20
rcl     al,1
mov     [byte ptr esi + 15],al
dec     dl
jnz     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    DecodePassword

PROC    EncodePassword

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset ResultPassword
mov     dl,[byte ptr esi + 16]
stc
@@10:
mov     al,[byte ptr esi + 0]
mov     ebx,0
mov     cl,0Fh
@@20:
rcr     [byte ptr esi + ebx],1
inc     ebx
dec     cl
jns     @@20
rcr     al,1
mov     [byte ptr esi + 0],al
dec     dl
jnz     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    EncodePassword

PROC    ConvertPasswordASCBin

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset ASCpassword
mov     edi,offset BINpassword
mov     cl,24
@@10:
mov     al,[byte ptr esi]
xor     ebx,ebx
@@20:
inc     bl
cmp     bl,42h
jae     @@40
cmp     al,[char_order + ebx-1]
jnz     @@20
dec     bl
cmp     bl,40h
jnz     @@30
mov     bl,0FFh
@@30:
mov     [byte ptr edi],bl
inc     esi
inc     edi
dec     cl
jnz     @@10
mov     eax,1
jmp     @@99

@@40:
mov     eax,0

@@99:
pop     ebx
pop     edi
pop     esi
pop     ebp
ret

ENDP    ConvertPasswordASCBin

PROC    ConvertPasswordBinASC

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset BINpassword
mov     edi,offset ASCpassword
mov     cl,24
@@10:
movzx   ebx,[byte ptr esi]
mov     al,[char_order + ebx]
stosb
inc     esi
dec     cl
jnz     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    ConvertPasswordBinASC

PROC    CalcCheckSum

; returns: AL = checksum

push    ebp
push    esi
push    edi
push    ebx

mov     esi,offset ResultPassword
mov     ebx,16
xor     al,al
@@10:
add     al,[byte ptr esi + ebx]
dec     ebx
jns     @@10

pop     ebx
pop     edi
pop     esi
pop     ebp
ret
ENDP    CalcCheckSum

END
