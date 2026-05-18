memset:
        tay             ; copy A to Y 
        ldy #0          ;

ms_loop:
        cpx #0          ; is X == 0
        beq ms_done     ; 

        tya             ;
        sta (ptr),y     ; store A at ptr + Y

        iny             ; next byte
        dex             ;
        bne ms_loop     ;if X != 0 -> loop again

ms_done:
        rts             ;
