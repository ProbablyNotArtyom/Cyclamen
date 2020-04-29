
	.include        "zeropage.inc"
	.include        "c64.inc"

	.import mmusave
	.export _platform_init

COLOR_BLACK             = $00
COLOR_WHITE             = $01
COLOR_RED               = $02
COLOR_CYAN              = $03
COLOR_VIOLET            = $04
COLOR_GREEN             = $05
COLOR_BLUE              = $06
COLOR_YELLOW            = $07
COLOR_ORANGE            = $08
COLOR_BROWN             = $09
COLOR_LIGHTRED          = $0A
COLOR_GRAY1             = $0B
COLOR_GRAY2             = $0C
COLOR_LIGHTGREEN        = $0D
COLOR_LIGHTBLUE         = $0E
COLOR_GRAY3             = $0F

.segment "STARTUP"

_platform_init:

    lda $01				; Switch off the BASIC ROM.
    sta mmusave         ; Save the memory configuration
    and #$F8
    ora #$06            ; Enable Kernal+I/O, disable BASIC
    sta $01

	lda	#$9B			; Enable the VIC as soon as possible to get something on the screen
	sta $D011
	lda #$37
	sta $D012
	lda #$08
	sta $D016			; Setup our the VIC banks
	lda #%00010111
	sta $D018

	lda #COLOR_LIGHTBLUE
	sta $d020
	lda #COLOR_BLUE
	sta $d021

	ldx #$FF			; Clear the screen
	lda #' '
loop:
	inx
	sta $400,x
	sta $500,x
	sta $600,x
	sta $700,x
	cpx #$FF
	bne loop

	ldx #$FF			; Clear the screen
	lda #COLOR_LIGHTBLUE
cram:
	inx
	sta $d800,x
	sta $d900,x
	sta $da00,x
	sta $db00,x
	cpx #$FF
	bne cram

	rts
