stx
_exit					export
RESET_VEC				export
INILIB					export
null_ptr_handler		export
    ifndef OS9
end_of_sbrk_mem         export
program_break           export
stack_overflow_handler  export
    endc

INITGL					import
_platform_init			import
program_start			import
program_end				import

s_rwdata    			import
l_rwdata    			import
s_rodata    			import
l_rodata    			import
s_bss       			import
l_bss       			import
s_initgl_end 			import
l_initgl_end 			import
s_end_romdata 			import
l_end_romdata 			import
s_vectors				import
l_vectors 				import
s_rwdata    			import

; ------------------------------------------------------------------------
; Startup code

	section code

; Initializes the standard library.
; Input: D = minus the number of bytes for the stack space (e.g., -1024 for 1k stack).
;        Not used under OS-9.
; Initializes the "bss" section with zeroes, under non OS-9 platforms.
; Saves the initial stack for use by exit().
; Initializes internal variables used by sbrk(), except under OS-9.
; Initializes the null pointer handler pointer.
; Initializes the stack overflow handler pointer, except under OS-9.
; Initializes CHROUT, which points to the system's character output routine.
; Initializes the user program's global variables.
;
INILIB:
	ifndef OS9
; Zero out BSS segment (for OS-9, done by OS9PREP).
; Must be done first, because code that follows initializes INISTK, etc.
l_bss   import
s_bss   import
        ldx #l_bss                  ; number of bytes in "bss" section
        beq @done
        leau s_bss,pcr               ; address of "bss" section
@loop:
        clr ,U+
        leax -1,X
        bne @loop
@done:
	endc

	ifdef OS9
	leax 6,S			;bX = initial stack pointer (OS-9 has argc and argv in stack)
	else
	leax 2,S			; X = initial stack pointer
	endc
	stx	INISTK,pcr		; save this for exit()

	ifndef OS9
	leax D,X						; point to top of stack space
	stx	end_of_sbrk_mem,pcr	sbrk() 	; will not allocate past this
	leax program_end,PCR			; end of generated code and data
	stx	program_break,pcr			; initial Unix-like "program break" (cf sbrk)
	endc

	leax nop_handler,PCR
	stx	null_ptr_handler,pcr

	ifndef OS9
    ldx	#0
	stx	stack_overflow_handler,pcr
	endc

	lbsr INITGL				; initialize global variables
    lbra constructors

RESET_VEC:
	jsr	_platform_init	; Do platform specific stuff

	; Clear first 256 bytes (Direct Page)
    clra				; setup to clear
    clrb				; 256 bytes
dpclrl:
	sta ,u+				; clear dp bytes
    decb
    bne dpclrl

	; Copy the initialized data, if any, to the data segment at Y.
    leau ,y                 		; destination: data segment
    clr ,u+                 		; reserve 1st byte so that (void *) serves as invalid ptr
    ldy #l_rwdata           		; length to copy
    beq @done               		; if table empty
    leax s_rodata+l_rodata,PCR		; 1st (writable) data address to copy
@loop:
    ldb ,x+
    stb ,u+
    leay -1,y
    bne @loop
@done:

	; Zero out the uninitialized data area (at U, right after initialized data).
    ldx #l_bss              ; length to write
    beq @done
@zero:
    clr ,u+
    leax -1,x
    bne @zero
@done:

    puls x,y,u

    jsr program_start

_exit:
	lbsr destructors

	ifdef _COCO_OR_DRAGON_BASIC_

* Zero out the LINBUF line buffer.  This seems necessary to avoid
* ?SN ERROR after program execution on the CoCo.
	ldx	#LINBUF
EXIT10:
	clr	,X+
	cmpx #LINBUF+1+LBUFMX
	blo	EXIT10

    ldd 2,S             ; get exit() arg (may be main() return value)
	lds	INISTK,pcr		; retrieve stack pointer saved at beginning
	; Fall through to the RTS at nop_handler.

	endc

	ifdef OS9
	ldb 3,s             ; get LSB of exit() argument
	os9	F$Exit
	endc

	ifdef USIM
	sync				; to leave usim
	endc

	ifdef VECTREX
	bra	_exit			; The eternal Vectrex loop on exit...
	endc

nop_handler:
	rts

	endsect

	section bss

INISTK:		RMB	2		; receives initial stack pointer

; Initialized by INILIB to an RTS routine.
; Call set_null_ptr_handler() to specify another handler.
; The handler is assumed to have this signature:
; void handler(char *addressOfFailedCheck);
;
null_ptr_handler:	RMB	2

	ifndef OS9

; Initialized by INILIB to a null pointer.
; Call set_stack_overflow_handler() to specify another handler.
; The handler is assumed to have this signature:
; void handler(char *addressOfFailedCheck, char *stackRegister);
;
stack_overflow_handler:	RMB	2


end_of_sbrk_mem:	RMB	2
program_break:		RMB	2
	endc
	endsect

; This section will precede the "constructors" section, i.e., all code that initializes
; user libraries. Such code must not end with an RTS instruction.
; This section exists to define the address of the pre-main constructor subroutine.
        section constructors_start
constructors:
        endsect

; This section will follow the "constructors" section. It ends the pre_main_init routine.
        section constructors_end
        rts
        endsect

; Similarly for destructor code.
        section destructors_start
destructors:
        endsect

        section destructors_end
        rts
        endsect

bytes_unused equ s_vectors-s_end_romdata

	section end_romdata
	fcb $00

	org $FFDE
	fcb $00
	endsect
