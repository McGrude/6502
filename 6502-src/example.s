
	.org $8000
	LDA #$00        ; Load A with low byte of start address (0x0400)
	STA $80         ; Store at $80
	LDA #$04        ; Load A with high byte of start address (0x0400)
	STA $81         ; Store at $81

	LDX #$20        ; X = 20 (ascii space)
	LDY #$00        ; Y = 0 (offset into memory)

	TXA 	        ; Transfer X to A
	
Loop:
	STA ($80),Y     ; Store A at memory location ($80 and $81) + Y
	JSR New_A	; 
	INY             ; Increment Y
	BNE Loop   	; Loop back if Y has not wrapped to 0 (256 bytes)
	INC $81         ; Increment the high byte of the address ($81)
	LDA $81         ; Load the high byte to compare
	CMP #$06        ; Compare to 6 (0x0600 is the end of the range)
	BNE Loop  	 ; If not done, repeat the loop


New_A:
	TXA		; Copy X to A
	CMP #$7E	; CMP to ascii value of ~
	BNE Skip	; if the same then reset X to $20
	LDX #$1F
Skip:
	INX
	TXA
	RTS
	
	
	
Spin:	
	JMP Spin        ; Return
