; Put Krok.BIN at 1EA80 in Metroid.NES

.mem 8
.index 8
.base $C000
ror             ; 2
ror             ; 2
ror             ; 2
and     #$C0    ; 2
ora     $37     ; 3
sta     $37     ; 3
txa             ; 2
lsr             ; 2
lsr             ; 2
ora     $38     ; 3
sta     $38     ; 3
                ; = 26 cycles total
.end
