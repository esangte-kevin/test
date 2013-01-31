#include "RAM.H"
#include "OSD.H"

unsigned char bdata flag1;
sbit NoSyncFlag		= flag1^0;
sbit ForceToBack 		= flag1^1; // if set to 0, to enable blue screen
sbit ChangeKey 		= flag1^2;
sbit VideoMute 		= flag1^3; 
sbit MessageShow		= flag1^4;
sbit Tw9919En		= flag1^5;
sbit NewMode			= flag1^6;
sbit GTmodeEn		= flag1^7;

unsigned char bdata flag2;
sbit Abort				= flag2^0;
sbit PowerDown		= flag2^1;
sbit KeyLock			= flag2^2;
sbit OsdEnable		= flag2^3;
sbit FastMuteEnable	= flag2^4;
sbit Interlance			= flag2^5;
sbit SaveDDC1		= flag2^6;
sbit SaveDDC0		= flag2^7;

unsigned char bdata VideoFlag;
sbit VideoIntLance		= VideoFlag^7;	
sbit VideoUnlock		= VideoFlag^6;
sbit Video_60Hz		= VideoFlag^5;
sbit ColorDetected		= VideoFlag^0;

//bit Pwroff_menu_flag;
bit channechange;//BlueScreen 0709
bit channel_select_video; // kevin 0709
bit MonoVideo;
bit VideoOK;
bit BypassSOG;
bit EndMute;
bit NonFullScreen;
bit BackLightTimerEn;
//bit KeyRepeat;
bit StartPollACC;
bit ScalerMode;
bit WaitHT;
bit IICError;
unsigned short HistTh;

//bit FLG_AutoTest = 0;
//bit ForceSource;

unsigned char bdata StatusFlag;
sbit PowerStatus	= StatusFlag^0;	// 0: power down; 1: power on
sbit BurnInMode		= StatusFlag^1;
sbit FactMode		= StatusFlag^2;
sbit OsdLock		= StatusFlag^3;
sbit AudioMute		= StatusFlag^4;
sbit AutoSetting	= StatusFlag^5;
sbit TextMode		= StatusFlag^6;
sbit OSDChannelSelectMode	= StatusFlag^7;

//unsigned char bdata MiscFlag;
//sbit LightEn		= StatusFlag^0;
//sbit ScalerMode		= StatusFlag^1;

idata unsigned char		Timer_1ms;
idata unsigned char 	T0_INT_CNT;
idata unsigned short	OsdTimer;
idata unsigned char		LocalTimer;
idata unsigned short	PowerTimer;
idata unsigned short	RepeatTimer;
idata unsigned char	DDCTimer;
idata unsigned char	SaveTimer;
idata unsigned char	VideoTimer;
idata unsigned char	BurnInTimer;
idata unsigned char	MuteTimer;
idata unsigned char	T0_10ms_Cnt;
idata unsigned char	T0_1ms_Cnt;
idata unsigned char	PowerKey_Timer;//benson980614
xdata unsigned long	BackLightTimer;
idata unsigned char	KeyRepeatCnt;

idata unsigned short	H_SYNC;
idata unsigned short 	V_SYNC;
idata unsigned short 	H_SYNC_Temp;
idata unsigned short 	V_SYNC_Temp;
idata unsigned short 	VTotal;
idata unsigned short	H_Act;
idata unsigned char	ResolutionPtr;
idata unsigned char	ChangeMode;
idata unsigned char	HV_Pol;
idata unsigned char	HV_Pol_Temp;
idata unsigned char	RegPage;		
idata unsigned char	ModePoint;
idata unsigned char	KeyBuffer;
idata unsigned char	PrevKey;
idata unsigned char	FuncPtr;
idata unsigned char	SavePointer;
idata unsigned char	OutOfRange;
idata unsigned char DigitalPtr;
idata unsigned char	SyncMode;
///0 = Separate H & V
///1= H+V
///2= Sync on green
///3= DVI
xdata unsigned short FuncBuf[85];//[64];//[47];
xdata unsigned short Hresolution;
xdata unsigned short Vresolution;
xdata unsigned short FuncMax;
xdata unsigned short FuncMin;
xdata unsigned short ClockBase;
xdata unsigned short HPositionBase;
xdata unsigned char BackPtr;
xdata unsigned char TvMode;
xdata unsigned char TempPtr;
xdata unsigned char VideoSourcePtr;
xdata unsigned char MaxItem;
xdata unsigned char BackItem;
xdata unsigned char UserModePtr;
xdata unsigned char BurnInPattern;
idata unsigned char TempPhase;
xdata unsigned char SCRev;
CONTROL_DEF xdata MenuPtr;
xdata unsigned char UserModeRefReso;
xdata unsigned char SCFactor;
xdata unsigned char DVI_Clock;
xdata unsigned char SCID;
//auto test,ray
xdata unsigned char TestPoint;
#ifdef PclkRun
xdata unsigned long XH_Counter;
#endif
/***************************************************************************************
* Function name : my define 
* writer		: kevin lin
* Date			: 05/22/2007
* Description	: The flag about FBA1665D0 Type define
****************************************************************************************/

