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
            deca r5                 # dst[1]
            deca r5                 # dst[0]
            ld   $0x0,      r1      # r1 = i = 0
            ld   $src,      r2      # r2 = &src
while:      ld   (r2,r1,4), r3      # r3 = src[i]
            st   r3, (r5,r1,4)      # dst[i] = src[i]
            beq  r3,        end     # while (src[i] != 0)
            inc  r1                 # i++
            j    while              # next iteration
end:        inca r5                 # pop dst[1]
            inca r5                 # pop dst[0]
            ld   (r5),      r6      # ra  = *sp
            inca r5                 # ra += 4, pop
            j    (r6)               # return
.pos 0x900
src:        .long 0x00000001
            .long 0x00000001
            .long 0x910
            .long 0x00000000
            .long 0x0000ffff
            .long 0xffff0100
            .long 0xffffffff
            .long 0x0200ffff
            .long 0xffff0300
            .long 0xffffffff
            .long 0x0400ffff
            .long 0xffff0500
            .long 0xffffffff
            .long 0x0600ffff
            .long 0xffff0700
            .long 0xffffffff
            .long 0xf0000000
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

