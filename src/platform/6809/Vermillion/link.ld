entry RESET_VEC

section bss load 0000
section rwdata
section data
section code load C000
section text
section start
section initgl_start
section initgl
section initgl_end
section rodata
section end_romdata
section vectors load FFDF

define lensympat l_%s
define basesympat s_%s
