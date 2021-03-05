add r0,1,r1
label2: add r1,-30,r2
jmp label1,r0
sub r2,r1,r3
label1: mul r3,2,r4
scall 0
scall 1
braz r1,label2
store r10,0,r1
load r9,0,r10
stop
