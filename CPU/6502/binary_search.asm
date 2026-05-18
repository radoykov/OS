binary_search:
    sta target          ; save target
    lda #0
    sta low           
    stx high         

bs_loop:
    lda high
    sec
    sbc low             ; high - low
    beq not_found  

    lsr            
    clc
    adc low             ; mid = low + (high-low)/2
    tay   

    lda ($30),y    
    cmp target
    beq found

    bcc go_right        ; if A < target -> search right half

go_left:
    tya
    sta high            ; high = mid
    jmp bs_loop

go_right:
    iny
    sty low             
    jmp bs_loop

found:
    lda #1
    rts

not_found:
    lda #0
    rts

-------------invoking
lda #<array
sta $30
lda #>array
sta $31
ldx #10        ; 10 елемента
lda #25        ; търсим 25
jsr binary_search
