#ifndef _STDINT_H
#define _STDINT_H

/* Exact-width integer types */
typedef signed char         int8_t;
typedef int                 int16_t;
typedef long                int32_t;
typedef unsigned char       uint8_t;
typedef unsigned            uint16_t;
typedef unsigned long       uint32_t;

typedef unsigned			size_t;

#define INT8_MIN            ((int8_t) 0x80)
#define INT8_MAX            ((int8_t) 0x7F)
#define INT16_MIN           ((int16_t) 0x8000)
#define INT16_MAX           ((int16_t) 0x7FFF)
#define INT32_MIN           ((int32_t) 0x80000000)
#define INT32_MAX           ((int32_t) 0x7FFFFFFF)
#define UINT8_MAX           ((uint8_t) 0xFF)
#define UINT16_MAX          ((uint16_t) 0xFFFF)
#define UINT32_MAX          ((uint32_t) 0xFFFFFFFF)

#define INT8_WIDTH 8
#define UINT8_WIDTH 8
#define INT16_WIDTH 16
#define UINT16_WIDTH 16
#define INT32_WIDTH 32
#define UINT32_WIDTH 32
#define INT64_WIDTH 64
#define UINT64_WIDTH 64

#define NULL 	0

#endif
