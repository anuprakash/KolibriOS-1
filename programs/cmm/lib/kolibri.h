//CODED by Veliant, Leency, Nable, Pavelyakov. GNU GPL licence.

#ifndef INCLUDE_KOLIBRI_H
#define INCLUDE_KOLIBRI_H
#print "[include <kolibri.h>]\n"

#pragma option OST
#pragma option ON
#pragma option cri-
#pragma option -CPA
#initallvar 0
#jumptomain FALSE

#startaddress 0

#code32 TRUE

char   os_name[8]   = {'M','E','N','U','E','T','0','1'};
dword  os_version   = 0x00000001;
dword  start_addr   = #______INIT______;
dword  final_addr   = #______STOP______+32;
dword  alloc_mem    = MEMSIZE;
dword  x86esp_reg   = MEMSIZE;
dword  I_Param      = #param;
dword  I_Path       = #program_path;
char param[4096];
char program_path[4096];

#define bool      char

#define NULL      0
#define OLD      -1
#define true      1
#define false     0

//Process Events
#define evReDraw  1
#define evKey     2
#define evButton  3
#define evDesktop 5
#define evMouse   6
#define evIPC     7
#define evNetwork 8
#define evDebug   9

//Button options
#define BT_DEL      0x80000000
#define BT_HIDE     0x40000000
#define BT_NOFRAME  0x20000000

//allow event mask
#define EVENT_MASK_REDRAW   000000001b
#define EVENT_MASK_KEYBOARD 000000010b
#define EVENT_MASK_BUTTONS  000000100b
#define EVENT_MASK_DESKTOP  000010000b
#define EVENT_MASK_MOUSE    000100000b
#define EVENT_MASK_IPC      001000000b
#define EVENT_MASK_NETWORK  010000000b
#define EVENT_MASK_DEBUG    100000000b

//ARGS FUNCTION
#define END_ARGS 0xFF00FF
//-------------------------------------------------------------------------

#include "../lib/system.h"
#include "../lib/mouse.h"
#include "../lib/keyboard.h"

:struct raw_image {
	dword w, h, data;
};

//------------------------------------------------------------------------------
:dword wait_event_code;
inline fastcall dword WaitEvent()
{
	$mov eax,10
	$int 0x40
	wait_event_code = EAX;
	//if(wait_event_code==evMouse) MOUSE.get();
	//return wait_event_code;
}

inline fastcall dword CheckEvent()
{
	$mov eax,11
	$int 0x40
}

inline fastcall dword WaitEventTimeout(EBX)
{
	$mov eax,23
	$int 0x40
} 
 
inline fastcall SetEventMask(EBX)
{
	$mov eax,40
	$int 0x40
}


inline fastcall pause(EBX)
{
	$mov eax, 5
	$int 0x40
}

inline fastcall word GetButtonID()
{
	$mov eax,17
	$int  0x40
	$shr eax,8
}

inline fastcall dword GetFreeRAM()
{
	$mov eax, 18
	$mov ebx, 16
	$int 0x40
	//return eax = ������ ��������� ������ � ����������
}

inline fastcall dword LoadDriver(ECX) //ECX - ��� ��������
{
	$mov eax, 68
	$mov ebx, 16
	$int 0x40
	//return 0 - �������, ����� eax = ����� �������� 
}

inline fastcall dword RuleDriver(ECX) //��������� �� ����������� ���������
{
	$mov eax, 68
	$mov ebx, 17
	$int 0x40
	//return eax = ������������ ���������
}

struct proc_info
{
	#define SelfInfo -1
	dword	use_cpu;
	word	pos_in_stack,num_slot,rezerv1;
	unsigned char name[11];
	char	rezerv2;
	dword	adress,use_memory,ID,left,top,width,height;
	word	status_slot,rezerv3;
	dword	work_left,work_top,work_width,work_height;
	char	status_window;
	dword   cwidth,cheight;
	byte    reserved[1024-71-8];
};

inline fastcall void GetProcessInfo(EBX, ECX)
{
	$mov eax,9;
	$int  0x40
	DSDWORD[EBX+71] = DSDWORD[EBX+42] - 9; //set cwidth
	DSDWORD[EBX+75] = DSDWORD[EBX+46] - GetSkinHeight() - 4; //set cheight
}

inline fastcall int GetPointOwner( EBX, ECX) //ebx=m.x, ecx=m.y
{
	$mov eax,34
	$int 0x40
}

inline fastcall int GetProcessSlot( ECX)
{
	EAX = 18;
	EBX = 21;
	$int 0x40
}

inline fastcall int GetActiveProcess()
{
	EAX = 18;
	EBX = 7;
	$int 0x40
}

:int CheckActiveProcess(int Form_ID)
{
	int id9=GetProcessSlot(Form_ID);
	if (id9==GetActiveProcess()) return 1;
	return 0;
}

