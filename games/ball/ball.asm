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
	
	;update dx
	move.b coordx, d0
	tst d0
	beq 1f
	cmp.b #184, d0
	beq 2f
	bra 3f
1:
	move.b #1, movedx
	bra 3f
2:
	move.b #-1,movedx	
3:	
	;update dy
	move.b coordy, d0
	tst d0
	beq 1f
	cmp.b #184, d0
	beq 2f
	bra 3f
1:
	move.b #1, movedy
	bra 3f
2:
	move.b #-1,movedy	
3:	
	;add dx, dy
	move.b coordx, d0
	add.b movedx,d0
	move.b d0, coordx
	move.b coordy, d0
	add.b movedy,d0
	move.b d0, coordy

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
	
	Push32 #0
	Push32 #99
	CallApi #API_getRandomNumber
	Pop8 d0
	cmpi.b #50, d0
	bpl 1f
	move.b #-1, movedx
	bra 2f
1:
	move.b #1, movedx
2:
	rts
	
RandomizeY:
	Push32 #1
	Push32 #184
	CallApi #API_getRandomNumber
	Pop8 d0
	move.b d0, coordy
	
	Push32 #0
	Push32 #99
	CallApi #API_getRandomNumber
	Pop8 d0
	cmpi.b #50, d0
	bpl 1f
	move.b #-1, movedy
	bra 2f
1:
	move.b #1, movedy
2:
	rts
	
	;---
	;DATA
	;---
	
.nolist
	
image: .incbin "ball.bin"

.list

coordx: .byte 0
coordy: .byte 0
movedx: .byte 1
movedy: .byte 1