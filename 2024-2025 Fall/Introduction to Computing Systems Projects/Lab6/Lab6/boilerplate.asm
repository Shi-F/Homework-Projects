; Entry point
.ORIG x200

; Loads system stack
LD R6, SystemStack

; Calls your code
LD R0, SetupAddr
JMP R0

SystemStack .FILL x2FFF
SetupAddr .FILL x0800

.END

; Setup program
.ORIG x0800

;Enable keyboard interrupt
LDI R0, KBSR0
LD R1, KBSRMask0
NOT R0, R0
NOT R1, R1
AND R0, R0, R1
NOT R0, R0
;R0 = [KBSR] OR x4000
STI R0, KBSR0
;KBSR[14] = 1

;Setup ISR for keyboard
LD R0, KBISRAddr
STI R0, KBVecAddr

;Setup ISR for trap
LD R0, TrapISRAddr
STI R0, TrapVecAddr

;Calls user program A
LD R0, PSR_A
ADD R6, R6, x-1
STR R0, R6, x0
;Push PSR of program A into the stack
LD R0, PC_A
ADD R6, R6, x-1
STR R0, R6, x0
;Push PC of program A into the stack
RTI

KBSR0 .FILL xFE00
KBSRMask0 .FILL x4000
KBVecAddr .FILL x0180
KBISRAddr .FILL x1000
TrapVecAddr .FILL x0077
TrapISRAddr .FILL x1800
PSR_A .FILL x8002
PC_A .FILL x4090

.END

; Keyboard ISR
.ORIG x1000

;Store R0, R1
ST R0, SAVE1_R0
ST R1, SAVE1_R1

;Get the keycode
LDI R0, KBDR

;If Q is pressed, halt the machine
LD R1, NEG_Q
ADD R1, R0, R1
BRnp SKIP
HALT
SKIP
;Enable keyboard interrupt
LDI R0, KBSR1
LD R1, KBSRMask1
NOT R0, R0
NOT R1, R1
AND R0, R0, R1
NOT R0, R0
;R0 = [KBSR] OR x4000
STI R0, KBSR1
;KBSR[14] = 1

;Restore R0, R1
LD R0, SAVE1_R0
LD R1, SAVE1_R1
RTI

SAVE1_R0 .BLKW 1
SAVE1_R1 .BLKW 1
NEG_Q .FILL x-51
KBSR1 .FILL xFE00
KBSRMask1 .FILL x4000
KBDR .FILL xFE02

.END

; Trap x77 ISR
.ORIG x1800

;Call switch program
LD R7, SwitchProgramAddr
JMP R7

SwitchProgramAddr .FILL x2000
.END

; Switch Program
.ORIG x2000
LD R7, CurrentProgram
BRp SwitchB2A
; A to B
LEA R7, PC_PSR_A
ST R7, PC_PSR_Save
LEA R7, PC_PSR_B
ST R7, PC_PSR_Load

AND R7, R7, x0
ADD R7, R7, x1
ST R7, CurrentProgram

LEA R7, RegA

BR SwitchEnd
SwitchB2A
; B to A
LEA R7, PC_PSR_B
ST R7, PC_PSR_Save
LEA R7, PC_PSR_A
ST R7, PC_PSR_Load

AND R7, R7, x0
ST R7, CurrentProgram

LEA R7, RegB

SwitchEnd

STR R0, R7, x0
STR R1, R7, x1
STR R2, R7, x2
STR R3, R7, x3
STR R4, R7, x4
STR R5, R7, x5

LD R2, PC_PSR_Save

; Pop PC
LDR R7, R6, x0
ADD R6, R6, x1
STR R7, R2, x0

; Pop PSR
LDR R7, R6, x0
ADD R6, R6, x1
STR R7, R2, x1

LD R2, PC_PSR_Load

; Push PSR
LDR R7, R2, x1
ADD R6, R6, x-1
STR R7, R6, x0

; Push PC
LDR R7, R2, x0
ADD R6, R6, x-1
STR R7, R6, x0

LD R7, CurrentProgram
BRz LoadB2A
LEA R7, RegB
BR LoadEnd
LoadB2A
LEA R7, RegA
LoadEnd

LDR R0, R7, x0
LDR R1, R7, x1
LDR R2, R7, x2
LDR R3, R7, x3
LDR R4, R7, x4
LDR R5, R7, x5

RTI

PC_PSR_Save .BLKW 1
PC_PSR_Load .BLKW 1

SaveR0 .BLKW 1
CurrentProgram .FILL x0

RegA .BLKW 8
RegB .BLKW 8

PC_PSR_A
.FILL x4090
.FILL x8002

PC_PSR_B
.FILL x8090
.FILL x8002
.END

; Program A
.ORIG x4090
LOOP_A
LEA R0, MSG_A
PUTS

LD R1, TIMER_LIMIT_A
LOOP_IA
ADD R1, R1, x-1
TRAP x77
BRp LOOP_IA

BR LOOP_A

TIMER_LIMIT_A .FILL x200
MSG_A .STRINGZ "Program A reporting.\n"
.END

; Program B
.ORIG x8090
LOOP_B
LEA R0, MSG_B
PUTS

LD R1, TIMER_LIMIT_B
LOOP_IB
ADD R1, R1, x-1
TRAP x77
BRp LOOP_IB

BR LOOP_B

TIMER_LIMIT_B .FILL x100
MSG_B .STRINGZ "Program B reporting.\n"
.END