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

	Push8 #0
	CallAPI #API_cls

	move.l #font, a0
	move.l #string, a1
	move.b #50, d0 ;x
	move.b #50, d1 ;y
	jsr PrintString
	
	rts

	;---
	;OTHER
	;---
	
;prints a null-terminated string at given coordinates
;a0 = font address (8x8 chars, 32 to 127)
;a1 = string address
;d0 = x coordinate to start printing
;d1 = y coordinate
PrintString:	
	move.l d3, -(sp)
3:		
	cmp.b #191, d0
	bhi 1f
	cmp.b #191, d1
	bhi 1f

2:	
	clr.l d2
	move.b (a1), d2	
	
	move.l a1, d3
	add.b #1, d3
	move.l d3, a1
	
	tst d2
	beq 1f
	cmp.b #32, d2
	blt 2b
	cmp.b #127, d2
	bhi 2b
	
	sub.b #32, d2
	mulu.w #64, d2
	add.l a0, d2	
	
	Push32 d2
	Push8 d0
	Push8 d1
	Push8 #8
	Push8 #8
	Push8 #0
	Push8 #0
	CallAPI #API_drawSprite	
	
	add.b #8, d0
	bra 3b
1:
	move.l (sp)+, d3
	rts
	
	;---
	;DATA
	;---
	
string: .byte "Hello, World!", 0	
font:	.incbin "envious.bin"
