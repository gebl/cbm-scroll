
.include "c64.inc"
.include "zeropage.inc"
; Entry point for C function
.global _blt
.global _src
.global _dst

_src = ptr1
_dst = ptr2

_blt:
    ldy #0
    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny

    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    lda (ptr1),y
    sta (ptr2),y
    iny
    
    rts