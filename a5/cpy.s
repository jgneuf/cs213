.pos 0x100
start:
            ld  $0x1028,    r5      # initialize stack pointer
            gpc $6,         r6      # ra = pc + 6
            j   main                # begin program
            halt
.pos 0x200
main:
            deca r5                 # sp -= 4, push
            st   r6,        (r5)    # *sp = ra
            gpc  $6,        r6      # r6  = pc + 6
            j    copy               # call copy()
            ld   (r5),      r6      # ra  = *sp
            inca r5                 # ra += 4, pop
            j    (r6)               # return
.pos 0x300
copy:
            deca r5                 # sp -= 4, push
            st   r6,        (r5)    # *sp = ra
            ld   (r5),      r6      # ra  = *sp
            inca r5                 # ra += 4, pop
            j    (r6)               # return
.pos 0x900
src:        .long 0x00000001
            .long 0x00000000
.pos 0x1000
stack:      .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000
            .long 0x00000000

