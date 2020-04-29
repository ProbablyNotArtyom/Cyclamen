
IRQ_VEC		import
FIRQ_VEC	import
NMI_VEC		import
RESET_VEC	import
SWI_VEC		import

	section vectors

	FDB 0
	FDB 0
	FDB 0
	FDB FIRQ_VEC
	FDB IRQ_VEC
	FDB SWI_VEC
	FDB NMI_VEC
	FDB RESET_VEC

	endsect
