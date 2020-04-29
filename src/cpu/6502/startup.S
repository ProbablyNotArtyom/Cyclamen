;
; Startup code for cc65 (C64 version)
;

        .export         _exit
        .export         __STARTUP__ : absolute = 1		; Mark as startup
		.export 		RESET_VEC, mmusave

		.import 		_platform_init
        .import         initlib, donelib
        .import         zerobss, callmain
        .import         BSOUT
        .import         __STACK__, __STACKSIZE__		; from configure file
        .importzp       ST

		.import			__DATA_SIZE__, __DATA__START__

        .include        "zeropage.inc"

; ------------------------------------------------------------------------
; Startup code

.segment        "STARTUP"

RESET_VEC:
		jsr		_platform_init	; Do platform specific stuff

        tsx
        stx     spsave          ; Save the system stack ptr

; Save space by putting some of the start-up code in the ONCE segment,
; which can be re-used by the BSS segment, the heap and the C stack.

        jsr     init

        jsr     zerobss
		jsr		copydata

; Push the command-line arguments; and, call main().

        jsr     callmain

; Back from main() [this is also the exit() entry]. Run the module destructors.

_exit:  jmp _exit

; ------------------------------------------------------------------------

.segment        "ONCE"

init:

; Save the zero-page locations that we need.

        ldx     #zpspace-1
L1:     lda     sp,x
        sta     zpsave,x
        dex
        bpl     L1

; Set up the stack.

        lda     #<(__STACK__)
        ldx     #>(__STACK__)
        sta     sp
        stx     sp+1            ; Set argument stack ptr

; Call the module constructors.

        rts

; ------------------------------------------------------------------------
; Data

.segment        "INIT"

mmusave:.res    1
spsave: .res    1
zpsave: .res    zpspace
