# Lab2 Report

Name: 时分

Student ID: PB23151799

## Propose

Collatz Conjecture's proposition is:

For a given sequence,
$$
a_n = 
\begin{cases}
\frac{a_{n-1}}{2},a_{n-1}\equiv 0 \mod 2 \\
3a_{n-1}+1,a_{n-1}\equiv 1 \mod 2
\end{cases}
$$
The conjecture is: 
$$
\forall a_0 \in N^*,\exist k\in N^*,s.t.a_k=1
$$
The purpose is to find the minimum k for a small number $a_0$ which is stored in x3100. Then save k in x3101. The program is processed in LC-3 system, and it is designed with assembly codes.

## Principles

There are some tasks to be solved:

1. To judge whether $ a_n $ is  1;
2. To judge whether $ a_n $ is even or odd;
3. To realize $ 3*a_{n-1} $;
4. To realize $ \frac{a_{n-1}}{2} $.

To find whether $a_n=1$, we can judge whether  $a_n+(-1)=0$ . The assembly code is:

```assembly
ADD R2,R0,x-1
BRZ OVER
```

If $a_n$ is even, the last bit of it is 0; if $a_n$ is odd, the last bit is 1. So we can judge whether $a_n$ AND 1 = 0. The assembly code is:

```assembly
AND R2,R0,x1
BRZ EVEN
```

Multiplication is easy to realize by ADD several times. The assembly code to realize R0 = R0*3+1 is:

```assembly
ADD R2,R0,R0
;R2 = 2*R0
ADD R0,R2,R0
;R0 = 3*R0
ADD R0,R0,x1
;R0 = 3*R0 + 1
```

Division can be realized by doing subtraction and counting times it needs. To realize R0 = R0/2 (R0 stores an even number), we can repeat R0 = R0-2  until R0=0, and record times we repeated.The assembly code is:

```assembly
 LOOP:
 	ADD R2,R2,x1
	ADD R0,R0,x-2
	;R0=R0-2,R2++
	BRP LOOP
    ;R2=R0/2
    ADD R0,R2,x0
```

## Procedure

The bugs encountered and solutions are as below:

Bug1: The division loop cannot stop.

Solution: BRP should be put just after ADD R0,R0,x-2, but not ADD R2,R2,x1.

Bug1: The data in R1 (k) cannot be stored in x3101 correctly.

Solution: When using PC-relative way (ST) to store, Pcoffset9 should be x3101 - (PC+1).

It's easier to store by the indirect way (STI).

## Result

1. x3100: 12

   x3101: 9

2. x3100: 14

   x3101: 17

3. x3100: 34

   x3101: 13

4. x3100: 56

   x3101: 19

5. x3100: 32

   x3101: 5

All results are correct.