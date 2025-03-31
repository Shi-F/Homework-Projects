.ORIG x3000
    LDI R0,LENGTH
    LD R1,STRING
    ;R1 is the pointer of the start of string
    ADD R2,R0,R1
    ADD R2,R2,#-1
    ;R2 is the pointer of the end of string
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
TRUE
    ADD R5,R5,#1
FALSE
    STI R5,RESULT
    HALT
LENGTH .FILL x3100
STRING .FILL x3101
RESULT .FILL x3200
    .END