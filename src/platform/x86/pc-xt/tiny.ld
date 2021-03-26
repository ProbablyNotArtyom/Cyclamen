ENTRY(_start)

SECTIONS {
	.text 0x00000 : {
		/* Actual segment contents. */
		*(.text .text$* ".text.*[^&]")
		*(.rodata .rodata$* ".rodata.*[^&]")
		*(.data .data$* ".data.*[^&]")
		*(.bss .bss$* ".bss.*[^&]")
		*(COMMON)

		ASSERT (. + 0x100 - ADDR (.text) <= 0xfff0,
		    "Error: too large for a tiny-model ELKS a.out file.");
	}

	/DISCARD/ : { *(.comment) }

	/* `entry' was the old entry point symbol.  (Unfortunately this
	   PROVIDE does not seem to work with the ld.gold linker --- it did
	   work with ld.bfd.  FIXME?)  */
	PROVIDE (_start = DEFINED (entry) ? entry : 0);
}
