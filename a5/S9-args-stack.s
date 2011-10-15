.pos 0x100
start:           ld   $0x1028, r5         # initialize stack pointer
                 gpc  $6, r6              # ra = pc + 6
                 j    foo                 # call foo()
halt                     
.pos 0x200
foo:             deca r5                  # sp -= 4
                 st   r6, (r5)            # *sp = ra
                 ld   $2, r0              # r0 = 2
                 deca r5                  # sp -=4 for arg1
                 st   r0, (r5)            # arg1 = 2
                 ld   $1, r0              # r0 = 1
                 deca r5                  # sp -=4 for arg0
                 st   r0, (r5)            # arg0 = 1
                 gpc  $6, r6              # r6 = pc + 6
                 j    add                 # call add (1,2)
                 inca r5                  # sp += 4 to discard arg0
                 inca r5                  # sp += 4 to discard arg1
                 ld   $s, r1              # r1 = &s
                 st   r0, (r1)            # r1 = add (1,2)
                 ld   (r5), r6            # ra = *sp
                 inca r5                  # sp += 4 to discard r5
                 j    (r6)                # return
.pos 0x300
add:             ld   0(r5), r0           # r0 = arg0
                 ld   4(r5), r1           # r1 = arg1
                 add  r1, r0              # return (r0) = a (r0) + b (r1)
                 j    (r6)                # return
.pos 0x400
s:               .long 0x00000000         # s
.pos 0x1000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
