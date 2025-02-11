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
	jsr RandomizeBox
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
	jsr RandomizeY
1:	

	Push8 #1
	CallApi #API_isJustPressed
	Pop8 d0
	tst d0
	beq 1f
	jsr RandomizeBox
1:	
	
	move.b overlapx, d0
	tst d0
	bne NoMoveVert

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

NoMoveVert:
	move.b overlapy, d0
	tst d0
	bne NoMoveHorz	
	
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

NoMoveHorz:

	;check overlapy
	move.b coordx, d0
	move.b boxx, d1
	add.b #1, d1
	cmp d1, d0
	beq 2f
	move.b #0, overlapy
	bra 1f
2:
	move.b #1, overlapy
1:	
	;check overlapx
	move.b coordy, d0
	move.b boxy, d1
	add.b #1, d1
	cmp d1, d0
	beq 2f
	move.b #0, overlapx
	bra 1f
2:
	move.b #1, overlapx
1:	
	;check overlap
	move.b coordx, d0
	move.b boxx, d1
	add.b #1, d1
	cmp d1, d0
	bne 3f
2:
	move.b coordy, d0
	move.b boxy, d1
	add.b #1, d1
	cmp d1, d0
	bne 3f
	move.b #1, overlap
	bra 1f
3:	
	move.b #0, overlap
1:	

	jsr Draw
	rts

	;---
	;OTHER
	;---

Draw:
	move.b #190, d0
	move.b coordy, d1
	add.b #3, d1
1:	
	move.b overlapx, d2
	tst d2
	bne 2f
	Push32 #redrect
	bra 3f
2:
	Push32 #rect
3:
	Push8 d0
	Push8 d1
	Push8 #2
	Push8 #2
	Push8 #0x00
	Push8 #0
	CallApi #API_drawSprite
	
	sub.b #2, d0
	cmp.b #0xFE, d0
	bne 1b
	
	move.b coordx, d0
	add.b #3, d0
	move.b #190, d1
1:	
	move.b overlapy, d2
	tst d2
	bne 2f
	Push32 #redrect
	bra 3f
2:
	Push32 #rect
3:
	Push8 d0
	Push8 d1
	Push8 #2
	Push8 #2
	Push8 #0x00
	Push8 #0
	CallApi #API_drawSprite
	
	sub.b #2, d1
	cmp.b #0xFE, d1
	bne 1b
	
	move.b overlap, d0
	tst d0
	beq 2f
	Push32 #yellowbox
	bra 1f
2:
	Push32 #bluebox
1:
	Push8 boxx
	Push8 boxy
	Push8 #10
	Push8 #10
	Push8 #0x00
	Push8 #0
	CallApi #API_drawSprite
	
	Push32 #ball
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

RandomizeBox:
	Push32 #0
	Push32 #181
	CallApi #API_getRandomNumber
	Pop8 d0
	move.b d0, boxx
	
	Push32 #0
	Push32 #181
	CallApi #API_getRandomNumber
	Pop8 d0
	move.b d0, boxy
	rts
	
	;---
	;DATA
	;---
	
.nolist
	
ball: .incbin "ball.bin"
rect: .incbin "rect.bin"
redrect: .incbin "redrect.bin"
bluebox: .incbin "bluebox.bin"
yellowbox: .incbin "yellowbox.bin"

.list

coordx: .byte 0
coordy: .byte 0
boxx: .byte 0
boxy: .byte 0
overlapx: .byte 0
overlapy: .byte 0
overlap: .byte 0