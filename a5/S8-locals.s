.pos 0x100
start:           ld   $0x1028, r5         # initialize stack pointer
                 gpc  $6, r6              # r6 = pc + 6
                 j    foo                 # call foo()
                 halt                     
.pos 0x200
foo:             deca r5                  # sp -= 4
                 st   r6, (r5)            # *sp = ra
                 gpc  $6, r6              # r6 = pc + 6
                 j    b                   # call b()
                 ld   (r5), r6            # ra = *sp
                 inca r5                  # sp+=4
                 j    (r6)                # return
.pos 0x300
b:               deca r5                  # sp -= 4 for ra
                 st   r6, (r5)            # *sp = ra
                 deca r5                  # sp -= 4 for l1
                 deca r5                  # sp -= 4 for l0
                 ld   $0, r0              # r0 = 0
                 st   r0, 0x0(r5)         # l0 = 0
                 ld   $0x1, r0            # r0 = 1
                 st   r0, 0x4(r5)         # l1 = 1
                 inca r5                  # sp +=4 to discard l0
                 inca r5                  # sp +-4 to discard l1
                 ld   (r5), r6            # ra = *sp
                 inca r5                  # sp += 4 to discard ra
                 j    (r6)                # return
.pos 0x1000
stack:           .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
