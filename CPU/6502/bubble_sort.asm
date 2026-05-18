bubble_sort:
    ldy #0
    sty $32          ; swap_flag = 0

    lda ($30),y      ; load first element
    tax         
    iny
    dex      

next_element:
    lda ($30),y   
    iny
    cmp ($30),y    
    bcc check_end   
    beq check_end    ; if equal, no swap

    pha              ; save arr[i]

    lda ($30),y  
    dey
    sta ($30),y   

    pla
    iny
    sta ($30),y  

    lda #$ff
    sta $32     

check_end:
    dex
    bne next_element ; continue inner loop

    bit $32
    bmi bubble_sort  ; if swap_flag != 0 -> another pass

    rts
