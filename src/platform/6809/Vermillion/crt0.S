
_platform_init	export

	section code

_platform_init:

	ldb #$FF			; Clear the screen
	ldx #$400
	lda #' '
loop:
	incb
	sta $000,x
	sta $100,x
	sta $200,x
	sta $300,x
	cmpb #$FF
	bne loop

	ldb #$FF			; Clear the screen
	ldx #$D800
	lda #' '
cram:
	incb
	sta $000,x
	sta $100,x
	sta $200,x
	sta $300,x
	cmpb #$FF
	bne cram

	rts
