;---------------------------------------------------------------------
; cell contents:
; db 1	; base (background)		; offset +0
; db 80	; NPC and walls			; offset +1
; db 0	; route				; offset +2
; db 1	; 1 - redraw cell, 0 - nothing	; offset +3

;---------------------------------------------------------------------
align 4
map_level_0:
 dd 030100h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h
 dd 000000h,001001h,001001h,000001h,000001h,000001h,001001h,001001h,001001h,000000h
 dd 010200h,001001h,000002h,000002h,000000h,000000h,000000h,000000h,001001h,000000h
 dd 000000h,000001h,000002h,001101h,000001h,000001h,000001h,000000h,001001h,000000h
 dd 000000h,000001h,000002h,001101h,000f00h,000000h,000001h,000000h,000001h,000000h
 dd 000000h,000001h,000000h,001101h,000000h,000000h,000001h,000000h,000001h,000000h
 dd 000000h,000001h,000000h,001001h,001001h,000001h,000001h,001100h,000001h,000000h
 dd 000000h,001101h,001100h,000000h,001000h,000000h,000000h,001100h,001101h,000000h
 dd 000000h,001101h,000001h,000001h,000001h,000001h,000001h,000001h,000001h,000003h
 dd 030300h,000000h,000000h,000000h,000000h,020400h,000000h,000000h,000003h,000003h
;---------------------------------------------------------------------
align 4
map_level_game_over:
 dd 000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h
 dd 000000h,000002h,000002h,000000h,000000h,000002h,000000h,000002h,000002h,000000h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h
 dd 000000h,000002h,000002h,000000h,000000h,000002h,000000h,000002h,000002h,000000h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000000h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000000h
 dd 000000h,000002h,000000h,000002h,000000h,000002h,000000h,000002h,000000h,000000h
 dd 000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h,000000h
;---------------------------------------------------------------------