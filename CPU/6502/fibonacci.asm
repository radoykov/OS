fib:
        cmp #0
        beq fib_zero

        cmp #1
        beq fib_one

        ldx #1          ; F(n-1)
        ldy #0          ; F(n-2)

fib_loop:
        tya
        clc
        adc #0
        adc x           ; A = F(n-1) + F(n-2)

        sty temp        ; save old F(n-2)
        stx y           ; new F(n-2) = old F(n-1)
        sta x           ; new F(n-1) = next

        dec n
        bne fib_loop

        lda x
        rts

fib_zero:
        lda #0
        rts

fib_one:
        lda #1
        rts

temp: .res 1
n:    .res 1
x:    .res 1
y:    .res 1
