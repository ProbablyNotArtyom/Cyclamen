//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

#ifndef DEV_PLAT_C64
#define DEV_PLAT_C64

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>

//---------------------------------------------------

char GETKey(void);

//---------------------------------------------------

char last_keypress;
char dev_plat_c64_getc(void) {
	char ret;
	do {
		ret = GETKey();
		if (ret == 0xFF) last_keypress = 0xFF;
	} while (ret == last_keypress);
	last_keypress = ret;
	return ret;
}

char dev_plat_c64_peek(void) {
	return GETKey();
}

//---------------------------------------------------
// Scary awful inline assembler version of
// the c64 keyboard read routine

#define PORTB 	0xDC00
#define PORTA 	0xDC01
#define DDRB  	0xDC02
#define DDRA  	0xDC03
#define T1CL  	0xDC04
#define T1CH  	0xDC05
#define T1LL  	0xDC06
#define T1LH  	0xDC07
#define T2LL  	0xDC08
#define T2CL  	0xDC08
#define T2CH  	0xDC09
#define SHIFT 	0xDC0A
#define ACR   	0xDC0B
#define PCR   	0xDC0C
#define IFR   	0xDC0D
#define IER   	0xDC0E
#define ORAX  	0xDC0F

const uint8_t B0[8] = { 0x08, 0x33, 0x35, 0x37, 0x39, 0x2B, 0x1C, 0x31 };
const uint8_t B1[8] = { 0x0a, 0x57, 0x52, 0x59, 0x49, 0x50, 0x2A, 0x08 };
const uint8_t B2[8] = { 0x1D, 0x41, 0x44, 0x47, 0x4A, 0x4C, 0x3B, 0xFF };
const uint8_t B3[8] = { 0x88, 0x34, 0x36, 0x38, 0x30, 0x2D, 0x13, 0x32 };
const uint8_t B4[8] = { 0x85, 0x5A, 0x43, 0x42, 0x4D, 0x2E, 0xFF, 0x20 };
const uint8_t B5[8] = { 0x86, 0x53, 0x46, 0x48, 0x4B, 0x3A, 0x3D, 0xFF };
const uint8_t B6[8] = { 0x87, 0x45, 0x54, 0x55, 0x4F, 0x00, 0x1E, 0x51 };
const uint8_t B7[8] = { 0x11, 0xFF, 0x58, 0x56, 0x4E, 0x2C, 0x2F, 0x03 };

const uint8_t SB0[8] = { 0x08, 0x33, 0x35, 0x37, 0x39, 0x2B, 0x1C, 0x31 };
const uint8_t SB1[8] = { 0x0a, 0x57, 0x52, 0x59, 0x49, 0x50, 0x2A, 0x08 };
const uint8_t SB2[8] = { 0x1D, 0x41, 0x44, 0x47, 0x4A, 0x4C, 0x3B, 0xFF };
const uint8_t SB3[8] = { 0x88, 0x34, 0x36, 0x38, 0x30, 0x2D, 0x13, 0x32 };
const uint8_t SB4[8] = { 0x85, 0x5A, 0x43, 0x42, 0x4D, 0x2E, 0xFF, 0x20 };
const uint8_t SB5[8] = { 0x86, 0x53, 0x46, 0x48, 0x4B, 0x3A, 0x3D, 0xFF };
const uint8_t SB6[8] = { 0x87, 0x45, 0x54, 0x55, 0x4F, 0x00, 0x1E, 0x51 };
const uint8_t SB7[8] = { 0x11, 0xFF, 0x58, 0x56, 0x4E, 0x2C, 0x2F, 0x03 };

