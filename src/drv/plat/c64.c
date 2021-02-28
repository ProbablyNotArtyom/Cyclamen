//---------------------------------------------------
// Cyclamen - NotArtyom - 2020
//---------------------------------------------------

	#include <hwdeps.h>
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>

	#include <std.h>
	#include <ctype.h>

#ifdef DEV_PLAT_C64
#define DEV_PLAT_C64

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

//  WRITE TO |                    READ PORT B ($DC01)
//  PORT A   |
//  ($DC00)  |  Bit 7   Bit 6   Bit 5   Bit 4   Bit 3   Bit 2   Bit 1   Bit 0
// ----------|---------------------------------------------------------------
//   Bit 7   |  STOP    Q       C=      SPACE   2       CTRL    <-      1
//   Bit 6   |  /       ^       =       RSHIFT  HOME    ;       *       LIRA
//   Bit 5   |  ,       @       :       .       -       L       P       +
//   Bit 4   |  N       O       K       M       0       J       I       9
//   Bit 3   |  V       U       H       B       8       G       Y       7
//   Bit 2   |  X       T       F       C       6       D       R       5
//   Bit 1   |  LSHIFT  E       S       Z       4       A       W       3
//   Bit 0   |  DOWN    F5      F3      F1      F7      RIGHT   RETURN  DELETE

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
	__asm__("lda #$FF");
	__asm__("sta %w", DDRB);
	__asm__("sta %w", PORTB);
	__asm__("lda #$00");
	__asm__("sta %w", DDRB);
	__asm__("lda #$FF");
	__asm__("sta %w", DDRA);
	__asm__("lda #%%11111110");
	__asm__("sta %w", PORTA);
	__asm__("lda %w", PORTB);
	__asm__("sta %v", scanresult);
	__asm__("lda #%%11111101");
	__asm__("sta %w", PORTA);
	__asm__("lda %w", PORTB);
	__asm__("sta %v+1", scanresult);
   	__asm__("lda #%%11111011");
	__asm__("sta %w", PORTA);
	__asm__("ldy %w", PORTB);
	__asm__("sty %v+2", scanresult);
	__asm__("lda #%%11110111");
	__asm__("sta %w", PORTA);
	__asm__("ldy %w", PORTB);
	__asm__("sty %v+3", scanresult);
	__asm__("lda #%%11101111");
	__asm__("sta %w", PORTA);
	__asm__("ldy %w", PORTB);
	__asm__("sty %v+4", scanresult);
	__asm__("lda #%%11011111");
	__asm__("sta %w", PORTA);
	__asm__("ldy %w", PORTB);
	__asm__("sty %v+5", scanresult);
	__asm__("lda #%%10111111");
	__asm__("sta %w", PORTA);
	__asm__("lda %w", PORTB);
	__asm__("sta %v+6", scanresult);
	__asm__("lda #%%01111111");
	__asm__("sta %w", PORTA);
	__asm__("ldy %w", PORTB);
	__asm__("sty %v+7", scanresult);

	__asm__("ldx #$FF");
R0:	__asm__("inx");
	__asm__("lda %v,x", scanresult);
	__asm__("cpx #$08");
	__asm__("beq %g", LE);
	__asm__("cmp #$FF");
	__asm__("beq %g", R0);
	__asm__("pha");
	__asm__("txa");
	__asm__("pha");
	__asm__("lda #$FF");
	__asm__("sta %w", DDRB);
	__asm__("sta %w", PORTB);
	__asm__("lda #$00");
	__asm__("sta %w", DDRB);
	__asm__("lda #%%11111101");
	__asm__("sta %w", PORTA);
	__asm__("lda %w", PORTB);
	__asm__("and #%%01000000");
	__asm__("cmp #$00");
	__asm__("bne %g", R1);
	__asm__("pla");
	__asm__("tax");
	__asm__("pla");
	__asm__("jmp %g", SKB1);
R1:	__asm__("lda #$FF");
	__asm__("sta %w", DDRB);
	__asm__("sta %w", PORTB);
	__asm__("lda #$00");
	__asm__("sta %w", DDRB);
	__asm__("lda #%%10111111");
	__asm__("sta %w", PORTA);
	__asm__("lda %w", PORTB);
	__asm__("and #%%00010000");
	__asm__("cmp #$00");
	__asm__("bne %g", R2);
	__asm__("pla");
	__asm__("tax");
	__asm__("pla");
	__asm__("jmp %g", SKB1);
R2:	__asm__("pla");
	__asm__("tax");
	__asm__("pla");
	__asm__("jmp %g", KB1);

LE:	__asm__("lda #$FF");
	__asm__("sta %v", retval);
	return retval;

// LtoB instance
KB1:
	__asm__("eor #$FF");
	__asm__("cmp #%%00000001");
	__asm__("bne %g", KBLTB0);
	__asm__("lda #$00");
	__asm__("jmp %g", KBLTB8);
KBLTB0:
	__asm__("cmp #%%00000010");
	__asm__("bne %g", KBLTB1);
	__asm__("lda #$01");
	__asm__("jmp %g", KBLTB8);
KBLTB1:
	__asm__("cmp #%%00000100");
	__asm__("bne %g", KBLTB2);
	__asm__("lda #$02");
	__asm__("jmp %g", KBLTB8);
KBLTB2:
	__asm__("cmp #%%00001000");
	__asm__("bne %g", KBLTB3);
	__asm__("lda #$03");
	__asm__("jmp %g", KBLTB8);
KBLTB3:
	__asm__("cmp #%%00010000");
	__asm__("bne %g", KBLTB4);
	__asm__("lda #$04");
	__asm__("jmp %g", KBLTB8);
KBLTB4:
	__asm__("cmp #%%00100000");
	__asm__("bne %g", KBLTB5);
	__asm__("lda #$05");
	__asm__("jmp %g", KBLTB8);