inline fastcall void ActivateWindow( ECX)
{
	EAX = 18;
	EBX = 3;
	$int 0x40
}

inline fastcall int MinimizeWindow()
{
	EAX = 18;
	EBX = 10;
	$int 0x40
}

inline fastcall int CreateThread(ECX,EDX)
{
	$mov eax,51
	$mov ebx,1
	$int 0x40
}

inline fastcall void SwitchToAnotherThread()
{
	$mov eax,68
	$mov ebx,1
	$int 0x40
}

inline fastcall int SendWindowMessage( ECX, EDX)
{
	$mov eax, 72
	$mov ebx, 1
	$int 0x40
}

inline fastcall int KillProcess( ECX)
{
	$mov eax,18;
	$mov ebx,18;
	$int 0x40
}

#define TURN_OFF 2
#define REBOOT 3
#define KERNEL 4
inline fastcall int ExitSystem( ECX)
{
	$mov eax, 18
	$mov ebx, 9
	$int 0x40
}

inline fastcall ExitProcess()
{
	$mov eax,-1;
	$int 0x40
}

//------------------------------------------------------------------------------

//eax = ���� ������� (1=eng, 2=fi, 3=ger, 4=rus)
inline fastcall int GetSystemLanguage()
{
	EAX = 26;
	EBX = 5;
	$int 0x40
}

inline fastcall GetSkinHeight()
{
	$push ebx
	$mov  eax,48
	$mov  ebx,4
	$int 0x40
	$pop  ebx
}

inline fastcall void SetSystemSkin( ECX)
{
	EAX = 48;
	EBX = 8;
	$int 0x40
}

inline fastcall int GetScreenWidth()
{
	$mov eax, 14
	$int 0x40
	$shr eax, 16
}

inline fastcall int GetScreenHeight()
{
	$mov eax, 14
	$int 0x40
	$and eax,0x0000FFFF
}

inline fastcall int GetClientTop()
{
	$mov eax, 48
	$mov ebx, 5
	$int 0x40
	$mov eax, ebx
	$shr eax, 16
}

inline fastcall int GetClientHeight()
{
	$mov eax, 48
	$mov ebx, 5
	$int 0x40
	$mov eax, ebx
}

inline fastcall dword LoadLibrary( ECX)
{
	$mov eax, 68 
	$mov ebx, 19
	$int  0x40
}

inline fastcall int TestBit( EAX, CL)
{
	$shr eax,cl
	$and eax,1
}

//------------------------------------------------------------------------------

void DefineAndDrawWindow(dword x, y, size_w, size_h, byte WindowType,dword WindowAreaColor, EDI, ESI)
{
	EAX = 12;              // function 12:tell os about windowdraw
	EBX = 1;
	$int 0x40
	
	$xor EAX,EAX
	EBX = x << 16 + size_w; 
	ECX = y << 16 + size_h;

	EDX = WindowType << 24 | WindowAreaColor;
	$int 0x40

	EAX = 12;              // function 12:tell os about windowdraw
	EBX = 2;
	$int 0x40
}

inline fastcall MoveSize( EBX,ECX,EDX,ESI)
{
	$mov eax, 67
	$int 0x40
}

inline fastcall void DrawTitle( ECX)
{
	EAX = 71;
	EBX = 1;
	$int 0x40;
}

void WriteTextB(dword x,y,byte fontType, dword color, EDX)
{
	EAX = 4;
	EBX = x<<16+y;
	ECX = fontType<<24+color;
	ESI = 0;
	$int 0x40;
	$add ebx, 1<<16
	$int 0x40
}

void WriteText(dword x,y,byte fontType, dword color, EDX)
{
	EAX = 4;
	EBX = x<<16+y;
	ECX = fontType<<24+color;
	$int 0x40;
}

dword WriteBufText(dword x,y,byte fontType, dword color, EDX, EDI)
{
	EAX = 4;
	EBX = x<<16+y;
	ECX = fontType<<24+color;
	$int 0x40;
}

void WriteNumber(dword x,y,byte fontType, dword color, count, ECX)
{
	EAX = 47;
	EBX = count<<16;
	EDX = x<<16+y;
	ESI = fontType<<24+color;
	$int 0x40;
}

void CopyScreen(dword EBX, x, y, w, h)
{
  EAX = 36;
  ECX = w << 16 + h;
  EDX = x << 16 + y;
  $int  0x40;
}

:dword GetPixelColor(dword x, x_size, y)
{
	$mov eax, 35
	EBX= y*x_size+x;
	$int 0x40
}


void _PutImage(dword x,y, w,h, EBX)
{
	EAX = 7;
	ECX = w<<16+h;
	EDX = x<<16+y;
	$int 0x40
}

