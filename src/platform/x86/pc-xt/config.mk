
CPU = i8086

CC = ia16-elf-gcc
LD = ia16-elf-ld
AS = ia16-elf-as
AR = ia16-elf-ar
OBJ = ia16-elf-objdump

CCFLAGS = -ffreestanding -fno-inline -mcmodel=small -msegment-relocation-stuff -mtune=$(CPU) -Os -S
ASFLAGS = -mtune=$(CPU)
LDSCRIPT = tiny.ld
LDFLAGS = -T $(BASEDIR)/src/platform/$(ARCH)/$(PLATFORM)/$(LDSCRIPT)

.PHONY: all-phony run debug
all-phony:
	$(MAKE) all

run:
	qemu-system-x86_64 -serial stdio -nodefaults -name Cyclamen -machine isapc -cpu 486,tsc -m 640K -display sdl -vga std -rtc base=utc $(BINARY_NAME)
