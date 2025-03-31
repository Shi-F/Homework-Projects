# Lab3 Report

Name: 时分

Student ID: PB23151799

## Propose

A palindromic string is a string that reads the same forwards and backwards, such as "level" or "madam". Given a string $S$ and its length $N$, the purpose is to determine if it is a palindromic string. The program is processed in LC-3 system, and it is designed with assembly codes.

$N$ will be stored in x3100, and $S$ will be stored in successive memory locations starting with address x3101. If $S$ is a palindromic string, the result is 1; otherwise the result is 0. Store the result in x3200. R0-R7 are set to 0 at the beginning.

## Principles

I use 2 pointers (R1, R2) in the process. At first, R1 points the start of the string, and R2 points the end of the string. The initialization code is:

```assembly
 	LDI R0,LENGTH
    LD R1,STRING
    ;R1 is the pointer of the start of string
    ADD R2,R0,R1
    ADD R2,R2,#-1
    ;R2 is the pointer of the end of string
```

In each loop, determine if M[R1] equals M[R2]. If not, the program stops looping and goes to FALSE; then R1 moves forwards, and R2 moves backwards. The loop ends when R1 >= R2, then the program goes to TRUE. The assembly code is:

```assembly
LOOP
    NOT R6,R2
    ADD R6,R6,#1
    ADD R6,R6,R1
    BRzp TRUE
    ;if R1 >= R2 Goto TRUE
    LDR R3,R1,#0
    LDR R4,R2,#0
    NOT R6,R4
    ADD R6,R6,#1
    ADD R6,R6,R3
    BRnp FALSE
    ;if M[R1] != M[R2] Goto FALSE
    ADD R1,R1,#1
    ADD R2,R2,#-1
    ;move the pointer
    BR LOOP
```

At first, R5 = 0. If the result is TRUE, set R5 to 1. Then store R5 in x3200. The assembly code is:

```assembly
TRUE
    ADD R5,R5,#1
FALSE
    STI R5,RESULT
```

## Procedure

The result is wrong when running the second time. This bug is fixed by reset R0 - R7 each time.

Another way to realize the determining process is to use a stack. Put the former half of the string in the stack. Then compare the back half of the string and characters in the stack. However, this method needs to calculate $N/2$ and needs more memory for a stack, so I didn't choose it.

## Result

1. x3100: 5

   x3101 - x3105: "abcba"

   x3200: 1

2. x3100: 13

   x3101 - x3113: "aBaDCDEDCDaBa"

   x3200: 1

3. x3100: 14

   x3101 - x3114: "aBaDCDEfDCDaBa"

   x3200: 0

4. x3100: 1

   x3101: a

   x3200: 1

5. x3100: 0

   x3200: 1

All results are correct.