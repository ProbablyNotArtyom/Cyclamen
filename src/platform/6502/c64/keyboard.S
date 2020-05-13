
.end

.define PORTB $DC00				; VIA A
.define PORTA $DC01
.define DDRB  $DC02
.define DDRA  $DC03
.define T1CL  $DC04
.define T1CH  $DC05
.define T1LL  $DC06
.define T1LH  $DC07
.define T2LL  $DC08
.define T2CL  $DC08
.define T2CH  $DC09
.define SHIFT $DC0A
.define ACR   $DC0B
.define PCR   $DC0C
.define IFR   $DC0D
.define IER   $DC0E
.define ORAX  $DC0F

.define Scanresult 	$F0
.define lastkey		$F1
.define actkey 		$F2

.global _GETKey

_GETKey:     					; PORTBB = ROWS     PORTAB = COLUMNS
			lda #$FF
			sta DDRB
			sta PORTB
			lda #$00
			sta DDRB
			lda #$FF
			sta DDRA
			lda #%11111110
		    sta PORTA
		    lda PORTB
		    sta Scanresult
		    lda #%11111101
		    sta PORTA
		    lda PORTB
		    sta Scanresult+1
   			lda #%11111011
    		sta PORTA
    		ldy PORTB
    		sty Scanresult+2
    		lda #%11110111
    		sta PORTA
    		ldy PORTB
    		sty Scanresult+3
    		lda #%11101111
    		sta PORTA
    		ldy PORTB
    		sty Scanresult+4
    		lda #%11011111
    		sta PORTA
    		ldy PORTB
    		sty Scanresult+5
    		lda #%10111111
    		sta PORTA
    		lda PORTB
    		sta Scanresult+6
    		lda #%01111111
    		sta PORTA
    		ldy PORTB
    		sty Scanresult+7

    		ldx #$FF
:    		inx
    		lda Scanresult,x
    		cpx #$08
    		beq LE
    		cmp #$FF
    		beq :-
    		pha
    		txa
    		pha
    		lda #$FF
			sta DDRB
			sta PORTB
			lda #$00
			sta DDRB
    		lda #%11111101		; Scan for left shift
    		sta PORTA
    		lda PORTB
    		and #%01000000
    		cmp #$00
    		bne :+
    		pla
    		tax
    		pla
    		jmp SKB1
:			lda #$FF
			sta DDRB
			sta PORTB
			lda #$00
			sta DDRB
			lda #%10111111		; scan for right shift
			sta PORTA
			lda PORTB
			and #%00010000
			cmp #$00
			bne :+
			pla
			tax
			pla
			jmp SKB1
:			pla					; Evaluate keyboard without shift
			tax
			pla
			jmp KB1

NJMP:
			pla
			tax
			pla
			jmp KB1
SJMP:
			pla
			tax
			pla
			jmp SKB1

LE:			lda #$FF			; Set lastkey to #$FF to indicate that there was no key press
			tax
			rts
								; X has the Row number	(Binary)
KB1:    	eor #$FF 			; A has the Column number (Linear)
    		jsr LtoB			; Convert A to Binary

    		cmp #$FF
    		beq LE

    		cpx #$00
    		bne :+
    		tax
    		lda B0,x
    		jmp KDN

:    		cpx #$01
    		bne :+
    		tax
    		lda B1,x
    		jmp KDN

:    		cpx #$02
    		bne :+
    		tax
    		lda B2,x
    		jmp KDN

:    		cpx #$03
    		bne :+
    		tax
    		lda B3,x
    		jmp KDN

:    		cpx #$04
    		bne :+
    		tax
    		lda B4,x
    		jmp KDN

:    		cpx #$05
    		bne :+
    		tax
    		lda B5,x
    		jmp KDN

:    		cpx #$06
    		bne :+
    		tax
    		lda B6,x
    		jmp KDN

