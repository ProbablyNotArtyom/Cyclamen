
RESET_VEC	import

IRQ_VEC		export
NMI_VEC		export
FIRQ_VEC	export
SWI_VEC		export

	section code

FIRQ_VEC:
	nop
	jmp RESET_VEC

IRQ_VEC:
	nop
	jmp RESET_VEC

NMI_VEC:
	nop
	jmp RESET_VEC

SWI_VEC:
	nop
	jmp RESET_VEC

	endsect
