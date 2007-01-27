recode:

  .866.1251:
	mov	edi,table.866.1251
	jmp	.main
  .1251.866:
	mov	edi,table.1251.866
	jmp	.main
  .866.koi:
	mov	edi,table.866.koi
	jmp	.main
  .koi.866:
	mov	edi,table.koi.866
	jmp	.main
  .1251.koi:
	mov	edi,table.1251.koi
	jmp	.main
  .koi.1251:
	mov	edi,table.koi.1251

  .main:
	mov	ecx,[cur_editor.Lines.Count]
	mov	esi,[cur_editor.Lines]
	jecxz	.exit
	xor	eax,eax
  .lp0: dec	ecx
	js	.exit
	movzx	edx,word[esi]
	add	esi,4
    @@: dec	edx
	js	.lp0
	lodsb
	add	al,-$80
	js	@b
	mov	al,[edi+eax]
	mov	[esi-1],al
	jmp	@b
  .exit:
	ret

table.866.1251 db \
  $C0,$C1,$C2,$C3,$C4,$C5,$C6,$C7 , $C8,$C9,$CA,$CB,$CC,$CD,$CE,$CF,\
  $D0,$D1,$D2,$D3,$D4,$D5,$D6,$D7 , $D8,$D9,$DA,$DB,$DC,$DD,$DE,$DF,\
  $E0,$E1,$E2,$E3,$E4,$E5,$E6,$E7 , $E8,$E9,$EA,$EB,$EC,$ED,$EE,$EF,\
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $F0,$F1,$F2,$F3,$F4,$F5,$F6,$F7 , $F8,$F9,$FA,$FB,$FC,$FD,$FE,$FF,\
  $A8,$B8,$AA,$BA,$AF,$BF,$A1,$A2 , $B0,$95,$B7,$00,$B9,$A4,$00,$00
table.1251.866 db \
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$00,$00,$00,$00,$F9,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$F6,$F7,$00,$FD,$00,$00,$00 , $F0,$00,$F2,$00,$00,$00,$00,$F4,\
  $F8,$00,$00,$00,$00,$00,$00,$FA , $F1,$FC,$F3,$00,$00,$00,$00,$F5,\
  $80,$81,$82,$83,$84,$85,$86,$87 , $88,$89,$8A,$8B,$8C,$8D,$8E,$8F,\
  $90,$91,$92,$93,$94,$95,$96,$97 , $98,$99,$9A,$9B,$9C,$9D,$9E,$9F,\
  $A0,$A1,$A2,$A3,$A4,$A5,$A6,$A7 , $A8,$A9,$AA,$AB,$AC,$AD,$AE,$AF,\
  $E0,$E1,$E2,$E3,$E4,$E5,$E6,$E7 , $E8,$E9,$EA,$EB,$EC,$ED,$EE,$EF
table.866.koi db \
  $E1,$E2,$F7,$E7,$E4,$E5,$F6,$FA , $E9,$EA,$EB,$EC,$ED,$EE,$EF,$F0,\
  $F2,$F3,$F4,$F5,$E6,$E8,$E3,$FE , $FB,$FD,$FF,$F9,$F8,$FC,$E0,$F1,\
  $C1,$C2,$D7,$C7,$C4,$C5,$D6,$DA , $C9,$CA,$CB,$CC,$CD,$CE,$CF,$D0,\
  $90,$00,$00,$81,$87,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$83,\
  $84,$89,$88,$86,$80,$8A,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$85,$82,$8D,$8C,$8E,$8F,$8B,\
  $D2,$D3,$D4,$D5,$C6,$C8,$C3,$DE , $DB,$DD,$DF,$D9,$D8,$DC,$C0,$D1,\
  $B3,$A3,$B4,$A4,$B7,$A7,$BE,$AE , $00,$95,$9E,$00,$B0,$9F,$00,$A0
table.koi.866 db \
  $C4,$B3,$DA,$BF,$C0,$D9,$C3,$B4 , $C2,$C1,$C5,$DF,$DC,$DB,$DD,$DE,\
  $B0,$00,$00,$00,$00,$F9,$00,$00 , $00,$00,$00,$00,$00,$00,$FA,$FD,\
  $FF,$00,$00,$F1,$F3,$00,$00,$F5 , $00,$00,$00,$00,$00,$00,$F7,$00,\
  $FC,$00,$00,$F0,$F2,$00,$00,$F4 , $00,$00,$00,$00,$00,$00,$F6,$00,\
  $EE,$A0,$A1,$E6,$A4,$A5,$E4,$A3 , $E5,$A8,$A9,$AA,$AB,$AC,$AD,$AE,\
  $AF,$EF,$E0,$E1,$E2,$E3,$A6,$A2 , $EC,$EB,$A7,$E8,$ED,$E9,$E7,$EA,\
  $9E,$80,$81,$96,$84,$85,$94,$83 , $95,$88,$89,$8A,$8B,$8C,$8D,$8E,\
  $8F,$9F,$90,$91,$92,$93,$86,$82 , $9C,$9B,$87,$98,$9D,$99,$97,$9A
table.1251.koi db \
  $B1,$B2,$00,$A2,$00,$00,$00,$00 , $00,$00,$B9,$00,$BA,$BC,$BB,$BF,\
  $A1,$91,$92,$93,$94,$95,$96,$97 , $00,$99,$A9,$00,$AA,$AC,$AB,$AF,\
  $A0,$BE,$AE,$B8,$9F,$BD,$00,$00 , $B3,$98,$B4,$9D,$00,$00,$9C,$B7,\
  $00,$00,$B6,$A6,$AD,$00,$00,$9E , $A3,$B0,$A4,$9B,$A8,$B5,$A5,$A7,\
  $E1,$E2,$F7,$E7,$E4,$E5,$F6,$FA , $E9,$EA,$EB,$EC,$ED,$EE,$EF,$F0,\
  $F2,$F3,$F4,$F5,$E6,$E8,$E3,$FE , $FB,$FD,$FF,$F9,$F8,$FC,$E0,$F1,\
  $C1,$C2,$D7,$C7,$C4,$C5,$D6,$DA , $C9,$CA,$CB,$CC,$CD,$CE,$CF,$D0,\
  $D2,$D3,$D4,$D5,$C6,$C8,$C3,$DE , $DB,$DD,$DF,$D9,$D8,$DC,$C0,$D1
table.koi.1251 db \
  $00,$00,$00,$00,$00,$00,$00,$00 , $00,$00,$00,$00,$00,$00,$00,$00,\
  $00,$91,$92,$93,$94,$95,$96,$97 , $00,$99,$00,$BB,$AE,$AB,$B7,$A4,\
  $A0,$90,$83,$B8,$BA,$BE,$B3,$BF , $BC,$9A,$9C,$9E,$9D,$B4,$A2,$9F,\
  $B9,$80,$81,$A8,$AA,$BD,$B2,$AF , $A3,$8A,$8C,$8E,$8D,$A5,$A1,$8F,\
  $FE,$E0,$E1,$F6,$E4,$E5,$F4,$E3 , $F5,$E8,$E9,$EA,$EB,$EC,$ED,$EE,\
  $EF,$FF,$F0,$F1,$F2,$F3,$E6,$E2 , $FC,$FB,$E7,$F8,$FD,$F9,$F7,$FA,\
  $DE,$C0,$C1,$D6,$C4,$C5,$D4,$C3 , $D5,$C8,$C9,$CA,$CB,$CC,$CD,$CE,\
  $CF,$DF,$D0,$D1,$D2,$D3,$C6,$C2 , $DC,$DB,$C7,$D8,$DD,$D9,$D7,$DA
