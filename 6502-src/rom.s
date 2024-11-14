	;;  ROM File

	.org 	$E000
	.byte	$00		; forces beginning of file
	
	.org	$FFFC
	.word	$8000 		; reset vector
	.word	$0000		;
