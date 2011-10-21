.pos 0x100
start:    ld   $0x2028, r5  # r5 = bottom of stack
          gpc  $6, r6       # ra = pc + 6
          j    bar          # call bar
          halt
.pos 0x200
bar:      deca r5           # sp -= 4 for ra
          st   r6, (r5)     # *sp = ra
          deca r5           # sp -= 4 for arg0
          ld   $a, r0       # r0 = &a
          ld   (r0), r0     # r0 = a
          st   r0, (r5)     # arg0 = a
          gpc  $6, r6       # r6 = pc
          j    foo          # call foo (a)
          inca r5           # sp +=4 to discard arg0
          ld   $b, r0       # r0 = &b
          ld   (r0), r0     # r0 = b
          deca r5           # sp -= 4 for arg0
          st   r0, (r5)     # arg0 = b
          gpc  $6, r6       # ra = pc + 6
          j    foo          # foo (b)
          inca r5           # sp += 4 to discard arg0
          ld   (r5), r6     # ra = *sp
          inca r5           # sp += 4 to discard ra
          j    (r6)         # return
.pos 0x300
foo:      deca r5           # sp += 4 for ra
          st   r6, (r5)     # *sp = ra
          ld   4(r5), r0    # r0 = a (arg0)
          gpc  $2, r6       # ra = pc + 2
          j    *0(r0)       # a->ping ()
          gpc  $2, r6       # ra = pc + 2
          j    *4(r0)       # a->pong ()
          ld   (r5), r6     # ra = *sp
          inca r5           # sp += 4 to discard ra
          j    (r6)         # return

.pos 0x400
A_ping:   j (r6)            # return
A_pong:   j (r6)            # return
B_ping:   j (r6)            # return
B_wiff:   j (r6)            # return


# static variables with snapshot of their dynamic values
.pos 0x500
a:        .long anA         # a: assigned dynamically by new_A
b:        .long aB          # b: assigned dynamically by new_B 

# snapshot of two objects that would be dynamically alloced in heap
.pos 0x1000
anA:      .long A_ping      # allocated dynamically by new_A
          .long A_pong
aB:       .long B_ping      # allocated dynamically by new_B
          .long A_pong
          .long B_wiff

# the runtime stack          
.pos 0x2000
stack:    .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          .long 0
          