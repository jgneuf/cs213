.address 0x0
                 ld   $0x1028, r5        # initialize sp 
                 ld   $0xfffffff4, r0    # r0 = -12 
                 add  r0, r5             # sp -= 12 
                 ld   $0x200, r0         # r0 = &a
                 ld   0x0(r0), r0        # r0 = a 
                 st   r0, 0x0(r5)        # arg0 = a
                 ld   $0x204, r0         # r0 = &b 
                 ld   0x0(r0), r0        # r0 = b 
                 st   r0, 0x4(r5)        # arg1 = b 
                 ld   $0x208, r0         # r0 = &c 
                 ld   0x0(r0), r0        # r0 = c 
                 st   r0, 0x8(r5)        # arg2 = c 
                 gpc  $6, r6             # save ra                  
                 j    0x300              # foo(a,b,c)             
                 ld   $0x20c, r1          
                 st   r0, 0x0(r1)         
                 halt                     
.address 0x200
arg0:            .long 0x000000ff        
arg1:            .long 0x00000002         
arg2:            .long 0x00000005         
result:          .long 0x00000000         
.address 0x300
foo:             ld   0x0(r5), r0        # r0 = arg0 
                 ld   0x4(r5), r1        # r1 = arg1 
                 ld   0x8(r5), r2        # r2 = arg2 
                 ld   $0xfffffff6, r3    # r3 = -10
                 add  r3, r0             # r0 = arg0 - 10
                 mov  r0, r3             # r3 = arg0 - 10 
                 not  r3                 
                 inc  r3                 # r3 = r3 * -1 
                 bgt  r3, L6             # if arg0 < 10 GOTO L6
                 mov  r0, r3             # r3 = arg0 - 10 
                 ld   $0xfffffff8, r4    # r4 = -8 
                 add  r4, r3             # r3 = arg0 - 18 
                 bgt  r3, L6             # if arg0 > 18 GOTO L6              
                 ld   $0x400, r3         # switch setup 
                 j    *(r3, r0, 4)       # switch (arg0 - 10) 
.address 0x330
                 add  r1, r2              
                 br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 bgt  r2, L0              
                 ld   $0x0, r2            
                 br   L1                  
L0:              ld   $0x1, r2            
L1:              br   L7                  
                 not  r1                  
                 inc  r1                  
                 add  r2, r1              
                 bgt  r1, L2              
                 ld   $0x0, r2            
                 br   L3                  
L2:              ld   $0x1, r2            
L3:              br   L7                  
                 not  r2                  
                 inc  r2                  
                 add  r1, r2              
                 beq  r2, L4              
                 ld   $0x0, r2            
                 br   L5                  
L4:              ld   $0x1, r2            
L5:              br   L7                  
L6:              ld   $0x0, r2            
                 br   L7                  
L7:              mov  r2, r0              
                 j    0x0(r6)             
.address 0x400
s10:        .long 0x00000330         
s11:             .long 0x00000384         
s12:             .long 0x00000334         
s13:             .long 0x00000384         
s14:             .long 0x0000033c         
s15:             .long 0x00000384         
s16:             .long 0x00000354         
s17:             .long 0x00000384         
s18:             .long 0x0000036c         
.address 0x1000
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
