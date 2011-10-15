# .pos is a pseudonym for .address
.pos 0x0
                 ld   $0x0, r0            
                 not  r0                  
                 ld   $0x1000, r1         
                 ld   0x0(r1), r1         
                 ld   $0x1004, r2         
                 ld   0x0(r2), r2         
                 not  r2                  
                 inc  r2                  
                 beq  r2, 0x34            
                 ld   0x0(r1), r3         
                 mov  r0, r4              
                 not  r4                  
                 inc  r4                  
                 add  r3, r4              
                 bgt  r4, 0x2c            
                 br   0x2e                
                 mov  r3, r0              
                 inca r1                  
                 inc  r2                  
                 br   0x1c                
                 ld   $0x1008, r1         
                 st   r0, 0x0(r1)         
                 halt                     
.pos 0x1000
                 .long 0x00002000         
                 .long 0x00000005         
                 .long 0x00000000         
.pos 0x2000
                 .long 0x00000002         
                 .long 0x00000004         
                 .long 0x00000010         
                 .long 0x00000007         
                 .long 0x00000001         
