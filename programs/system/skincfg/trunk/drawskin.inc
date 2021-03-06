;----------------------------------------------------------------------
area:
  .x      = 5	;345
  .y      = 5	;20
  .width  = 206
  .height = 191
;--------------------------------------
wnd3:
  .x      = area.x+32
  .y      = area.y+35
  .width  = 160
  .height = 100
;--------------------------------------
wnd4:
  .x      = area.x+15
  .y      = area.y+70
  .width  = 160
  .height = 100
;--------------------------------------
virtual at edi+SKIN_PARAMS.dtp.data
	dtp system_colors
end virtual
;----------------------------------------------------------------------
draw_skin:
	mcall	13,<area.x,area.width>,<area.y+2,area.height>,0x00FFFFFF
	mov	ebp,not_packed_area
	mov	edi,[ebp+SKIN_HEADER.params]
	add	edi,ebp
	mpack	ebx,wnd3.x,wnd3.width
	mpack	ecx,wnd3.y,wnd3.height
	mov	edx,[edi+SKIN_PARAMS.colors_1.outer]
	call	draw_framerect
	mpack	ebx,wnd3.x+4,wnd3.width-8
	mpack	ecx,wnd3.y+4,wnd3.height-8
	mov	edx,[edi+SKIN_PARAMS.colors_1.inner]
	call	draw_framerect
	mcall	13,<wnd3.x+1,wnd3.width-2>,<wnd3.y+1,3>,[edi+SKIN_PARAMS.colors_1.frame]
	add	ecx,(wnd3.height-5)*65536
	mcall
	mcall	,<wnd3.x+1,3>,<wnd3.y+1,wnd3.height-2>
	add	ebx,(wnd3.width-5)*65536
	mcall
	mcall	,<wnd3.x+5,wnd3.width-10>,<wnd3.y+5,wnd3.height-10>,dword[dtp.work]

	mov	eax,0x00000001 ; left, inactive
	call	find_bitmap
	mcall	7,,,<wnd3.x,wnd3.y>

	pushd	[ebx-8]
	mov	eax,0x00000003 ; base, inactive
	call	find_bitmap
	pop	edx
	mov	esi,wnd3.x+wnd3.width-1
	sub	esi,edx
	shl	edx,16
	add	edx,wnd3.x*65536+wnd3.y
	mcall	7
;--------------------------------------
@@:
	rol	edx,16
	add	dx,[ebx-8]
	cmp	dx,si
	ja	@f

	rol	edx,16
	mcall	7
	jmp	@b
;--------------------------------------
@@:
	mov	eax,0x00000002 ; oper, inactive
	call	find_bitmap
	mov	edx,ecx
	shr	edx,16
	neg	edx
	shl	edx,16
	add	edx,(wnd3.x+wnd3.width)*65536+wnd3.y
	mcall	7

	mov	ebp,not_packed_area
	mov	edi,[ebp+SKIN_HEADER.params]
	add	edi,ebp
	mov	eax,dword[edi+SKIN_PARAMS.margin.left-2]
	mov	ax,word[edi+SKIN_PARAMS.skin_height]
	sub	ax,[edi+SKIN_PARAMS.margin.bottom]
	shr	ax,1
	add	ax,[edi+SKIN_PARAMS.margin.top]
	add	ax,-9
	push	eax
	lea	ebx,[eax+wnd3.x*65536+wnd3.y]
	mov ecx,dword[dtp.grab_text]
	add ecx,0x10000000
	mcall	4,,,caption_text,caption_text.size
;---------------------------------------------------------
	mov	edi,[ebp+SKIN_HEADER.params]
	add	edi,ebp
	mpack	ebx,wnd4.x,wnd4.width
	mpack	ecx,wnd4.y,wnd4.height
	mov	edx,[edi+SKIN_PARAMS.colors.outer]
	call	draw_framerect
	mpack	ebx,wnd4.x+4,wnd4.width-8
	mpack	ecx,wnd4.y+4,wnd4.height-8
	mov	edx,[edi+SKIN_PARAMS.colors.inner]
	call	draw_framerect
	mcall	13,<wnd4.x+1,wnd4.width-2>,<wnd4.y+1,3>,[edi+SKIN_PARAMS.colors.frame]
	add	ecx,(wnd4.height-5)*65536
	mcall
	mcall	,<wnd4.x+1,3>,<wnd4.y+1,wnd4.height-2>
	add	ebx,(wnd4.width-5)*65536
	mcall
	mcall	,<wnd4.x+5,wnd4.width-10>,<wnd4.y+5,wnd4.height-10>,dword[dtp.work]

	mov	eax,0x00010001 ; left, inactive
	call	find_bitmap
	mcall	7,,,<wnd4.x,wnd4.y>

	pushd	[ebx-8]
	mov	eax,0x00010003 ; base, inactive
	call	find_bitmap
	pop	edx
	mov	esi,wnd4.x+wnd4.width-1
	sub	esi,edx
	shl	edx,16
	add	edx,wnd4.x*65536+wnd4.y
	mcall	7
