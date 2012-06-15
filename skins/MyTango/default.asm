include 'my_skin.inc'

SKIN_PARAMS \
  height          = bmp_base.height,\                       ; skin height
  margins         = [8:1:47:1],\                            ; margins [left:top:right:bottom]
  colors active   = [binner=0x4989CA:\                      ; border inner color
                     bouter=0x2B5E92:\                      ; border outer color
                     bframe=0x4989CA],\                     ; border frame color
  colors inactive = [binner=0xA3A3A3:\                      ; border inner color
                     bouter=0x828282:\                      ; border outer color
                     bframe=0xA3A3A3],\                     ; border frame color
  dtp             = 'default.dtp'                           ; dtp colors

SKIN_BUTTONS \
  close    = [-23:4][18:16],\                               ; buttons coordinates
  minimize = [-44:4][18:16]                                 ; [left:top][width:height]

SKIN_BITMAPS \
  left active   = bmp_left,\                                ; skin bitmaps pointers
  left inactive = bmp_left1,\
  oper active   = bmp_oper,\
  oper inactive = bmp_oper1,\
  base active   = bmp_base,\
  base inactive = bmp_base1

BITMAP bmp_left ,'active/left.bmp'                          ; skin bitmaps
BITMAP bmp_oper ,'active/oper.bmp'
BITMAP bmp_base ,'active/base.bmp'
BITMAP bmp_left1,'inactive/left.bmp'
BITMAP bmp_oper1,'inactive/oper.bmp'
BITMAP bmp_base1,'inactive/base.bmp'
