strcpy:
        ldy #0          

strcpy_loop:
        lda (src),y     
        sta (dst),y     
        beq strcpy_done 
        iny             ; index++
        bne strcpy_loop ; end loop

strcpy_done:
        rts
