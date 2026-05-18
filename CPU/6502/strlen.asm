strlen:
        ldy #0         

strlen_loop:
        lda (src),y     
        beq strlen_done 

        iny             
        bne strlen_loop 

strlen_done:
        tya           
        rts
