
CPU = 6309

CC = cmoc
LD = lwlink
AS = lwasm
AR = lwar
OBJ = lwobjdump

CCFLAGS = --no-relocate --srec --data=0000 --org=C000 -i -S
ASFLAGS = --obj -3 -f obj
LDFLAGS = --format=raw -m$(BINDIR)/cyclamen.map --script=$(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/link.ld -L$(SRCDIR)/cpu/$(ARCH) -lcmoc
