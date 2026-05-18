memcpy:
        ldy #0         

memcpy_loop:
        cpx #0          ; if (count == 0)
        beq memcpy_done ;

        lda (src),y     ; indexing
        sta (dst),y     ; indexing

        iny             ; ++
        dex             ; --
        bne memcpy_loop

memcpy_done:
        rts
