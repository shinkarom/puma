.include "puma.asm"

jsr Func_Init

infinite_loop:
	jsr Func_Update
	
	CallApi #API_endFrame		
bra infinite_loop	

	;---
	;INIT
	;---
	
Func_Init:	 

	rts
 
	;---
	;UPDATE
	;---
	
Func_Update: 	

	rts

	;---
	;OTHER
	;---
	
	;---
	;DATA
	;---
	