static char scanresult, retval;
char GETKey(void) {
	asm ("lda #$FF");
	asm ("sta %w", DDRB);
	asm ("sta %w", PORTB);
	asm ("lda #$00");
	asm ("sta %w", DDRB);
	asm ("lda #$FF");
	asm ("sta %w", DDRA);
	asm ("lda #%%11111110");
	asm ("sta %w", PORTA);
	asm ("lda %w", PORTB);
	asm ("sta %v", scanresult);
	asm ("lda #%%11111101");
	asm ("sta %w", PORTA);
	asm ("lda %w", PORTB);
	asm ("sta %v+1", scanresult);
   	asm ("lda #%%11111011");
	asm ("sta %w", PORTA);
	asm ("ldy %w", PORTB);
	asm ("sty %v+2", scanresult);
	asm ("lda #%%11110111");
	asm ("sta %w", PORTA);
	asm ("ldy %w", PORTB);
	asm ("sty %v+3", scanresult);
	asm ("lda #%%11101111");
	asm ("sta %w", PORTA);
	asm ("ldy %w", PORTB);
	asm ("sty %v+4", scanresult);
	asm ("lda #%%11011111");
	asm ("sta %w", PORTA);
	asm ("ldy %w", PORTB);
	asm ("sty %v+5", scanresult);
	asm ("lda #%%10111111");
	asm ("sta %w", PORTA);
	asm ("lda %w", PORTB);
	asm ("sta %v+6", scanresult);
	asm ("lda #%%01111111");
	asm ("sta %w", PORTA);
	asm ("ldy %w", PORTB);
	asm ("sty %v+7", scanresult);

	asm ("ldx #$FF");
R0:	asm ("inx");
	asm ("lda %v,x", scanresult);
	asm ("cpx #$08");
	asm ("beq %g", LE);
	asm ("cmp #$FF");
	asm ("beq %g", R0);
	asm ("pha");
	asm ("txa");
	asm ("pha");
	asm ("lda #$FF");
	asm ("sta %w", DDRB);
	asm ("sta %w", PORTB);
	asm ("lda #$00");
	asm ("sta %w", DDRB);
	asm ("lda #%%11111101");
	asm ("sta %w", PORTA);
	asm ("lda %w", PORTB);
	asm ("and #%%01000000");
	asm ("cmp #$00");
	asm ("bne %g", R1);
	asm ("pla");
	asm ("tax");
	asm ("pla");
	asm ("jmp %g", SKB1);
R1:	asm ("lda #$FF");
	asm ("sta %w", DDRB);
	asm ("sta %w", PORTB);
	asm ("lda #$00");
	asm ("sta %w", DDRB);
	asm ("lda #%%10111111");
	asm ("sta %w", PORTA);
	asm ("lda %w", PORTB);
	asm ("and #%%00010000");
	asm ("cmp #$00");
	asm ("bne %g", R2);
	asm ("pla");
	asm ("tax");
	asm ("pla");
	asm ("jmp %g", SKB1);
R2:	asm ("pla");
	asm ("tax");
	asm ("pla");
	asm ("jmp %g", KB1);

LE:	asm ("lda #$FF");
	asm ("sta %v", retval);
	return retval;

// LtoB instance
KB1:
	asm ("eor #$FF");
	asm ("cmp #%%00000001");
	asm ("bne %g", KBLTB0);
	asm ("lda #$00");
	asm ("jmp %g", KBLTB8);
KBLTB0:
	asm ("cmp #%%00000010");
	asm ("bne %g", KBLTB1);
	asm ("lda #$01");
	asm ("jmp %g", KBLTB8);
KBLTB1:
	asm ("cmp #%%00000100");
	asm ("bne %g", KBLTB2);
	asm ("lda #$02");
	asm ("jmp %g", KBLTB8);
KBLTB2:
	asm ("cmp #%%00001000");
	asm ("bne %g", KBLTB3);
	asm ("lda #$03");
	asm ("jmp %g", KBLTB8);
KBLTB3:
	asm ("cmp #%%00010000");
	asm ("bne %g", KBLTB4);
	asm ("lda #$04");
	asm ("jmp %g", KBLTB8);
KBLTB4:
	asm ("cmp #%%00100000");
	asm ("bne %g", KBLTB5);
	asm ("lda #$05");
	asm ("jmp %g", KBLTB8);
KBLTB5:
	asm ("cmp #%%01000000");
	asm ("bne %g", KBLTB6);
	asm ("lda #$06");
	asm ("jmp %g", KBLTB8);
KBLTB6:
	asm ("cmp #%%10000000");
	asm ("bne %g", KBLTB7);
	asm ("lda #$07");
	asm ("jmp %g", KBLTB8);
KBLTB7:
	asm ("ldx #$FF");
KBLTB8:

	asm ("cmp #$FF");
	asm ("beq %g", LE);

	asm ("cpx #$00");
	asm ("bne %g", K0);
	asm ("tax");
	asm ("lda %v,x", SB0);
	asm ("jmp %g", KDN);
K0:	asm ("cpx #$01");
	asm ("bne %g", K1);
	asm ("tax");
	asm ("lda %v,x", SB1);
	asm ("jmp %g", KDN);
K1:	asm ("cpx #$02");
	asm ("bne %g", K2);
	asm ("tax");
	asm ("lda %v,x", SB2);
	asm ("jmp %g", KDN);
K2:	asm ("cpx #$03");
	asm ("bne %g", K3);
	asm ("tax");
	asm ("lda %v,x", SB3);
	asm ("jmp %g", KDN);
K3:	asm ("cpx #$04");
	asm ("bne %g", K4);
	asm ("tax");
	asm ("lda %v,x", SB4);
	asm ("jmp %g", KDN);
K4:	asm ("cpx #$05");
	asm ("bne %g", K5);
	asm ("tax");
	asm ("lda %v,x", SB5);
	asm ("jmp %g", KDN);
K5:	asm ("cpx #$06");
	asm ("bne %g", K6);
	asm ("tax");
	asm ("lda %v,x", SB6);
	asm ("jmp %g", KDN);
K6:	asm ("cpx #$07");
	asm ("bne %g", K7);
	asm ("tax");
	asm ("lda %v,x", SB7);
	asm ("jmp %g", KDN);
K7:	asm ("lda #$FF");
	asm ("tax");
	asm ("sta %v", retval);
	return retval;

// LtoB instance
SKB1:
	asm ("eor #$FF");
	asm ("cmp #%%00000001");
	asm ("bne %g", SKBLTB0);
	asm ("lda #$00");
	asm ("jmp %g", SKBLTB8);
SKBLTB0:
	asm ("cmp #%%00000010");
	asm ("bne %g", SKBLTB1);
	asm ("lda #$01");
	asm ("jmp %g", SKBLTB8);
SKBLTB1:
	asm ("cmp #%%00000100");
	asm ("bne %g", SKBLTB2);
	asm ("lda #$02");
	asm ("jmp %g", SKBLTB8);
SKBLTB2:
	asm ("cmp #%%00001000");
	asm ("bne %g", SKBLTB3);
	asm ("lda #$03");
	asm ("jmp %g", SKBLTB8);
SKBLTB3:
	asm ("cmp #%%00010000");
	asm ("bne %g", SKBLTB4);
	asm ("lda #$04");
	asm ("jmp %g", SKBLTB8);
SKBLTB4:
	asm ("cmp #%%00100000");
	asm ("bne %g", SKBLTB5);
	asm ("lda #$05");
	asm ("jmp %g", SKBLTB8);
SKBLTB5:
	asm ("cmp #%%01000000");
	asm ("bne %g", SKBLTB6);
	asm ("lda #$06");
	asm ("jmp %g", SKBLTB8);
SKBLTB6:
	asm ("cmp #%%10000000");
	asm ("bne %g", SKBLTB7);
	asm ("lda #$07");
	asm ("jmp %g", SKBLTB8);
SKBLTB7:
	asm ("ldx #$FF");
SKBLTB8:

	asm ("cmp #$FF");
	asm ("beq %g", LE);

	asm ("cpx #$00");
	asm ("bne %g", S0);
	asm ("tax");
	asm ("lda %v,x", SB0);
	asm ("jmp %g", KDN);
S0:	asm ("cpx #$01");
	asm ("bne %g", S1);
	asm ("tax");
	asm ("lda %v,x", SB1);
	asm ("jmp %g", KDN);
S1:	asm ("cpx #$02");
	asm ("bne %g", S2);
	asm ("tax");
	asm ("lda %v,x", SB2);
	asm ("jmp %g", KDN);
S2:	asm ("cpx #$03");
	asm ("bne %g", S3);
	asm ("tax");
	asm ("lda %v,x", SB3);
	asm ("jmp %g", KDN);
S3:	asm ("cpx #$04");
	asm ("bne %g", S4);
	asm ("tax");
	asm ("lda %v,x", SB4);
	asm ("jmp %g", KDN);
S4:	asm ("cpx #$05");
	asm ("bne %g", S5);
	asm ("tax");
	asm ("lda %v,x", SB5);
	asm ("jmp %g", KDN);
S5:	asm ("cpx #$06");
	asm ("bne %g", S6);
	asm ("tax");
	asm ("lda %v,x", SB6);
	asm ("jmp %g", KDN);
S6:	asm ("cpx #$07");
	asm ("bne %g", S7);
	asm ("tax");
	asm ("lda %v,x", SB7);
	asm ("jmp %g", KDN);
S7:	asm ("lda #$FF");

KDN:
	asm ("tax");
	asm ("sta %v", retval);
	return retval;
}

//---------------------------------------------------

#endif
