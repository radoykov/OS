result: .res 2

multiply8:
        sta multiplicand     ; save A
        stx multiplier       ; save B

        lda #0
        sta result
        sta result+1

        ldx #8               ; 8 bits to process

mul_loop:
        lsr multiplier       ; shift right multiplier
        bcc skip_add         ; if bit0 == 0 -> skip

        clc
        lda result
        adc multiplicand
        sta result

        lda result+1
        adc #0
        sta result+1

skip_add:
        asl multiplicand     ; multiplicand <<= 1
        dex
        bne mul_loop

        rts

multiplicand: .res 1
multiplier:   .res 1
