
	.org $8000
	LDA #$00        ; Load A with low byte of start address (0x0400)
	STA $80         ; Store at $80
	LDA #$04        ; Load A with high byte of start address (0x0400)
	STA $81         ; Store at $81

	LDX #$00        ; X = 0 (not used directly in the loop)
	LDY #$00        ; Y = 0 (offset into memory)

ClearLoop:
	LDA #$58        ; Load A with $55 (the value to store)
	STA ($80),Y     ; Store A at memory location ($80 and $81) + Y
	INY             ; Increment Y
	BNE ClearLoop   ; Loop back if Y has not wrapped to 0 (256 bytes)

	INC $81         ; Increment the high byte of the address ($81)
	LDA $81         ; Load the high byte to compare
	CMP #$06        ; Compare to 6 (0x0600 is the end of the range)
	BNE ClearLoop   ; If not done, repeat the loop

Spin:	
	JMP Spin        ; Return
