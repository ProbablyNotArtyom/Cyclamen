_exit					export
RESET_VEC				export

INITGL					import
_main                   import
_platform_init			import
program_start			import

s_rwdata    			import
l_rwdata    			import
s_rodata    			import
l_rodata    			import
s_bss       			import
l_bss       			import
s_code       			import
l_code       			import
s_rwdata    			import

; ------------------------------------------------------------------------
; Startup code

	section code

RESET_VEC:
	jsr	_platform_init			; Do platform specific stuff

	; Clear first 256 bytes (Direct Page)
    clra						; Load a with zero
    clrb						; Load counter b with the length of the DP. (starts with 0, then rolls under to 0xFF to get a length of 256)
@l:	sta ,u+
    decb
    bne @l

	; Copy the data segment to into memory
    ldy #l_rwdata				; Length of segment
    beq @d						; Skip if theres nothing to copy
    leau s_rwdata,pcr			; Location of the data segment in RAM
    leax s_rodata+l_rodata,pcr	; Location of the actual data in ROM
@l:	ldb ,x+
    stb ,u+
    leay -1,y
    bne @l
@d:

	; Zero out the BSS
    ldx #l_bss				; Length of the BSS section
    beq @d					; Skip if theres nothing to zero
    leau s_bss,pcr			; Location of the BSS section in RAM
@l:	clr ,u+
    leax -1,x
    bne @l
@d:

	leas s_code-1,pcr		; Set the stack pointer to the end of memory (just before the ROM)
	lbsr INITGL				; Initialize global variables
    lbsr constructors		; Attempt to initialize constructors

    jsr _main		; Jump to main
	
_exit:
	jmp _exit				; Main returned for some reason, so just loop in place
							; Ideally this should never happen, and main will run until power off

	endsect

; ----------------------- Constructors/Destructors -----------------------

    section constructors_start
constructors:
    endsect

    section constructors_end
    rts
    endsect

    section destructors_start
destructors:
    endsect

    section destructors_end
    rts
    endsect
