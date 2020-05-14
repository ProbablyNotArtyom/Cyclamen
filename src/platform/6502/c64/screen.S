;---------------------------------------------------
; Cyclamen - NotArtyom - 2020
;---------------------------------------------------

	.include        "zeropage.inc"
	.include        "c64.inc"

	.importzp ptr1, ptr2
	.export _plat_c64_scroll

.define scr_width 	40
.define scr_height 	25
.define scr_base 	>$400

;---------------------------------------------------

_plat_c64_scroll:

	; scroll the screen up, excluding the last line
	ldx #0
scrl3:
	clc
	lda row_lo,x
	sta ptr2
	adc #scr_width
	sta ptr1
	lda row_hi,x
	sta ptr2+1
	adc #0
	sta ptr1+1
	ldy #scr_width-1
scrl:
	lda (ptr1),y
	sta (ptr2),y
	dey
	bpl scrl
	inx
	cpx #24
	bne scrl3

	; erase the last line
	lda row_lo,x
	sta ptr1
	lda row_hi,x
	sta ptr1+1
	lda #' '
	ldy #scr_width-1
scrl2:
	sta (ptr1),y
	dey
	bpl scrl2
	rts

;---------------------------------------------------

; Precalculated tables containing pointers to the start of each screen line
row_lo: .byte <($400+0),   <($400+40),  <($400+80),  <($400+120), <($400+160)
		.byte <($400+200), <($400+240), <($400+280), <($400+320), <($400+360)
		.byte <($400+400), <($400+440), <($400+480), <($400+520), <($400+560)
		.byte <($400+600), <($400+640), <($400+680), <($400+720), <($400+760)
		.byte <($400+800), <($400+840), <($400+880), <($400+920), <($400+960)
row_hi: .byte >($400+0),   >($400+40),  >($400+80),  >($400+120), >($400+160)
		.byte >($400+200), >($400+240), >($400+280), >($400+320), >($400+360)
		.byte >($400+400), >($400+440), >($400+480), >($400+520), >($400+560)
		.byte >($400+600), >($400+640), >($400+680), >($400+720), >($400+760)
		.byte >($400+800), >($400+840), >($400+880), >($400+920), >($400+960)
