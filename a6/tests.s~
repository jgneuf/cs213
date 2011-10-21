.pos 0x100
            ld $f, r0       # r0 = f
            j  *0(r0)       # jump to 0x200
            halt
.pos 0x200
            ld $g, r0       # r0 = g
            j  *4(r0)       # jump to 0x250
            halt
.pos 0x250
            ld $0x2, r1
            j  *(r0,r1,4)   # jump to 0x300
            halt
.pos 0x300
            inc r1          # offset++
            j   *(r0,r1,4)  # jump to 0x350
            halt
.pos 0x350
            inc r1          # offset++
            j   *(r0,r1,4)  # jump to 0x400
            halt
.pos 0x400
            halt
.pos 0x1000
f:          .long 0x200     # jump to 0x200
g:          .long 0xfff     # test offset
            .long 0x250     # jump to 0x250
            .long 0x300     # jump to 0x300 
            .long 0x350     # skip this
            .long 0x400     # jump to 0x400
