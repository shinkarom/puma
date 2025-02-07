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
	jsr RandomizeX
	jsr RandomizeY
	rts
 
	;---
	;UPDATE
	;---
	
Func_Update: 	
	;clear screen
	Push8 #0
	CallApi #API_cls

	Push8 #0
	CallApi #API_isJustPressed
	Pop8 d0
	tst d0
	beq 1f
	jsr RandomizeX
1:	
	
	Push8 #1
	CallApi #API_isJustPressed
	Pop8 d0
	tst d0
	beq 1f
	jsr RandomizeY
1:	

	;up
	Push8 #17
	CallApi #API_isPressed
	Pop8 d0
	tst d0
	beq 1f
	;if y>0 then y=y-1
	move.b coordy, d0
	cmp.b #0, d0
	beq 1f	
	sub.b #1, d0
	move.b d0, coordy
1:	

	;down
	Push8 #9
	CallApi #API_isPressed
	Pop8 d0
	tst d0
	beq 1f
	;if y<183 then y=y+1
	move.b coordy, d0
	cmp.b #183, d0
	bhi 1f
	add.b #1, d0
	move.b d0, coordy
1:	
	
	;left
	Push8 #8
	CallApi #API_isPressed
	Pop8 d0
	tst d0
	beq 1f
	;if x>0 then x=x-1
	move.b coordx, d0
	cmp.b #0, d0
	beq 1f	
	sub.b #1, d0
	move.b d0, coordx
1:	

	;right
	Push8 #10
	CallApi #API_isPressed
	Pop8 d0
	tst d0
	beq 1f
	;if x<183 then x=x+1
	move.b coordx, d0
	cmp.b #183, d0
	bhi 1f
	add.b #1, d0
	move.b d0, coordx
1:	

	jsr Draw
	rts

	;---
	;OTHER
	;---

Draw:
	Push32 #image
	Push8 coordx
	Push8 coordy
	Push8 #8
	Push8 #8
	Push8 #0x00
	Push8 #0
	CallApi #API_drawSprite
	rts

RandomizeX:
	Push32 #1
	Push32 #184
	CallApi #API_getRandomNumber
	Pop8 d0
	move.b d0, coordx
	
	rts
	
RandomizeY:
	Push32 #1
	Push32 #184
	CallApi #API_getRandomNumber
	Pop8 d0
	move.b d0, coordy
	
	rts
	
	;---
	;DATA
	;---
	
.nolist
	
image: .incbin "ball.bin"

.list

coordx: .byte 0
coordy: .byte 0