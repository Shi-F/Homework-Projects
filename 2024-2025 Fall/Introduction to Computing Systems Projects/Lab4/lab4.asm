.ORIG x3000
        LD R6,STACK
        ADD R1,R1,#10   ;S
        LDI R0,INPUT
        BRz OVER
        ADD R2,R2,#6    ;Earn
        ADD R3,R3,#-2   ;Spend
        JSR FUNC
OVER    STI R1,RESULT
        HALT
FUNC
        ADD R0,R0,#-1
        BRz DONE
        STR R7,R6,#0
        ADD R6,R6,#-1    ;Push R7
        STR R2,R6,#0
        ADD R6,R6,#-1    ;Push Earn(n)
        STR R3,R6,#0
        ADD R6,R6,#-1    ;Push Spend(n)
        ADD R5,R2,R3
        BRnz KEEP       ;if Earn(n) <= Spend(n) goto KEEP
        ADD R5,R3,R3
        ADD R3,R5,R5    ;Spend(n+1) = 4*Spend(n)
        BR NEXT
KEEP    AND R3,R3,#0
        ADD R3,R3,#-2   ;Spend(n+1) = 2
NEXT    ADD R2,R2,R2    ;Earn(n+1) = 2*Earn(n)
        JSR FUNC
        ADD R6,R6,#1
        LDR R3,R6,#0    ;Pop Spend(n)
        ADD R6,R6,#1
        LDR R2,R6,#0    ;Pop Earn(n)
        ADD R6,R6,#1
        LDR R7,R6,#0    ;Pop R7
DONE    ADD R1,R1,R3
        ADD R1,R1,R2
        RET
STACK   .FILL x6000
INPUT   .FILL x3100
RESULT  .FILL x3200
.END