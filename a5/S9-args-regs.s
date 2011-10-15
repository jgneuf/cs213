.pos 0x100
start:           ld   $0x1028, r5         # initialize stack pointer
                 gpc  $6, r6              # ra = pc + 6
                 j    foo                 # call foo()
                 halt                     
.pos 0x200
foo:             deca r5                  # sp -= 4
                 st   r6, (r5)            # *sp = ra
                 ld   $1, r0              # arg0 = 1
                 ld   $2, r1              # arg1 = 2
                 gpc  $6, r6              # r6 = pc + 6
                 j    add                 # call add (1,2)
                 ld   $s, r1              # r1 = &s
                 st   r0, (r1)            # r1 = add (1,2)
                 ld   (r5), r6            # ra = *sp
                 inca r5                  # sp += 4
                 j    (r6)                # return
.pos 0x300
add:             add  r1, r0              # return value = a + b
                 j    (r6)                # return
.pos 0x280
s:               .long 0x00000000         # s
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
