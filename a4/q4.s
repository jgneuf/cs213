.pos 0x100
            ld   $i, r0             # r0 = &i
            ld   0(r0), r0          # r0 = i = 0
            ld   $a, r1             # r1 = address of a[0]
            ld   $min, r2           # r2 = &min
            ld   0(r2), r2          # r2 = min
bar:
            ld  $foo, r3            # r3 = address of bar
            j   0(r3)               # bar ()
foo:      
            ld   $0xfffffff6, r4    # r4 = -10
loop:       mov  r0, r5             # r5 = i
            add  r4, r5             # r5 = i - 10
            beq  r5, end_loop       # if i == 10 done loop
            ld   (r1, r0, 4), r3    # r3 = a[i]
            mov  r2, r6             # r6 = min
            not  r6                 # ~r6
            inc  r6                 # r6 = -i
            add  r3, r6             # r6 = min - a[i]
            inc  r0                 # i++
            bgt  r6, loop           # if min > a[i] continue
            mov  r3, r2             # min = a[i]
            br   loop               # next iteration
end_loop:   ld   $min, r1           # r1 = address of a[0]
            st   r2, 0(r1)          # min = min
            halt
.pos 0x1000
min:        .long 0x000f4240        # min
i:          .long 0x00000000        # i
a:          .long 0x00000002        # a[0]
            .long 0x00000028        # a[1]
            .long 0x00000006        # a[2]
            .long 0x00000050        # a[3]
            .long 0x0000000a        # a[4]
            .long 0x00000078        # a[5]
            .long 0x0000000e        # a[6]
            .long 0x00000010        # a[7]
            .long 0x00000030        # a[8]
            .long 0x00000014        # a[9]

