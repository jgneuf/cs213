# .pos is a pseudonym for .address
.pos 0x0
start:           ld   $0x2028, r5       # initialize sp         
                 gpc  $6, r6            # set return address
                 j    0x300             # bar()
                 halt                     
.pos 0x100
a:               .long 0x00001000         
.pos 0x200
foo:             ld   0x0(r5), r0       # r0 = arg0  
                 ld   0x4(r5), r1       # r1 = arg1 
                 ld   $0x100, r2        # r2 = a
                 ld   0x0(r2), r2       # r2 = &a
                 ld   (r2, r1, 4), r3   # r3 = a[arg1]  
                 add  r3, r0            # r3 = a[arg1] + arg0
                 st   r0, (r2, r1, 4)   # a[arg1] = r3
                 j    0x0(r6)           # return
.pos 0x300
bar:             ld   $0xfffffff4, r0   # r0 = -12
                 add  r0, r5            # push 3 elements to stack
                 st   r6, 0x8(r5)       # push return address
                 ld   $0x1, r0          # r0   = 1
                 st   r0, 0x0(r5)       # arg0 = 1
                 ld   $0x2, r0          # r0   = 2
                 st   r0, 0x4(r5)       # arg1 = 2
                 ld   $0xfffffff8, r0   # r0   = -8
                 add  r0, r5            # push 2 elements to stack
                 ld   $0x3, r0          # r0   = 3
                 st   r0, 0x0(r5)       # arg0 = 3
                 ld   $0x4, r0          # r0   = 4
                 st   r0, 0x4(r5)       # arg1 = 4
                 gpc  $6, r6            # set return address
                 j    0x200             # foo(arg0, arg1)
                 ld   $0x8, r0          # r0   = 8
                 add  r0, r5            # pop 2 elements off stack
                 ld   0x0(r5), r1       # r1   = arg0
                 ld   0x4(r5), r2       # r2   = arg1
                 ld   $0xfffffff8, r0   # r0   = -8  
                 add  r0, r5            # push 2 elements to stack
                 st   r1, 0x0(r5)       # r1   = arg0 
                 st   r2, 0x4(r5)       # r2   = arg1
                 gpc  $6, r6            # set return address
                 j    0x200             # foo(arg0, arg1)
                 ld   $0x8, r0          # r0   = 8
                 add  r0, r5            # pop 2 elements off stack
                 ld   0x8(r5), r6       # ra   = return address
                 ld   $0xc, r0            
                 add  r0, r5              
                 j    0x0(r6)             
.pos 0x1000
arr:             .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
.pos 0x2000
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
