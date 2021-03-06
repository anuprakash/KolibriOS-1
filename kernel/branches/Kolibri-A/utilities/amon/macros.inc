; language for programs
lang fix en ; ru en fr ge fi

@^ fix macro comment {
^@ fix }

nl		equ	0x0d, 0x0a
;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
macro uglobal {
  UGlobals equ UGlobals,
  macro __UGlobalBlock { }

endg fix }      ; Use endg for ending iglobal and uglobal blocks.

macro IncludeUGlobals{
  macro UGlobals dummy,[n] \{
    \common
      \local begin, size
      begin = $
      virtual at $
    \forward
      __UGlobalBlock
      purge __UGlobalBlock
    \common
      size = $ - begin
    end virtual
    rb size
  \}
  match U, UGlobals \{ U \} }

uglobal
endg
;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

; new application structure
macro meos_app_start
 {
  use32
  org 0x0

  db 'MENUET01'
  dd 0x01
  dd __start
  dd __end
  dd __memory
  dd __stack

  if used __params & ~defined __params
    dd __params
  else
    dd 0x0
  end if

  dd 0x0
 }
MEOS_APP_START fix meos_app_start

macro code
 {
  __start:
 }
CODE fix code

macro data
 {
  __data:
 }
DATA fix data

macro udata
 {
  if used __params & ~defined __params
    __params:
      db 0
    __end:
      rb 255
  else
    __end:
  end if
  __udata:
  IncludeUGlobals
 }
UDATA fix udata

macro meos_app_end
 {
  align 32
  rb 2048
  __stack:
  __memory:
  display nl, ' '
 }
MEOS_APP_END fix meos_app_end

 ; macro for defining multiline text data
struc mls [sstring] {
	forward
	local ssize
	virtual at 0
		db sstring	; mod
		ssize = $
	end virtual
	db ssize
	db sstring
	common
	
	db -1			; mod
}
 


; strings
macro sz name,[data] {	     ; from MFAR [mike.dld]
 common
  if used name
   name db data
   .size = $-name
  end if
}

macro lsz name,[lng,data] {  ; from MFAR [mike.dld]
 common
  if used name
   label name
 forward
  if lang eq lng
   db data
  end if
 common
   .size = $-name
  end if
}

macro szc name,elsz,[data] {	     ; from MFAR [mike.dld]
 common
  local s,m
  m = 0
  if used name
   label name
   virtual at 0
    db data
    s = $
   end virtual
   d#elsz s
   if m < s
    m = s
   end if
   db data
   .size = $-name
   .maxl = m
  end if
}

macro lszc name,elsz,[lng,data] {  ; from MFAR [mike.dld]
 common
  local s,m
  m = 0
  if used name
   label name
 forward
  if lang eq lng
   virtual at 0
    db data
    s = $
   end virtual
   d#elsz s
   if m < s
    m = s
   end if
   db data
  end if
 common
   .size = $-name
   .maxl = m
  end if
}


; easy system call macro
macro mpack dest, hsrc, lsrc
{
  if (hsrc eqtype 0) & (lsrc eqtype 0)
    mov dest, (hsrc) shl 16 + lsrc
  else
    if (hsrc eqtype 0) & (~lsrc eqtype 0)
      mov dest, (hsrc) shl 16
      add dest, lsrc
    else
      mov dest, hsrc
      shl dest, 16
      add dest, lsrc
    end if
  end if
}

;macro __mov reg,a {         ; mike.dld
; if ~a eq
;   mov reg,a
; end if
;}

macro __mov reg,a,b {	    ; mike.dld
 if (~a eq)&(~b eq)
   mpack reg,a,b
 else if (~a eq)&(b eq)
   mov reg,a
 end if
}

macro mcall a,b,c,d,e,f {   ; mike.dld
 __mov eax,a
 __mov ebx,b
 __mov ecx,c
 __mov edx,d
 __mov esi,e
 __mov edi,f
 int   0x40
}


; -------------------------
macro header a,[b] {
 common
  use32
  org 0
  db 'MENUET',a
 forward
  if b eq
   dd 0
  else
   dd b
  end if }
macro section name { align 16
 label name }
macro func name {
 if ~used name
  display 'FUNC NOT USED: ',`name,13,10
 else
  align 4
  name:
;pushad
;pushfd
;dps `name
;newline
;mcall 5,1
;popfd
;popad
}
macro endf { end if }

macro diff16 title,l1,l2
 {
  local s,d
  s = l2-l1
  display title,': 0x'
  repeat 8
   d = '0' + s shr ((8-%) shl 2) and $0F
   if d > '9'
    d = d + 'A'-'9'-1
   end if
   display d
  end repeat
  display 13,10
 }

macro diff10 title,l1,l2
 {
  local s,d,z,m
  s = l2-l1
  z = 0
  m = 1000000000
  display title,': '
  repeat 10
   d = '0' + s / m
   s = s - (s/m)*m
   m = m / 10
   if d <> '0'
    z = 1
   end if
   if z <> 0
    display d
   end if
  end repeat
  display 13,10
 }

; optimize the code for size
__regs fix <eax,ebx,ecx,edx,esi,edi,ebp,esp>

macro add arg1,arg2
 {
   if (arg2 eqtype 0)
      if (arg2) = 1
	 inc arg1
      else
	 add arg1,arg2
      end if
   else
      add arg1,arg2
   end if
 }

macro sub arg1,arg2
 {
   if (arg2 eqtype 0)
      if (arg2) = 1
	 dec arg1
      else
	 sub arg1,arg2
      end if
   else
      sub arg1,arg2
   end if
 }

macro mov arg1,arg2
 {
   if (arg1 in __regs) & (arg2 eqtype 0)
      if (arg2) = 0
	 xor arg1,arg1
      else if (arg2) = 1
	 xor arg1,arg1
	 inc arg1
      else if (arg2) = -1
	 or  arg1,-1
      else if (arg2) > -128 & (arg2) < 128
	 push arg2
	 pop  arg1
      else
	 mov  arg1,arg2
      end if
   else
      mov arg1,arg2
   end if
 }


struc POINT _t,_dx,_dy {
 .x _t _dx
 .y _t _dy
}


; constants

; events
EV_IDLE        = 0
EV_TIMER       = 0
EV_REDRAW      = 1
EV_KEY	       = 2
EV_BUTTON      = 3
EV_EXIT        = 4
EV_BACKGROUND  = 5
EV_MOUSE       = 6
EV_IPC	       = 7
EV_STACK       = 8

; event mask bits for function 40
EVM_REDRAW     =	1b
EVM_KEY        =       10b
EVM_BUTTON     =      100b
EVM_EXIT       =     1000b
EVM_BACKGROUND =    10000b
EVM_MOUSE      =   100000b
EVM_IPC        =  1000000b
EVM_STACK      = 10000000b