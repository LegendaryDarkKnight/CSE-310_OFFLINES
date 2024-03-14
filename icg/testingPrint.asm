.MODEL SMALL
.STACK 1000H
.DATA
    number DB "00000$"
    
main PROC
      MOV AX, 5656
      call print_output
      call print_newline
      MOV AX, 5656
      call print_output
      call print_newline
      MOV AH, 4CH
      INT 21H
main ENDP
print_newline PROC NEAR
    PUSH BP
    MOV BP, SP 
    MOV BX, 0
    MOV DL, 10
    MOV AH, 2
    INT 21H
    MOV DL, 13
    MOV AH, 2
    INT 21H
    POP BP
    RET 0 
print_output PROC NEAR
    PUSH BP
    MOV BP, SP 
    MOV BX, 0
PRINT_1:
    MOV DX, 0
    MOV CX, 10
    DIV CX
    PUSH DX
    INC BX
    CMP AX, 0
    JG PRINT_1
PRINT_2:
    POP DX
    ADD DX, 30H
    DEC BX
    MOV AL, 0 
    MOV AH, 2
    INT 21H
    CMP BX, 0
    JG PRINT_2
    POP BP
    RET 0 
;print_output PROC FAR
;    PUSH BP
;    MOV BP, SP
;    CMP AX, 0
;    JBE RETURN
;    MOV DX, 0
;    MOV CX, 10
;    DIV CX
;    PUSH DX
;    CALL print_output
;    MOV DX, WORD PTR[BP-2]
;    ADD DX, 30H 
;    MOV AL, 0 
;    MOV AH, 2
;    INT 21H    
;RETURN:    
;    POP BP
;    RET 2
END main