bitmap:
    .res 8     ; 64 bits

contains:
        tay             ; Y = x
        lsr             ; A = x / 2
        lsr             ; A = x / 4
        lsr             ; A = x / 8  → byte index
        tax             ; X = byte index

        tya             ; restore x
        and #7          ; bit = x % 8
        tay             ; Y = bit index

        lda bitmap,x    ; load byte
        lsr a           ; shift right Y times
contains_shift:
        dey
        bmi contains_check
        lsr a
        bne contains_shift

contains_check:
        and #1          ; isolate lowest bit
        rts

insert:
        tay
        lsr
        lsr
        lsr
        tax             ; X = byte index

        tya
        and #7
        tay             ; Y = bit index

        lda #1
insert_shift:
        dey
        bmi insert_do
        asl a
        bne insert_shift

insert_do:
        ora bitmap,x
        sta bitmap,x
        rts

remove:
        tay
        lsr
        lsr
        lsr
        tax             ; X = byte index

        tya
        and #7
        tay             ; Y = bit index

        lda #1
remove_shift:
        dey
        bmi remove_do
        asl a
        bne remove_shift

remove_do:
        eor #$FF        ; invert mask
        and bitmap,x
        sta bitmap,x
        rts
