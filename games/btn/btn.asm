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
	
	move.b #0, d0
btnloop:	
	
	
MaybeJustPressed:	
	Push8 d0
	CallApi #API_isJustPressed
	Pop8 d1
	tst d1
	beq MaybeJustReleased
	Push32 #btnjp
	
	move.b d0, d1
	add.b #60, d1
	Push8 #0x00
	Push8 d1
	CallApi #API_writeAudioRegister
	
	bra Done
MaybeJustReleased:	
	Push8 d0
	CallApi #API_isJustReleased
	Pop8 d1
	tst d1
	beq MaybeSimplyPressed
	Push32 #btnjr
	
	Push8 #0x00
	Push8 #0
	CallApi #API_writeAudioRegister
	
	bra Done
MaybeSimplyPressed:
	Push8 d0
	CallApi #API_isPressed
	Pop8 d1
	tst d1
	beq NotPressed
	Push32 #btnp
	bra Done	
NotPressed:
	Push32 #btn
Done:
	
	;x
	clr d1
	move.b d0, d1
	divu #8, d1
	swap d1
	and.l #0xFFFF, d1
	mulu #24,d1
	Push8 d1
	
	;y
	clr d1
	move.b d0, d1
	divu #8, d1
	and.l #0xFFFF, d1
	sub.b #3,d1
	neg.b d1
	mulu #48,d1
	Push8 d1
	
	Push8 #24
	Push8 #48	
	Push8 #0
	Push8 #0
	CallApi #API_drawSprite	
	add.b #1, d0
	cmp.b #32, d0
	bne btnloop
	
	rts

	;---
	;OTHER
	;---
	
	;---
	;DATA
	;---
	
btn: .incbin "btn.bin"
btnp: .incbin "btnp.bin"
btnjp: .incbin "btnjp.bin"
btnjr: .incbin "btnjr.bin"