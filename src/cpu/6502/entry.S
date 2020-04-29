
	.include "entry.inc"

	.export IRQ_VEC
	.export NMI_VEC

IRQ_VEC:
	nop
	jmp RESET_VEC

NMI_VEC:
	nop
	jmp RESET_VEC
