.pos 0x1000
code:
				ld	$v,			r0		# r0 = v
				ld	$s,			r1		# r1 = s
				ld	0x0(r1),	r2		# r2 = s.x[i]
				st	r2,			0x0(r0) # v = s.x[i]
				halt                     
.pos 0x2000
i:				.long 0
v:				.long 0
s:				.long 0					# x[0]
				.long 0					# x[1]
				.long 0					# y
				.long 0					# z
.pos 0x3000
heap:
