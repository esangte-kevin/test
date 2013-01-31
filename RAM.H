#ifndef	_RAM_H_
#define _RAM_H_

#include "OSD.H"

typedef struct
	{
	unsigned char Menu;	//define OSD menu type
	unsigned char Page; 	//define OSD page index
	unsigned char Level; 	//define the level of each page
	unsigned char Ptr;   		//define the control function ptr
	unsigned char Op;    		//define the operation type(+ or -)
	}CONTROL_DEF;


// menu type ==========
// menu-0: there is no OSD menu
// menu-1: general OSD menu
// menu-2: Hot key menu
#define MenuType_None			0x00
#define MenuType_General			0x01
#define MenuType_HotKey			0x02

// Russh coustom OSD =======
// Menu type = osd main menu
// page-0: Main menu
// page-1: Image setup menu
// page-2: Source select
// page-3: OSD setup
// page-4: ChannelSelect
#define Menu_Main				0x00
#define Menu_ImageSetup			0x01
#define Menu_SourceSelect			0x02
#define Menu_OSDSetup			0x03
#define Menu_ChannelSelect		0x04
// Menu type = hot key menu
#define HotKeyMenu_None			0x05
#define HotKeyMenu_Contrast		0x06	
#define HotKeyMenu_ChannelSelect	0x07

// level-0: normal
// level-1: Main item level
// level-2: subitem level
#define Level_Normal		0x00
#define Level_MainItem		0x01
#define Level_SubItem		0x02

//====================
//


extern unsigned char bdata flag1;
extern bit NoSyncFlag;
extern bit ForceToBack;  
extern bit ChangeKey; 	
extern bit VideoMute; 	
extern bit MessageShow;	
extern bit Tw9919En;	
extern bit NewMode;			
extern bit GTmodeEn;	

extern unsigned char bdata flag2;
extern bit Abort;		
extern bit PowerDown;	
extern bit KeyLock;
extern bit OsdEnable;		
extern bit FastMuteEnable;
extern bit Interlance;	
extern bit SaveDDC1;
extern bit SaveDDC0;	

extern unsigned char bdata VideoFlag;
extern bit VideoIntLance;
extern bit Video_60Hz;
extern bit VideoUnlock;
extern bit ColorDetected;

//extern bit Pwroff_menu_flag;
extern bit MonoVideo;
extern bit channechange; //BlueScreen 0709
extern bit channel_select_video; // kevin 0709
extern bit VideoOK;
extern bit BypassSOG;
extern bit EndMute;
extern bit NonFullScreen;
extern bit BackLightTimerEn;
//extern bit KeyRepeat;
extern bit StartPollACC;
extern bit ScalerMode;
extern bit WaitHT;
extern bit IICError;
extern unsigned short HistTh;


extern unsigned char bdata StatusFlag;
extern bit PowerStatus;
extern bit BurnInMode;
extern bit FactMode;
extern bit OsdLock;
extern bit AudioMute;
extern bit AutoSetting;
extern bit TextMode;
extern bit OSDChannelSelectMode;
//extern bit FLG_AutoTest;
//extern bit ForceSource;

extern idata unsigned char		Timer_1ms;
extern idata unsigned char 	T0_INT_CNT;
extern idata unsigned short	OsdTimer;
extern idata unsigned char	LocalTimer;
extern idata unsigned short	PowerTimer;
extern idata unsigned short	RepeatTimer;
extern idata unsigned char  DDCTimer;
extern idata unsigned char  SaveTimer;
extern idata unsigned char	VideoTimer;
extern idata unsigned char  BurnInTimer;
extern idata unsigned char  MuteTimer;
extern idata unsigned char	T0_10ms_Cnt;
extern idata unsigned char	T0_1ms_Cnt;
extern idata unsigned char	PowerKey_Timer;//benson980614
extern xdata unsigned long	BackLightTimer;
extern idata unsigned char	KeyRepeatCnt;


