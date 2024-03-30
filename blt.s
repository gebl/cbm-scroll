.export _Blt

.import _src
.import _dst

_Blt:
    LDY #0
    lda _src
    LDA _src,y
    STA _dst,y
    iny
    LDA _src,y
    STA _dst,y
    iny
    LDA _src,y
    STA _dst,y
    iny
    LDA _src,y
    STA _dst,y
    iny
    RTS