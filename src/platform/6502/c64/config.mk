
CPU = 6502

CC = cc65
LD = ld65
AS = ca65
AR = ar65
OBJ = co65

CCFLAGS = -t c64 --cpu $(CPU) -O
ASFLAGS = -t c64 --cpu $(CPU) -U
LDFLAGS = --mapfile $(BINDIR)/cyclamen.map -C $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld c64.lib

.PHONY: all-phony run
all-phony:
	$(MAKE) all

run:
	x64 -kernal $(BINARY_NAME)
