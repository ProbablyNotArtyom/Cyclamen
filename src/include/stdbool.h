#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifndef CONFIG_PCXT
#define bool _Bool
typedef unsigned char _Bool;
#else
typedef unsigned char bool;
#endif

/* Standard test-results. */
#define false 0
#define true  1

#endif
