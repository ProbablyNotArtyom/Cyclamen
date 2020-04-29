.ifdef __6502_ENTRY_H
.define __6502_ENTRY_H

.macro SAVE_ALL
.if (.cpu & CPU_ISET_65816 || .cpu & CPU_ISET_65C02)
	pha
	phx
	phy
.else
	pha
	txa
	pha
	tya
	pha
.endif
.endmacro

.macro RESTORE_ALL
.if (.cpu & CPU_ISET_65816 || .cpu & CPU_ISET_65C02)
	pla
	plx
	ply
.else
	pla
	tay
	pla
	tax
	pla
.endif
.endmacro

.endif
