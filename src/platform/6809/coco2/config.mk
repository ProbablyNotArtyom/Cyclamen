
CPU = 6809

CC = cmoc
LD = lwlink
AS = lwasm
AR = lwar
OBJ = lwobjdump

CCFLAGS = --no-relocate --srec --data=0000 --org=800 -i -S
ASFLAGS = --obj -3 -f obj
LDFLAGS = --format=decb -m$(BINDIR)/cyclamen.map --script=$(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld -L$(SRCDIR)/cpu/$(ARCH) -lcmoc

.PHONY: all-phony run
all-phony:
	$(MAKE) all
	$(MAKE) makebin

makebin:
	@cp -f $(BINARY_NAME) $(BINARY_NAME).bin

run: all makebin
	xroar -rompath /home/artyom/Projects/project-files/_Retro_/CoCo/ROMs/coco2 -machine coco2bus -run $(BINARY_NAME).bin