extern idata unsigned short	H_SYNC;
extern idata unsigned short	V_SYNC;
extern idata unsigned short	H_SYNC_Temp;
extern idata unsigned short	V_SYNC_Temp;
extern idata unsigned short	VTotal;
extern idata unsigned short	H_Act;
extern idata unsigned char  ResolutionPtr;
extern idata unsigned char  ChangeMode;
extern idata unsigned char	HV_Pol;
extern idata unsigned char	HV_Pol_Temp;
extern idata unsigned char	ModePoint;
extern idata unsigned char	KeyBuffer;
extern idata unsigned char	PrevKey;
extern idata unsigned char  FuncPtr;
extern idata unsigned char  SavePointer;
extern idata unsigned char 	RegPage;		
extern idata unsigned char  OutOfRange;
extern idata unsigned char	SyncMode;
extern idata unsigned char  DigitalPtr;

extern xdata unsigned short	FuncBuf[85];
extern xdata unsigned short Hresolution;
extern xdata unsigned short Vresolution;
extern xdata unsigned short FuncMax;
extern xdata unsigned short FuncMin;
extern xdata unsigned short ClockBase;
extern xdata unsigned short HPositionBase;
extern xdata unsigned char BackPtr;
extern xdata unsigned char TvMode;
extern xdata unsigned char VideoSourcePtr;
extern xdata unsigned char TempPtr;
extern xdata unsigned char MaxItem;
extern xdata unsigned char BackItem;
extern xdata unsigned char UserModePtr;
extern xdata unsigned char  BurnInPattern;
extern idata unsigned char 	TempPhase;
extern xdata unsigned char SCRev;
extern CONTROL_DEF xdata MenuPtr;
extern xdata unsigned char UserModeRefReso;
extern xdata unsigned char SCFactor;
extern xdata unsigned char DVI_Clock;
extern xdata unsigned char SCID;

extern xdata unsigned char TestPoint;


extern unsigned char bdata FBA1665D0Type;
//extern bit VideoAutoDetect;
//extern bit VGAOnlyFlag;
//extern bit svideoFlag;
//extern bit autoPowerFlag;
//extern bit Mirror_Flag;
//extern bit Reverse_Flag;
//extern bit power_onFlag;
extern bit dimmer_Flag;
extern bit sensorFlag;
extern bit audioFlag;
//extern bit Mute_Flag;
//extern unsigned char bdata AllSyncStatus;
extern unsigned char bdata flag3;
extern bit Pwroff_menu_flag;
extern bit CCDSignal_IN;	
extern bit CCD_power_on;
extern bit	Osd_re_build;
extern bit AutoColorFlag;
extern bit goto_flag;				// 1: enable & force system goto restart
extern bit PowerTooLow;

/*
extern bit cvbs1sync;
extern bit cvbs2sync;
extern bit svideosync;
extern bit dsubsync;
extern bit dvidsync;
extern bit ypbprsync;
extern bit PlugCounter;
extern bit syncChange;
*/
extern unsigned char idata AllSyncStatus_temp;
extern unsigned char bdata InputMode;
extern bit cvbs1;
extern bit cvbs2;
extern bit svideo;
extern bit dsub;
extern bit dvid;
extern bit YPbPr;
extern bit hotkey;

extern unsigned char data dimmerValue;
extern unsigned char NTSCorPAL;
extern unsigned char ICommand;
//IR
extern bit IR_Ready;
extern bit Ch100;
extern bit IR_DisPlay;
extern bit Favorite_channel;
extern bit Chan_key;
extern bit ScanChan_key;
extern bit IRKeyRepeat;
extern bit IRKeyfirst;
extern xdata unsigned short IR_Timer;
extern xdata unsigned short Chankey_Timer;
extern xdata unsigned short Timer3;
extern xdata unsigned short Timer4;
extern xdata unsigned short Timer5;
extern xdata unsigned char IRState;
extern xdata unsigned char IRKeyCount;
extern xdata unsigned char ChanPoint;
extern xdata unsigned long IRCode;
extern xdata unsigned char 	IRKeyTemp;
extern xdata unsigned char 	IRSerchTime;
extern xdata unsigned char 	cbuf[64];
extern xdata unsigned char videoPowerDownSync;
extern xdata unsigned char dimmerPlusBrightness;
extern xdata unsigned char old_light;
extern xdata unsigned char CCD_old_channel;
extern xdata unsigned char CCD_pre_StatusFlag;

// kevin 1203 2008
extern code unsigned char systemdate[];
extern code unsigned char systemtime[];
#endif
#ifdef PclkRun
extern xdata unsigned long XH_Counter;
#endif
