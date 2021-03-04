startwhile: add r0,1,r1
jmp waits,r5      
add r0,3,r1
jmp waits,r5      
add r0,7,r1
jmp waits,r5      
add r0,15,r1
loop: seq r1,0,r2         
seq r2,r0,r2        
braz r2,startwhile
jmp waits,r5      
shl r1,1,r1         
jmp loop,r0
jmp startwhile,r0 
waits: add r0,0,r3    
testloop: seq r3,5,r4     
branz r4,finwait  
add r3,1,r3       
jmp testloop,r0     
finwait: jmp r5,r0   