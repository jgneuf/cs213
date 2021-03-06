.pos 0x100
				ld		$0x0,		r0			# r0 = 0 - LI
				ld		$0xf,		r1			# r1 = 15 - LI
				ld		$a,			r2			# r2 = a - LI
				ld 		$b,			r3			# r3 = b - LI
				st		r0,			0x0(r2) 	# a = 0 - ST b+o
				st 		r1,			0x0(r3) 	# b = 15 - ST b+o
				mov		r1,			r0			# r0 = 15 - MOV
				add		r1,			r0			# r0 = 15 + 15 - ADD
				and		r1,			r0			# r0 =  14 - AND
				inc		r0						# r0 = 15 - INC
				dec		r0						# r0 = 14 - DEC
				not 	r0						# NOT
				nop								# NOP
				ld		$0x0,		r2			# r2 = 0 - LI
				shl		$8,			r1			# r1 = 256 - SHL
				ld		$c,			r0			# r0 = c - LI
				st 		r1,			(r0,r2,4)	# c[0] = 256 - ST indexed
				inca	r2						# r2 += 4 - INCA
				st		r1,			(r0,r2,4)	# c[4] = 256 - ST indexed
				deca	r2						# r2 -+ 4 - DECA
				ld		0x0(r0),	r5			# r5 = c[0]	- LD b+o
				ld		0x4(r0),	r6			# r6 = c[1] - LD b+o
				ld		(r5,r2,4),	r7			# r7 = c[0]
				inca	r2						# r2 += 4
				br		jumper					# jump to jumper
				halt

jumper:
				ld 		$0xffff,	r0			# r0 = 65535
                beq     r0,         jumper2     # shouldn't branch
                ld      $0x0,       r0          # r0 = 0
                beq     r0,         jumper2     # should branch
                halt                            # should not execute

jumper2:
                bgt     r0,         jumper3     # shouldn't branch
                inc     r0                      # r0 = 1
                bgt     r0,         jumper3     # should branch
                halt                            # should not execute
                
jumper3:
                j       jumper4                # jump to jumper4
                halt

jumper4:
                gpc     $0x0,        r1          # r1 = pc + 0
                gpc     $0x4,        r1          # r1 = pc + 4      
                ld      $0x900,      r2          # r2 = 0x900
                j       0x0(r2)                  # jump to 0x900
				halt
.pos 0x900
                halt
.pos 0x1000
a:				.long	0xffffffff				# a
.pos 0x2000
b:				.long	0xffffffff				# b
.pos 0x3000
c:				.long	0xffffffff				# c[0]
				.long	0xffffffff				# c[1]
				.long	0xffffffff				# c[2]
				.long	0xffffffff				# c[3]
				.long	0xffffffff				# c[4]
