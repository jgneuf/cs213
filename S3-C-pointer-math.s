.pos 0x100
                 ld   $0x0, r0            # r0 = 0
                 ld   $c, r1              # r1 = address of c
                 ld   0x0(r1), r2         # r2 = c
                 st   r0, 0x0(r2)         # *c = 0
                 inc  r0                  # r0 = 1
                 st   r0, 0x4(r2)         # *(c+1) = 1
                 ld   0x4(r2), r3         # r3 = c[1]
                 st   r3, 0x8(r2)         # c[2] = r3
                 ld   0x8(r2), r3         # r3 = *(c+2)
                 st   r3, 0xc(r2)         # *(c+3) = r3
                 mov  r2, r5              # r5 = c
                 inca r5                  # r5 = c + 1*4
                 inca r5                  # r5 = c + 2*4
                 inca r5                  # r5 = c + 3*4
                 st   r5, 0x0(r2)         # *c = (c+3)
                 st   r5, 0x0(r1)         # c = &c[3]
                 ld   0xc(r5), r6         # r6 = c[3]
                 st   r6, 0x0(r5)         # *c = c[3]
                 ld   $0x0, r0            # r0 = 0
                 st   r0, 0x0(r1)         # c = 0
                 halt                     # halt
.pos 0x2000
c:               .long 0x000080b0         # int *c (initialized dynamically)
.pos 0x80b0
c_data:          .long 0xffffffff         # array in heap (allocated dynamically)
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
                 .long 0xffffffff         
