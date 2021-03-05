add r0,3,r10   
mul r10,r10,r11 
add r0,0,r1   
L5: slt r1,r10,r2   
braz r2,Label_end
add r0,0,r2   
L4: slt r2,r10,r8   
braz r8,L1    
add r0,0,r4   
add r0,0,r5   
L2: slt r5,r10,r6   
mul r1,r10,r6   
add r6,r5,r6  
mul r5,r10,r7
add r7,r2,r7  
load r6,r0,r8  
load r7,r0,r9  
mul r8,r9,r8  
add r4,r8,r4  
add r5,1,r5   
jmp L2,r0
L3: mul r1,r10,r6
add r6,r2,r6   
add r6,r11,r6  
store r6,0,r4  
add r2,1,r2    
jmp L4,r0
L1: add r1,1,r1    
jmp L5,r0
Label_end:
stop