.nolist
	mc68040
	.org 0x200
	.set API_stackAddress, 0xFFFFFFF9
	.macro CallApi value
		move.l \value, 0xFFFFFFFC
	.endm

	.macro Push8 value
		move.b \value, API_stackAddress
	.endm

	.macro Push16 value
		move.w \value, API_stackAddress
	.endm

	.macro Push32 value
		move.l \value, API_stackAddress
	.endm

	.macro Pop8 value
		move.b API_stackAddress, \value
	.endm

	.macro Pop16 value
		move.w API_stackAddress, \value
	.endm

	.macro Pop32 value
		move.l API_stackAddress, \value
	.endm
	
	.set API_endFrame, 0
	.set API_writeAudioRegister, 1
	.set API_getRandomNumber, 2
	.set API_isPressed, 3
	.set API_isJustPressed, 4
	.set API_isJustReleased, 5
	.set API_cls, 6
	.set API_drawSprite,7
	.set API_printRegisters, 254
	.set API_printStack, 255
.list