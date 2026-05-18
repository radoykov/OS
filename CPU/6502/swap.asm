* = $8000

main:
    lda #<$0200
    sta $30
    lda #>$0200
    sta $31

    lda #$05
    sta $0200       ; first element
    lda #$03
    sta $0201       ; second element

swap:
    ldy #0          ; Y = 0 
    lda ($30),y     ; A = *($30)
    pha         

    iny           
    lda ($30),y   
    dey     
    sta ($30),y

    pla       
    iny     
    sta ($30),y     ; *($30+1) = first element

    rts
