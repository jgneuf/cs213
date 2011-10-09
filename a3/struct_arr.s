.pos 0x1000
code:
				ld	$v,			r0		# r0 = v
				ld	$s,			r1		# r1 = s.x
				ld 	$i,			r2		# r2 = i
				ld	0x0(r1),	r3		# r3 = s.x[i]
				st	r3,			0x0(r0) # v = s.x[i]
				inca 			r1		# r1 = s.x[1]
				inca 			r1		# r1 = s.y
				ld	$heap0,		r4		# r4 = y's array
				st	r4,			0x0(r1) # s.y = (int*) malloc (sizeof (int))
				ld	0x0(r1),	r3		# r3 = &s.y[i]
				ld 	0x0(r3),	r3		# r3 = s.y[i]
				st	r3,			0x0(r0) # v = s.y[i]
				inca			r1		# r1 = s.z
				ld	$heap1,		r4		# r4 = z's struct
				st	r4,			0x0(r1) # s.z = malloc (sizeof (struct D))
				ld	0x0(r1),	r3		# r3 = &s.z->x[i]
				ld 	0x0(r3),	r3		# r3 = s.z->x[i]
				st	r3,			0x0(r0) # v = s.z->x[i]
				halt                     
.pos 0x2000
i:				.long 0
v:				.long 0
s:				.long 1					# x[0]
				.long 2					# x[1]
				.long 0					# y
				.long 0					# z
.pos 0x3000
heap0:			.long 3					# y[0]
				.long 4					# y[1]
				.long 5					# y[2]
heap1:			.long 6					# z->x[0]
				.long 7					# z->x[1]
				.long 8					# z->y
				.long 9					# z->z