void PutPaletteImage(dword EBX,w,h,x,y,ESI,EDI)
{
	EAX = 65;
	ECX = w<<16+h;
	EDX = x<<16+y;
	EBP = 0;
	$int 0x40
} 

inline fastcall void PutPixel( EBX,ECX,EDX)
{
  EAX=1;
  $int 0x40
}

void DrawBar(dword x,y,w,h,EDX)
{
	if (h<=0) || (h>60000) || (w<=0) || (w>60000) return; //bad boy :)
	EAX = 13;
	EBX = x<<16+w;
	ECX = y<<16+h;
	$int 0x40
}

void DefineButton(dword x,y,w,h,EDX,ESI)
{
	EAX = 8;
	$push edx
	EDX += BT_DEL;
	$int 0x40;
	$pop edx
	EBX = x<<16+w;
	ECX = y<<16+h;
	$int 0x40
}

inline RefreshWindow(dword ID_REFRESH,ID_ACTIVE)
{
	EAX = 18;
	EBX = 22;
	ECX = 3;
	EDX = ID_REFRESH;
	$int 0x40
	EAX = 18;
	EBX = 3;
	EDX = ID_ACTIVE;
	$int 0x40
}

void UnsafeDefineButton(dword x,y,w,h,EDX,ESI)
{
	EAX = 8;
	EBX = x<<16+w;
	ECX = y<<16+h;
	$int 0x40
}

inline fastcall void DeleteButton( EDX)
{
	EAX = 8;
	EDX += BT_DEL;
	$int 0x40
}

inline fastcall dword GetStartTime()
{
	$mov eax,26
	$mov ebx,9
	$int 0x40
}

:dword X_EventRedrawWindow,Y_EventRedrawWindow;
:void _EventRedrawWindow()
{
	loop()switch(WaitEvent())
	{
		case evReDraw:
			DefineAndDrawWindow(X_EventRedrawWindow,Y_EventRedrawWindow,100,1,1,0x34,0xFFFFFF,"");
			pause(10);
			ExitProcess();
			break;
	}
}
:char REDRAW_BUFF_EVENT_[4096];
:void EventRedrawWindow(dword x,y)
{
	X_EventRedrawWindow = x;
	Y_EventRedrawWindow = y;
	CreateThread(#_EventRedrawWindow,#REDRAW_BUFF_EVENT_+4092);
}

:struct obj
{
	dword x,y,w,h;
	void size();
};

:void obj::size(dword _x, _y, _w, _h)
{
	x=_x; 
	y=_y;
	w=_w;
	h=_h;
}

:struct _screen
{
	dword width,height;
} screen;

:byte skin_height;

:void DrawDate(dword x, y, color, in_date)
{
	EDI = in_date;
	EAX = 47;
	EBX = 2<<16;
	EDX = x<<16+y;
	ESI = 0x90<<24+color;
	ECX = EDI.date.day;
	$int 0x40;
	EDX += 20<<16;
	ECX = EDI.date.month;
	$int 0x40;
	EDX += 20<<16;
	EBX = 4<<16;
	ECX = EDI.date.year;
	$int 0x40;
	DrawBar(x+17,y+10,2,2,color);
	DrawBar(x+37,y+10,2,2,color);
}

:void __path_name__(dword BUF,PATH)
{
	dword beg = PATH;
	dword pos = PATH;
	dword sav = PATH;
	dword i;
	while(DSBYTE[pos])
	{
		if(DSBYTE[pos]=='/')sav = pos;
		pos++;
	}
	i = sav-beg;
	while(i)
	{
		DSBYTE[BUF] = DSBYTE[beg];
		beg++;
		BUF++;
		i--;
	}
	/*while(DSBYTE[beg])
	{
		DSBYTE[BUF1] = DSBYTE[beg];
		beg++;
		BUF1++;
	}*/
	//DSBYTE[BUF1] = 0;
	DSBYTE[BUF] = 0;
}
char __BUF_DIR__[4096];
:struct SELF
{
	dword dir;
	dword file;
	dword path;
} self;

dword __generator;  // random number generator - ��� ��������� ��������� �����

:dword program_path_length;

//The initialization of the initial data before running
void ______INIT______()
{
	self.dir = #__BUF_DIR__;
	self.file = 0;
	self.path = I_Path;
	__path_name__(#__BUF_DIR__,I_Path);
	
	skin_height   = GetSkinHeight();
	screen.width  = GetScreenWidth();
	screen.height = GetScreenHeight();
	
	//program_path_length = strlen(I_Path);
	DOUBLE_CLICK_DELAY = GetMouseDoubleClickDelay();
	__generator = GetStartTime();
	
	mem_init();

	main();
	ExitProcess();
}
______STOP______:
#endif

#ifndef INCLUDE_MEM_H
#include "../lib/mem.h"
#endif

#ifndef INCLUDE_DEBUG_H
#include "../lib/debug.h"
#endif