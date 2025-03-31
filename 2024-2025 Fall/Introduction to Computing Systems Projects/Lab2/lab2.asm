.ORIG x3000
;LD R0,xFF
LDI R0,AZERO
AND R1,R1,x0
JUDGE
    ADD R2,R0,x-1
    BRZ OVER
    ;if R0=1:OVER
    
    AND R2,R0,x1
    BRZ EVEN
    
    ;if R0 is ODD:
    ADD R2,R0,R0
    ADD R0,R2,R0
    ADD R0,R0,x1
    ;R0=R0*3+1
    ADD R1,R1,x1
    ;R1++
    BR JUDGE
    
EVEN
    ;if R0 is EVEN:
    LOOP
        ADD R2,R2,x1
        ADD R0,R0,x-2
        ;R0=R0-2,R2++
        BRP LOOP
    ;R2=R0/2
    ADD R0,R2,x0
    ;R0=R0/2
    ADD R1,R1,x1
    ;R1++
    BR JUDGE
    
OVER
    STI R1,RESULT
    ;ST R1,xEF
    TRAP x25
AZERO .FILL x3100
RESULT .FILL x3101
.END


    