;--------------------------------------
@@:
	rol	edx,16
	add	dx,[ebx-8]
	cmp	dx,si
	ja	@f

	rol	edx,16
	mcall	7
	jmp	@b
;--------------------------------------
@@:
	mov	eax,0x00010002 ; oper, inactive
	call	find_bitmap
	mov	edx,ecx
	shr	edx,16
	neg	edx
	shl	edx,16
	add	edx,(wnd4.x+wnd4.width)*65536+wnd4.y
	mcall	7

	mov	ebp,not_packed_area
	mov	edi,[ebp+SKIN_HEADER.params]
	add	edi,ebp
	pop	eax
	lea	ebx,[eax+wnd4.x*65536+wnd4.y]
	mov ecx,dword[dtp.grab_text]
	add ecx,0x10000000
	mcall	4,,,caption_text,caption_text.size
;----------------------------------------------------------------------
	mov	edi,[ebp+SKIN_HEADER.buttons]
	add	edi,ebp
;--------------------------------------
.lp1:
	cmp	dword[edi],0
	je	.lp2

	mov	ebx,dword[edi+SKIN_BUTTONS.left-2]
	mov	bx,[edi+SKIN_BUTTONS.width]
	mov	ecx,dword[edi+SKIN_BUTTONS.top-2]
	mov	cx,[edi+SKIN_BUTTONS.height]
	add	ebx,(wnd4.x+wnd4.width)*65536
	add	ecx,wnd4.y*65536
	dec	ebx
	dec	ecx
	mcall	8,,,0x40000000
	add	edi,12
	jmp	.lp1
;--------------------------------------
.lp2:
	mov	edi,[ebp+SKIN_HEADER.params]
	add	edi,ebp
	mpack	ebx,wnd4.x+10,wnd4.y+10
	add	bx,word[edi+SKIN_PARAMS.skin_height]
	mcall	4,,[dtp.work_text],window_text,window_text.size

	mov	ecx,[edi+SKIN_PARAMS.skin_height]
	shl	ecx,16
	add	ecx,(wnd4.y+8)*65536+10
	mcall	13,<wnd4.x+window_text.size*6+20,wnd4.x+wnd4.width-10-\
			(wnd4.x+window_text.size*6+20)>,,[dtp.work_graph]

	add	ecx,25*65536+10
	mcall	8,<wnd4.x+wnd4.width/2-button_text.size*3-6,\
			button_text.size*6+11>,,0,[dtp.work_button]

	shr	ecx,16
	mov	bx,cx
	add	ebx,0x00060007
	mcall	4,,[dtp.work_button_text],button_text,button_text.size
	ret
;---------------------------------------------------------------------
find_bitmap:
	mov	edi,[ebp+SKIN_HEADER.bitmaps]
	add	edi,ebp
	xor	ebx,ebx
;--------------------------------------
.lp1:
	cmp	dword[edi],0
	je	.lp2

	cmp	dword[edi+0],eax
	jne	@f
;--------------------------------------
	mov	ebx,[edi+SKIN_BITMAPS._data]
	add	ebx,ebp
	mov	ecx,[ebx-2]
	mov	cx,[ebx+4]
	add	ebx,8
;--------------------------------------
.lp2:
	ret
;--------------------------------------
@@:
	add	edi,8
	jmp	.lp1
;----------------------------------------------------------------------
dec_edx:
	sub	dl,4
	jnc	@f

	xor	dl,dl
;--------------------------------------
@@:
	sub	dh,4
	jnc	@f

	xor	dh,dh
;--------------------------------------
@@:
	rol	edx,16
	sub	dl,4
	jnc	@f

	xor	dl,dl
;--------------------------------------
@@:
	rol	edx,16
	ret
;---------------------------------------------------------------------
draw_framerect: ; ebx,ecx
	push	ebx ecx
	add	bx,[esp+6]
	mov	cx,[esp+2]
	dec	ebx
	mcall	38
	add	cx,[esp]
	rol	ecx,16
	add	cx,[esp]
	sub	ecx,0x00010001
	mcall
	mov	ebx,[esp+4]
	mov	ecx,[esp]
	mov	bx,[esp+6]
	add	cx,[esp+2]
	dec	ecx
	mcall
	add	bx,[esp+4]
	rol	ebx,16
	add	bx,[esp+4]
	sub	ebx,0x00010001
	mcall
	add	esp,8
	ret
;----------------------------------------------------------------------