KBLTB5:
	__asm__("cmp #%%01000000");
	__asm__("bne %g", KBLTB6);
	__asm__("lda #$06");
	__asm__("jmp %g", KBLTB8);
KBLTB6:
	__asm__("cmp #%%10000000");
	__asm__("bne %g", KBLTB7);
	__asm__("lda #$07");
	__asm__("jmp %g", KBLTB8);
KBLTB7:
	__asm__("ldx #$FF");
KBLTB8:

	__asm__("cmp #$FF");
	__asm__("beq %g", LE);

	__asm__("cpx #$00");
	__asm__("bne %g", K0);
	__asm__("tax");
	__asm__("lda %v,x", B0);
	__asm__("jmp %g", KDN);
K0:	__asm__("cpx #$01");
	__asm__("bne %g", K1);
	__asm__("tax");
	__asm__("lda %v,x", B1);
	__asm__("jmp %g", KDN);
K1:	__asm__("cpx #$02");
	__asm__("bne %g", K2);
	__asm__("tax");
	__asm__("lda %v,x", B2);
	__asm__("jmp %g", KDN);
K2:	__asm__("cpx #$03");
	__asm__("bne %g", K3);
	__asm__("tax");
	__asm__("lda %v,x", B3);
	__asm__("jmp %g", KDN);
K3:	__asm__("cpx #$04");
	__asm__("bne %g", K4);
	__asm__("tax");
	__asm__("lda %v,x", B4);
	__asm__("jmp %g", KDN);
K4:	__asm__("cpx #$05");
	__asm__("bne %g", K5);
	__asm__("tax");
	__asm__("lda %v,x", B5);
	__asm__("jmp %g", KDN);
K5:	__asm__("cpx #$06");
	__asm__("bne %g", K6);
	__asm__("tax");
	__asm__("lda %v,x", B6);
	__asm__("jmp %g", KDN);
K6:	__asm__("cpx #$07");
	__asm__("bne %g", K7);
	__asm__("tax");
	__asm__("lda %v,x", B7);
	__asm__("jmp %g", KDN);
K7:	__asm__("lda #$FF");
	__asm__("tax");
	__asm__("sta %v", retval);
	return retval;

// LtoB instance
SKB1:
	__asm__("eor #$FF");
	__asm__("cmp #%%00000001");
	__asm__("bne %g", SKBLTB0);
	__asm__("lda #$00");
	__asm__("jmp %g", SKBLTB8);
SKBLTB0:
	__asm__("cmp #%%00000010");
	__asm__("bne %g", SKBLTB1);
	__asm__("lda #$01");
	__asm__("jmp %g", SKBLTB8);
SKBLTB1:
	__asm__("cmp #%%00000100");
	__asm__("bne %g", SKBLTB2);
	__asm__("lda #$02");
	__asm__("jmp %g", SKBLTB8);
SKBLTB2:
	__asm__("cmp #%%00001000");
	__asm__("bne %g", SKBLTB3);
	__asm__("lda #$03");
	__asm__("jmp %g", SKBLTB8);
SKBLTB3:
	__asm__("cmp #%%00010000");
	__asm__("bne %g", SKBLTB4);
	__asm__("lda #$04");
	__asm__("jmp %g", SKBLTB8);
SKBLTB4:
	__asm__("cmp #%%00100000");
	__asm__("bne %g", SKBLTB5);
	__asm__("lda #$05");
	__asm__("jmp %g", SKBLTB8);
SKBLTB5:
	__asm__("cmp #%%01000000");
	__asm__("bne %g", SKBLTB6);
	__asm__("lda #$06");
	__asm__("jmp %g", SKBLTB8);
SKBLTB6:
	__asm__("cmp #%%10000000");
	__asm__("bne %g", SKBLTB7);
	__asm__("lda #$07");
	__asm__("jmp %g", SKBLTB8);
SKBLTB7:
	__asm__("ldx #$FF");
SKBLTB8:

	__asm__("cmp #$FF");
	__asm__("beq %g", LE);

	__asm__("cpx #$00");
	__asm__("bne %g", S0);
	__asm__("tax");
	__asm__("lda %v,x", B0);
	__asm__("jmp %g", KDN);
S0:	__asm__("cpx #$01");
	__asm__("bne %g", S1);
	__asm__("tax");
	__asm__("lda %v,x", B1);
	__asm__("jmp %g", KDN);
S1:	__asm__("cpx #$02");
	__asm__("bne %g", S2);
	__asm__("tax");
	__asm__("lda %v,x", B2);
	__asm__("jmp %g", KDN);
S2:	__asm__("cpx #$03");
	__asm__("bne %g", S3);
	__asm__("tax");
	__asm__("lda %v,x", B3);
	__asm__("jmp %g", KDN);
S3:	__asm__("cpx #$04");
	__asm__("bne %g", S4);
	__asm__("tax");
	__asm__("lda %v,x", B4);
	__asm__("jmp %g", KDN);
S4:	__asm__("cpx #$05");
	__asm__("bne %g", S5);
	__asm__("tax");
	__asm__("lda %v,x", B5);
	__asm__("jmp %g", KDN);
S5:	__asm__("cpx #$06");
	__asm__("bne %g", S6);
	__asm__("tax");
	__asm__("lda %v,x", B6);
	__asm__("jmp %g", KDN);
S6:	__asm__("cpx #$07");
	__asm__("bne %g", S7);
	__asm__("tax");
	__asm__("lda %v,x", B7);
	__asm__("jmp %g", KDN);
S7:	__asm__("lda #$FF");

KDN:
	__asm__("tax");
	__asm__("sta %v", retval);
	return retval;
}

//---------------------------------------------------

#endif
