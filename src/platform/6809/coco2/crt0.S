
_platform_init	export

	section code

_platform_init:

	; Clear the screen
    ldx #$200
	ldb #96
    leau $400,pcr
@l:	stb ,u+
    leax -1,x
    bne @l

	rts
