# Lab1 Report 
Student ID: PB23151799  
Name: 时分

## Tasks
Create a program to decrypt the secret number with LC-3 machine code.  
The secret number is placed in the register R0. The decryption process is to bitwise XOR the secret number with a secret key, which is determined by student ID (which is 01111111 or x7F in this case), and put the result in register R3.

## The Core Algorithm
The algorithm includes two parts:

1. Input the secret key into register R2.
2. Do bitwise XOR between R0 and R2, put the result into register R3.

### Part 1
As x7F is too large to fit into imm5 (used by ADD), it needs to be input by 3 steps:

1. Put x7 into register R2.
2. Shift them left by 4 bits.
3. Put xF into register R2.

The assembly code is as below.
```assembly
AND R2,R2,x0
ADD R2,R2,x7
ADD R2,R2,R2
ADD R2,R2,R2
ADD R2,R2,R2
ADD R2,R2,R2
ADD R2,R2,xf
```
After translating to machine code:
```
0101 010 010 1 00000
0001 010 010 1 00111
0001 010 010 0 00 010
0001 010 010 0 00 010
0001 010 010 0 00 010
0001 010 010 0 00 010
0001 010 010 1 01111
```

### Part 2
A way to realize bitwise XOR A with B is:

1. A + B = C.
2. (A AND B) * 2 = CARRY, CARRY is the carry bits in A+B.
3. C - CARRY = A XOR B

The assembly code is as below:
```assembly
ADD R3,R0,R2
;put A+B in R3
AND R1,R0,R2
ADD R1,R1,R1
;put CARRY in R1 
NOT R1,R1
ADD R1,R1,x1
ADD R3,R1,R3
;put R3-R1 in R3
```
After translating to machine code:
```
0001 011 000 0 00 010
0101 001 000 0 00 010
0001 001 001 0 00 001
1001 001 001 111111
0001 001 001 1 00001
0001 011 001 0 00 011
```
## Run Results
Input 1: xFFFF  
Output 1: xFF80  

Input 2: x0000  
Output 2: x007F  

Input 3: xFF80  
Output 3: xFFFF  

Input 4: x1020  
Output 4: x105F

Input 5: x2024  
Output 5: x205B

All 5 tests passed, no bug found.

## Suggestions
The bitwise XOR can also be realised by logical operations, as  
**A XOR B = (A AND B) NOR (A NOR B)**
