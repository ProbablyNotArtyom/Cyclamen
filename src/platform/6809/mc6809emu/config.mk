
CPU = 6809

CC = cmoc
LD = lwlink
AS = lwasm
AR = lwar
OBJ = lwobjdump

CCFLAGS = --no-relocate --srec --data=0000 --org=C000 -i -S
ASFLAGS = --obj -3 -f obj
LDFLAGS = --format=srec -m$(BINDIR)/cyclamen.map --script=$(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld -L$(SRCDIR)/cpu/$(ARCH) -lcmoc

.PHONY: all-phony run
all-phony:
	$(MAKE) all
	$(MAKE) makebin

makebin:
	@srec_cat $(BINARY_NAME) -Output $(BINARY_NAME).bin -Binary

run: makebin
	$(TOOLDIR)/mc09emulator $(BINARY_NAME).bin 0000
