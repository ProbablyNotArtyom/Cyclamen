
//---------------------------------------------------
// Memory / IO access macros

#ifndef _HEADER_CPU_IO
#define _HEADER_CPU_IO

	#include <std.h>
	#include <stdint.h>

//---------------------------------------------------

#define outb(val, addr)		((*(uint8_t *) (addr)) = (val))
#define outw(val, addr)		((*(uint16_t *) (addr)) = (val))
#define outl(val, addr)		((*(uint32_t *) (addr)) = (val))

#define inb(addr)			({ uint8_t __v = (*(uint8_t *) (addr)); __v; })
#define inw(addr)			({ uint16_t __v = (*(uint16_t *) (addr)); __v; })
#define inl(addr)			({ uint32_t __v = (*(uint32_t *) (addr)); __v; })

//---------------------------------------------------

uint16_t swapw(uint16_t val);
uint32_t swapl(uint32_t val);
void io_outsb(size_t addr, const void* buff, size_t len);
void io_outsw(size_t addr, const void* buff, size_t len);
void io_outsl(size_t addr, const void* buff, size_t len);
void io_insb(size_t addr, void* buff, size_t len);
void io_insw(size_t addr, void* buff, size_t len);
void io_insl(size_t addr, void* buff, size_t len);

//---------------------------------------------------
#endif
