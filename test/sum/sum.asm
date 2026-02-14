MOV R0, #1  ; const. 1
MOV R1, #10 ; const. 10
MOV R3, #0  ; sum
MOV R4, #1  ; i
ADD R3, R4  ; sum += i
ADD R4, R0  ; i++
MOV R5, R1
LESS R5, R4 ; 10 < i
JZ R5, 251  ; -5 + 256
MOV 0, R3   ; sum -> mem