unsigned char bdata FBA1665D0Type;
//sbit VideoAutoDetect = FBA1665D0Type^0;
//sbit VGAOnlyFlag = FBA1665D0Type^1;
//sbit autoPowerFlag = FBA1665D0Type^2;
//sbit Mirror_Flag = FBA1665D0Type^3;
//sbit Reverse_Flag = FBA1665D0Type^4;
//sbit power_onFlag = FBA1665D0Type^4;
sbit dimmer_Flag = FBA1665D0Type^5;	// dimmer_flag = 0  : 0V is max, 5V is min
									//dimmer_flag = 1: 0V is min, 5V is max 
sbit sensorFlag = FBA1665D0Type^6;		// 0: lightness sensor disable
									// 1: photo sensor enable
sbit audioFlag = FBA1665D0Type^7;		// 0: No audio amp 
									// 1: audio amp on board

unsigned char bdata flag3;
sbit Pwroff_menu_flag	= flag3^0;			// 0: means not key press power down, 1: means key press power down 
sbit CCDSignal_IN = flag3^1;				// CCD in 0: ccd signal active low 
sbit CCD_power_on = flag3^2;			// 0: CCD signal dispear, 1:CCD signal exist
sbit	Osd_re_build=flag3^3;
sbit AutoColorFlag=flag3^5;				// 1: auto color was done 0: never runed befor
sbit goto_flag =flag3^6;					// 1: enable & force system goto restart
sbit PowerTooLow =flag3^7;				// 0: power 5V statable, 1: power 5 V too low
/*
unsigned char bdata AllSyncStatus;
sbit cvbs1sync	= AllSyncStatus^0;
sbit cvbs2sync	= AllSyncStatus^1;
sbit svideosync	= AllSyncStatus^2;
sbit dsubsync	= AllSyncStatus^3;
sbit dvidsync	= AllSyncStatus^4;
sbit ypbprsync	= AllSyncStatus^5;
sbit PlugCounter= AllSyncStatus^6;
sbit syncChange	= AllSyncStatus^7;
*/
unsigned char idata AllSyncStatus_temp;
unsigned char bdata InputMode;
sbit cvbs1 = InputMode^0;
sbit cvbs2 = InputMode^1;
sbit svideo = InputMode^2;
sbit dsub = InputMode^3;
sbit dvid = InputMode^4;
sbit YPbPr = InputMode^5;// benson980615
sbit hotkey = InputMode^6;

unsigned char data dimmerValue;
unsigned char NTSCorPAL;
unsigned char ICommand;

//IR
bit	IR_Ready=0;
bit	Ch100 = 0;
bit	IR_DisPlay=0;
bit	Favorite_channel;
bit	Chan_key = 0;
bit	ScanChan_key = 0;
bit IRKeyRepeat = 0;
bit IRKeyfirst = 0;

xdata unsigned short	IR_Timer=0;
xdata unsigned short	Chankey_Timer=0;
xdata unsigned short Timer3=0;
xdata unsigned short	 Timer4=0;
xdata unsigned short	 Timer5=0;
xdata unsigned char		IRState;
xdata unsigned char		IRKeyCount=0;
xdata unsigned char		ChanPoint = 0;
xdata unsigned long		IRCode;
xdata unsigned char 	IRKeyTemp=0;
xdata unsigned char 	IRSerchTime=0;
xdata unsigned char 	cbuf[64];
xdata unsigned char videoPowerDownSync;
xdata unsigned char dimmerPlusBrightness;
xdata unsigned char old_light;
xdata unsigned char 	CCD_old_channel;
xdata unsigned char	CCD_pre_StatusFlag;
//kevin 1203 2008
unsigned char code systemdate[] =__DATE__;
unsigned char code systemtime[] = __TIME__;
