
//---------------------------------------------------
// Memory / IO access macros

#ifndef _HEADER_CPU_IO
#define _HEADER_CPU_IO

	#include <std.h>
	#include <stdint.h>

//---------------------------------------------------

#define outb(val, addr)		((*(volatile uint8_t *) (addr)) = (val))
#define outw(val, addr)		((*(volatile uint16_t *) (addr)) = (val))
#define outl(val, addr)		((*(volatile uint32_t *) (addr)) = (val))

#define inb(addr)			({ uint8_t __v = (*(volatile uint8_t *) (addr)); __v; })
#define inw(addr)			({ uint16_t __v = (*(volatile uint16_t *) (addr)); __v; })
#define inl(addr)			({ uint32_t __v = (*(volatile uint32_t *) (addr)); __v; })

//---------------------------------------------------

static uint16_t swapw(volatile uint16_t val);
static uint32_t swapl(volatile uint32_t val);
static void io_outsb(size_t addr, const void* buff, size_t len);
static void io_outsw(size_t addr, const void* buff, size_t len);
static void io_outsl(size_t addr, const void* buff, size_t len);
static void io_insb(size_t addr, void* buff, size_t len);
static void io_insw(size_t addr, void* buff, size_t len);
static void io_insl(size_t addr, void* buff, size_t len);

//---------------------------------------------------
#endif
