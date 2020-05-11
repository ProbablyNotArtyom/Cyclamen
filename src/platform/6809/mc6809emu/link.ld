entry RESET_VEC

section *,bss load 0000
section rwdata
section data
section code load C000
section text
section start
section initgl_start
section initgl
section initgl_end
section constructors_start
section constructors
section constructors_end
section destructors_start
section destructors
section destructors_end
section rodata
section end_romdata
section vectors load FFF0

define lensympat l_%s
define basesympat s_%s