:    		cpx #$07
			bne :+
			tax
    		lda B7,x
    		jmp KDN

:			jmp LE

			ldx #$FF
			rts

								; X has the Row number	(Binary)
SKB1:    	eor #$FF 			; A has the Column number (Linear)
    		jsr LtoB			; Convert A to Binary

    		cmp #$FF
    		beq LE

    		cpx #$00
    		bne :+
    		tax
    		lda SB0,x
    		jmp KDN

:    		cpx #$01
    		bne :+
    		tax
    		lda SB1,x
    		jmp KDN

:    		cpx #$02
    		bne :+
    		tax
    		lda SB2,x
    		jmp KDN

:    		cpx #$03
    		bne :+
    		tax
    		lda SB3,x
    		jmp KDN

:    		cpx #$04
    		bne :+
    		tax
    		lda SB4,x
    		jmp KDN

:    		cpx #$05
    		bne :+
    		tax
    		lda SB5,x
    		jmp KDN

:    		cpx #$06
    		bne :+
    		tax
    		lda SB6,x
    		jmp KDN

:    		cpx #$07
			bne :+
			tax
    		lda SB7,x
    		jmp KDN

:			jmp LE

			ldx #$FF
			rts

KDN:
			tax
			rts

LtoB:
			cmp #%00000001
			bne :+
			lda #$00
			rts
:			cmp #%00000010
			bne :+
			lda #$01
			rts
:			cmp #%00000100
			bne :+
			lda #$02
			rts
:			cmp #%00001000
			bne :+
			lda #$03
			rts
:			cmp #%00010000
			bne :+
			lda #$04
			rts
:			cmp #%00100000
			bne :+
			lda #$05
			rts
:			cmp #%01000000
			bne :+
			lda #$06
			rts
:			cmp #%10000000
			bne :+
			lda #$07
			rts
:								; No key was pressed, Return FF
			ldx #$FF
			rts


BTBL:   	;    0    1    2    3    4    5    6    7S
B0:			.byte $08, $33, $35, $37, $39, $2B, $1C, $31 ;0
B1:			.byte $0a, $57, $52, $59, $49, $50, $2A, $08 ;1
B2:			.byte $1D, $41, $44, $47, $4A, $4C, $3B, $FF ;2
B3:			.byte $88, $34, $36, $38, $30, $2D, $13, $32 ;3
B4:			.byte $85, $5A, $43, $42, $4D, $2E, $FF, $20 ;4
B5:			.byte $86, $53, $46, $48, $4B, $3A, $3D, $FF ;5
B6:			.byte $87, $45, $54, $55, $4F, $00, $1E, $51 ;6
B7:			.byte $11, $FF, $58, $56, $4E, $2C, $2F, $03 ;7

SBTBL:   	;    0    1    2    3    4    5    6    7
SB0:		.byte $08, $33, $35, $37, $39, $2B, $1C, $31 ;0
SB1:		.byte $0a, $57, $52, $59, $49, $50, $2A, $08 ;1
SB2:		.byte $1D, $41, $44, $47, $4A, $4C, $3B, $FF ;2
SB3:		.byte $88, $34, $36, $38, $30, $2D, $13, $32 ;3
SB4:		.byte $85, $5A, $43, $42, $4D, $2E, $FF, $20 ;4
SB5:		.byte $86, $53, $46, $48, $4B, $3A, $3D, $FF ;5
SB6:		.byte $87, $45, $54, $55, $4F, $00, $1E, $51 ;6
SB7:		.byte $11, $FF, $58, $56, $4E, $2C, $2F, $03 ;7

CRTbl:
			.byte $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E, $0E
			.byte $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C, $1C
			.byte $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A, $2A
			.byte $38, $38, $38, $38, $38, $38, $38, $38, $38, $38, $38, $38, $38, $38
			.byte $46, $46, $46, $46, $46, $46, $46, $46, $46, $46, $46, $46, $46, $46
