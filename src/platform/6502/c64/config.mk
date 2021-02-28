
CPU = 6502

CC = cc65
LD = ld65
AS = ca65
AR = ar65
OBJ = co65

CCFLAGS = -t c64 --cpu $(CPU) -O --standard c99
ASFLAGS = -t c64 --cpu $(CPU) -U
LDFLAGS = -Ln $(BINDIR)/cyclamen.vs --mapfile $(BINDIR)/cyclamen.map --dbgfile $(BINDIR)/cyclamen.dbg -C $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld c64.lib

.PHONY: all-phony run debug
all-phony:
	$(MAKE) all

run:
	x64sc -kernal $(BINARY_NAME)

debug:
	c64debugger -kernal $(BINARY_NAME) -moncommands "$(TOOLDIR)/vice-startup"
