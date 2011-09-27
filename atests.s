.pos 0x100
				ld		$0x0,		r0		# r0 = 0 - LI
				ld		$0xf,		r1		# r1 = 15 - LI
				ld		$a,			r2		# r2 = a - LI
				ld 		$b,			r3		# r3 = b - LI
				st		r0,			0x0(r2) # a = 0 - ST b+o
				st 		r1,			0x0(r3) # b = 15 - ST b+o
				mov		r1,			r0		# r0 = 15 - MOV
				add		r1,			r0		# r0 = 15 + 15 - ADD
				and		r1,			r0		# r0 =  14 - AND
				inc		r0					# r0 = 15 - INC
				dec		r0					# r0 = 14 - DEC
				not 	r0					# NOT
				nop							# :)
				halt

.pos 0x1000
a:				.long	0xffffffff			# a
.pos 0x2000
b:				.long	0xffffffff			# b
.pos 0x3000
c:				.long	0xffffffff			# c[0]
				.long	0xffffffff			# c[1]
				.long	0xffffffff			# c[2]
				.long	0xffffffff			# c[3]
				.long	0xffffffff			# c[4]
