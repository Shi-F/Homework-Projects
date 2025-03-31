.ORIG x3000
            JSR     PRINTSTART
            LD      R1, ASCII_LY    ;R1 = -x79 = -'y'
            AND     R2, R2, #0      ;R2 stores the state
            AND     R3, R3, #0      ;R3 counts 1010
LOOP        GETC
            OUT
            ADD     R5, R0, R1    
            BRz     DONE            ;if input = 'y', goto DONE
            ADD     R5, R0, R2
            AND     R5, R5, #1
            BRz     RESET           ;if state + input is even, goto RESET
            ADD     R5, R2, #-4
            BRnp    SKIP
            ADD     R2, R2, #-2     ;if state = 4 && input = 1, R2 = 3
SKIP        ADD     R2, R2, #1      ;if state + input is odd, R2++
            BRnzp   TEST4
RESET       AND     R2, R0, #1
TEST4       ADD     R5, R2, #-4
            BRnp    NOT4            ;if state = 4, R3++
            ADD     R3, R3, #1
NOT4        BRnzp   LOOP
DONE        JSR     PRINTRESULT
            HALT
ASCII_LY    .FILL   xFF87
;
PRINTSTART
            LEA     R0, TEXT0
            PUTS
            RET
TEXT0       .STRINGZ "SD is ready!Please input your number:"
;
PRINTRESULT
            AND     R0, R0, #0
            ADD     R0, R0, xA
            OUT                     ;print '\n'
            ST      R7, SAVE_R7
            LEA     R0, TEXT1
            PUTS                    ;print "There are(is) "
            JSR     BINARYTOASCII   ;transform the binary number in R3 to ASCII string
            PUTS
            LEA     R0, TEXT2
            PUTS                    ;print " 1010 in the sequence!"
            LD      R7, SAVE_R7
            RET
TEXT1       .STRINGZ "There are(is) "
TEXT2       .STRINGZ " 1010 in the sequence!"
SAVE_R7     .BLKW   1
;
BINARYTOASCII
            LEA     R0, ASCIIBUFF
            LEA     R1, ASCIIBUFF    ;R1 is the pointer
            AND     R5, R5, #0       ;R5 counts the digit, R5 = 4 at first
            ADD     R5, R5, #4
LOOPA       AND     R4, R4, #0       ;R4 = -10 ^ R5
            ADD     R2, R5, #-4
            BRnp    SKIPW
            LD      R4, NEG1W
            BRnzp   STARTA
SKIPW       ADD     R2, R5, #-3
            BRnp    SKIPK
            LD      R4, NEG1K
            BRnzp   STARTA
SKIPK       ADD     R2, R5, #-2
            BRnp    SKIP100
            LD      R4, NEG100
            BRnzp   STARTA
SKIP100     ADD     R2, R5, #-1
            BRnp    SKIP10
            ADD     R4, R4, #-10
            BRnzp   STARTA
SKIP10      ADD     R4, R4, #-1      ;decide R4
STARTA      LD      R2, ASCII_0      ;R2 is the digit
LOOPB       ADD     R3, R3, R4
            BRn     ENDA
            ADD     R2, R2, #1
            BRnzp   LOOPB            ;get the digit
ENDA        STR     R2, R1, #0
            ADD     R1, R1, #1       ;store the digit and move the pointer
            NOT     R4, R4
            ADD     R4, R4, #1       ;R4 = +10 ^ R5
            ADD     R3, R3, R4       ;correct R3 for one-too-many substracts
            LD      R4, ASCII_N0
            ADD     R4, R2, R4
            BRnp    SKIPA            ;if R2 = '0' && R5 != 0, R0++
            ADD     R4, R5, #0
            BRz     SKIPA
            ADD     R0, R0, #1
SKIPA       ADD     R5, R5, #-1      ;move the digit
            BRzp    LOOPA
            RET
ASCII_N0    .FILL   xFFD0
ASCII_0     .FILL   x0030
NEG1W       .FILL   #-10000
NEG1K       .FILL   #-1000
NEG100      .FILL   #-100
ASCIIBUFF   .STRINGZ "00000"
.END