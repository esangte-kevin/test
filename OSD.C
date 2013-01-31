#include "stdio.h"
#include "OSD.H"
#include "Scaler.H"
#include "IIC.H"
#include "MyDef.H"
#include "RAM.H"
#include "F63XREG.H"
#include "F63XDEF.H"
#include "MCU.H"
#include "OSD_TAB.H"
#include "ROM_MAP.H"
#include "PANEL.H"
#include "UserAdj.H"
#include "ModeHandle.H"
#include "TW9919.H"

#if OSD_ENABLE
void BCD(Word Value,Byte *VaSeq,Byte Zero);

void OsdProcess()
{
	// kevin 20090722 for adjust red offset
	Byte tempPtr[5];

	
	if(MenuPtr.Op == 0)
		return;
	/*
	printf("menu= %d \n",(unsigned short)MenuPtr.Menu);
	printf("Level= %d \n",(unsigned short)MenuPtr.Level);
	printf("Op= %d \n",(unsigned short)MenuPtr.Op);
	printf("Page= %d \n",(unsigned short)MenuPtr.Page);
	printf("Ptr= %d \n",(unsigned short)MenuPtr.Ptr);
	printf("FuncPtr=%d\n",(unsigned short)FuncPtr);
	printf("\n");
	*/
	

	if(MenuPtr.Menu == MenuType_General)
	{
		if(MenuPtr.Level == Level_MainItem)
		{
			switch(MenuPtr.Op)
			{
				case oUPDATEALL:
					ShowMain(MenuPtr.Page);
					Osd_On(enWIN4);
					TempPtr = FuncBuf[pVIDEOSOURCE];
					MenuPtr.Level = Level_MainItem;
					break;
				case oENTER:					
						switch(FuncPtr)
						{
							default:
								MenuPtr.Level=Level_SubItem;
								break;
							case pImageSetup:
								ClearTextArea();
								MenuPtr.Ptr =0;
								MenuPtr.Page = Menu_ImageSetup;
								break;
							case pOPTION:	
							case pSourceSelect:
								ClearTextArea();
								MenuPtr.Ptr =0;
								MenuPtr.Page = Menu_SourceSelect;
								break; 
							case pOSDSetup:
								ClearTextArea();
								MenuPtr.Ptr =0;
								MenuPtr.Page = Menu_OSDSetup;
								break;	
							case pRECALL:
							case pAUTOADJUST:
								break;
						}
				case oDECFUNC:
				case oINCFUNC:
					ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
					break;
				case oINCVAL:
				case oDECVAL:
					break;
				case oRETURN:
					if(MenuPtr.Page == Menu_Main)
					{
						Osd_Off();
					}else if(MenuPtr.Page == Menu_ChannelSelect){
						OSDChannelSelectMode=FALSE;
						Osd_Off();		
						ShowMessage(MSG_NOSYNC);
					}else{
						ClearTextArea();
						MenuPtr.Ptr =0;
						if(OSDChannelSelectMode)
							MenuPtr.Page = Menu_ChannelSelect;
						else
							MenuPtr.Page = Menu_Main;
						ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
					}
					break;
				};
		}else 
		if(MenuPtr.Level == Level_SubItem)
		{
			switch(MenuPtr.Op)
			{
				case oENTER:
					break;
				case oRETURN:
					MenuPtr.Level=Level_MainItem;
					ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
					break;
				case oINCVAL:
				case oDECVAL:
					switch(FuncPtr)
					{
						case pHotKey:
							showHotkeydefine();
							break;
						case pSHARPNESS:
							ShowSharpness();
							break;					
						case pSETCVBS1:
						case pSETSVIDEO:
						case pSETDSUB:
						case pSETDVID:
							ShowOnOff(MenuPtr.Ptr);
							break;
						case pCOLORTEMP:
							ShowColorMenu();
							break;
						//case pAUTOPOWERON:
						case pDEFAULTCHANNEL:
							ShowSignalAndPlay();
							break;
						case pLANGUAGE:
							ShowLanguage();
							break;
						case pOSDRotate:
							showRotate();
							break;
							
						case pCONTRAST:
						case pCLOCK:
						case pPHASE:
						case pHPOSITION:
						case pVPOSITION:
						case pRCOLOR:
						case pGCOLOR:
						case pBCOLOR:
						case pOSDVPOSITION:
						case pOSDHPOSITION:
						case pOSDTIMER:
						case pTRANSOSD:
							ShowBar(MAINMENU_BAR_ADDR+(2 * OSD_WIDTH*MenuPtr.Ptr),BAR_2bitIcon_OnSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);
							break;
					};
					break;
				
			};
		}
	}else 
	if(MenuPtr.Menu == MenuType_HotKey && MenuPtr.Page == HotKeyMenu_Contrast){


		switch(MenuPtr.Op)
		{
			case oINCVAL:
			case oDECVAL:
				ShowHotKey_Contrast();
				break;
		};			

	}
	MenuPtr.Op = 0;
	
}

void ShowSyncMode()
{
	Byte code ENG_ANALOG[]={
	6,"Analog"
	};
	Byte code FRA_ANALOG[]={
	10,"Analogique"
	};
	Byte code DEU_ANALOG[]={
	5,"Analog"
	};
	Byte code ESP_ANALOG[]={
	9,"Analgica"
	};
	Byte code ITA_ANALOG[]={
	9,"Analogico"
	};
	Byte code JAP_ANALOG[]={
	4,_J19,_J1a,_J10,_J13
	};
	Byte code RUS_ANALOG[]={
	10,_A,_P04,_a,_P19,_o,_P0a,_o,_P07,_P1a,_P02
	};
	Byte code CHT_ANALOG[]={
	2,_C61,_C21
	};
	Byte code CHI_ANALOG[]={
	2,_C32,_C33
	};
	Byte code ENG_DVI[]={
	7,"DigitalL"
	};
	Byte code FRA_DVI[]={
	9,"Numrique"
	};
	Byte code DEU_DVI[]={
	7,"Digital"
	};
	Byte code ESP_DVI[]={
	7,"Digital"
	};
	Byte code ITA_DVI[]={
	8,"Digitale"
	};
	Byte code JAP_DVI[]={
	4,_J14,_J1b,_J1c,_J18
	};
	Byte code RUS_DVI[]={
	6,_P06,_P03,_P15,_p,_o,_P07,_o,_P02
	};
	Byte code CHT_DVI[]={
	2,_C62,_C14
	};
	Byte code CHI_DVI[]={
	2,_C34,_C27
	};
	Byte code ENG_YPbPr[]={
	10,"DVI-Analog"
	};
	Byte code ENG_CVBS[]={
	4,"CVBS"
	};
	Byte code ENG_SVIDEO[]={
	7,"S-Video"
	};
code unsigned char *SourceTab[][9]={
	{ENG_ANALOG,FRA_ANALOG,DEU_ANALOG,ESP_ANALOG,ITA_ANALOG,JAP_ANALOG,RUS_ANALOG,CHT_ANALOG,CHI_ANALOG},
	{ENG_DVI,FRA_DVI,DEU_DVI,ESP_DVI,ITA_DVI,JAP_DVI,RUS_DVI,CHT_DVI,CHI_DVI},
	{ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr,ENG_YPbPr},
	{ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS,ENG_CVBS},
	{ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO},
};

	Byte i,End,x,y,*Offset,Length;
	Word code SyncModeAddr[5]={ROW2+2,ROW3+2,ROW4+2,ROW5+2,ROW6+2};


	Osd_On(enWIN4);
	//if(Saa7114En)
	if(Tw9919En)
		End = 5;
	else
		End = 2;
	//i = TempPtr;
	i = MenuPtr.Ptr;
	x = SyncModeAddr[i] % OSD_WIDTH;
	y = SyncModeAddr[i] / OSD_WIDTH;
	//if(MenuPtr.Level == 1) 
		//OpenWindow(WINDOW3,x,x+10,y,y,atWIN_SHADOW_4L,WIN3_COLOR);
//	x = Read24C16(ep_Sync_Source);
	if(MenuPtr.Ptr == 2) MenuPtr.Ptr = 3; // pass "dvi-analog" 06/26/2007 kevin
	for(i=0;i<End;i++)
		{
		Offset = SourceTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
//		if(MenuPtr.Level == 1)
//			if(i == MenuPtr.Ptr)
//				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
//			else
//				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
//		else
//			if(FuncBuf[pVIDEOSOURCE] == i)
//				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
//			else			
//				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else if(FuncBuf[pVIDEOSOURCE] == i && MenuPtr.Level == 0)
			WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		//if(i == FuncBuf[pVIDEOSOURCE])
//		if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
//			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
//		else			
//			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		WriteSeqIndexPort(SyncModeAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	if(MenuPtr.Level == 1) 
		Osd_On(enWIN4);
}

#if 0
/********************************************************************************
** name		: kevin 
** data 	: 2007/07/04
** Function	: To show the page of setup mode , Total eight item list like below
				CVBS1
				CVBS2
				S-Video
				D-SUB
				DVI-D
				HotKey_Define
				RESET
				AUTO TUNE
				exit
********************************************************************************/

extern code unsigned char ENG_LANG[];
extern code unsigned char FRA_LANG[];
extern code unsigned char DEU_LANG[];
extern code unsigned char ESP_LANG[];
extern code unsigned char ITA_LANG[];
extern code unsigned char JAP_LANG[];
extern code unsigned char CHI_LANG[];
extern code unsigned char RUS_LANG[];

extern code unsigned char ENG_EXIT[];
extern code unsigned char FRA_EXIT[];
extern code unsigned char DEU_EXIT[];
extern code unsigned char ESP_EXIT[];
extern code unsigned char ITA_EXIT[];
extern code unsigned char JAP_EXIT[];
extern code unsigned char CHI_EXIT[];
extern code unsigned char RUS_EXIT[];

void ShowSetupMode()
{
	Byte code ENG_CVBS1[]={
	5,"CVBS1"
	};
	Byte code ENG_CVBS2[]={
	5,"CVBS2"
	};
	Byte code ENG_SVIDEO[]={
	7,"S_Video"
	};
	Byte code ENG_DSUB[]={
#if vga0_YPbPr1
	5,"YPbPr"
#else
	3,"VGA"
#endif
	};
	Byte code ENG_DVID[]={
	5,"DVI_D"
	};
	Byte code ENG_HOTKEY_DEFINE[]={
	13,"HotKey Define"
	};
	//Byte code ENG_PHOTO_SENSER[]={
	//12,"Light Sensor"
	//};
	
	Byte code ENG_AUTOPOWERON[]={
	16,"Signal_in & Play"
	};
	Byte code ENG_AUTOCOLOR[]={
	16,"Adjust red Color"
	};
code unsigned char *SetupTab[][9]={
	{ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1,ENG_CVBS1},
	{ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2,ENG_CVBS2},
	{ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO,ENG_SVIDEO},
	{ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB,ENG_DSUB},
	{ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID,ENG_DVID},
	//{ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,
	// ENG_PHOTO_SENSER,ENG_PHOTO_SENSER,ENG_PHOTO_SENSER},
	{ENG_AUTOPOWERON,ENG_AUTOPOWERON,ENG_AUTOPOWERON,ENG_AUTOPOWERON,ENG_AUTOPOWERON,
	ENG_AUTOPOWERON,ENG_AUTOPOWERON,ENG_AUTOPOWERON,ENG_AUTOPOWERON},
	{ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,
	 ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE,ENG_HOTKEY_DEFINE},
	{ENG_LANG,FRA_LANG,DEU_LANG,ESP_LANG,ITA_LANG,JAP_LANG,RUS_LANG,CHI_LANG,CHI_LANG},
	{ENG_EXIT,FRA_EXIT,DEU_EXIT,ESP_EXIT,ITA_EXIT,JAP_EXIT,RUS_EXIT,CHI_EXIT,CHI_EXIT},
	{ENG_AUTOCOLOR,ENG_AUTOCOLOR,ENG_AUTOCOLOR,ENG_AUTOCOLOR,ENG_AUTOCOLOR,ENG_AUTOCOLOR,ENG_AUTOCOLOR,
	ENG_AUTOCOLOR,ENG_AUTOCOLOR},
};

	Byte i,End,x,y,*Offset,Length;
	Word code SetupModeAddr[11]={ROW3+2,ROW4+2,ROW5+2,ROW6+2,ROW7+2,ROW8+2,ROW9+2,ROW10+2,ROW11+2,ROW12+2,ROW13+2};

	Osd_On(enWIN4);
	
	i = MenuPtr.Ptr;
	x = SetupModeAddr[i] % OSD_WIDTH;
	y = SetupModeAddr[i] / OSD_WIDTH;
	
	if(AutoColorFlag && FuncBuf[pVIDEOSOURCE]<= YPbPrinput)
		End = 10;
	else
	End = 9;
	//End = 10; take off photo sensor

	for(i=0;i<End;i++)
		{
		Offset = SetupTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
/* take off photo sensor from osd setup page	20090620 kevin	
#if decoder_TECHWELL
		else  if(i == 5 && (!sensorFlag))
#else
		else  if(i<=2 || (i == 5 && !sensorFlag))
#endif
*/
#if video_decoder == tw9906
		else if(i==1)
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);			
#endif
			
#if !decoder_TECHWELL
		else  if(i<=2)
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);
#endif
		else if(FuncBuf[pVIDEOSOURCE] == i && MenuPtr.Level == 0)
			WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		
		WriteSeqIndexPort(SetupModeAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	if(MenuPtr.Level == 1) 
		Osd_On(enWIN4);
}
//COLOR
extern code unsigned char ENG_COLOR[];
extern code unsigned char FRA_COLOR[];
extern code unsigned char DEU_COLOR[];
extern code unsigned char ESP_COLOR[];
extern code unsigned char ITA_COLOR[];
extern code unsigned char JAP_COLOR[];
extern code unsigned char CHI_COLOR[];
extern code unsigned char RUS_COLOR[];
//VOLUME
extern code unsigned char ENG_VOLUME[];
extern code unsigned char FRA_VOLUME[];
extern code unsigned char DEU_VOLUME[];
extern code unsigned char ESP_VOLUME[];
extern code unsigned char ITA_VOLUME[];
extern code unsigned char JAP_VOLUME[];
extern code unsigned char RUS_VOLUME[];
extern code unsigned char CHT_VOLUME[];
extern code unsigned char CHI_VOLUME[];
// Mute
extern code unsigned char ENG_MUTE[];
extern code unsigned char FRA_MUTE[];
extern code unsigned char DEU_MUTE[];
extern code unsigned char ESP_MUTE[];
extern code unsigned char ITA_MUTE[];
extern code unsigned char JAP_MUTE[];
extern code unsigned char RUS_MUTE[];
extern code unsigned char CHT_MUTE[];
extern code unsigned char CHI_MUTE[];

#ifdef CTA

void ShowOption()
{
	Byte code ENG_REVERSE[]={
	7,"REVERSE"
	};
	Byte code ENG_MIRROR[]={
	7,"MIRROR "
	};

code unsigned char *SetupTab[][9]={
	{ENG_VOLUME,FRA_VOLUME,DEU_VOLUME,ESP_VOLUME,ITA_VOLUME,JAP_VOLUME,RUS_VOLUME,CHT_VOLUME,CHI_VOLUME},
	{ENG_MUTE,FRA_MUTE,DEU_MUTE,ESP_MUTE,ITA_MUTE,JAP_MUTE,RUS_MUTE,CHT_MUTE,CHI_MUTE},
	{ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE},
	{ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR},
	{ENG_COLOR,FRA_COLOR,DEU_COLOR,ESP_COLOR,ITA_COLOR,JAP_COLOR,RUS_COLOR,CHI_COLOR,CHI_COLOR},
	{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,},
	{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,},
	{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,},
	{ENG_EXIT,FRA_EXIT,DEU_EXIT,ESP_EXIT,ITA_EXIT,JAP_EXIT,RUS_EXIT,CHI_EXIT,CHI_EXIT},
};

	Byte i,End,x,y,*Offset,Length;
	Word code SetupModeAddr[9]={ROW3+2,ROW4+2,ROW5+2,ROW6+2,ROW7+2,ROW8+2,ROW9+2,ROW10+2,ROW11+2};

	Osd_On(enWIN4);

	i = MenuPtr.Ptr;
	x = SetupModeAddr[i] % OSD_WIDTH;
	y = SetupModeAddr[i] / OSD_WIDTH;
	End = 9;

	for(i=0;i<End;i++)
	{
		if(i>4 && i< 8)
			i=8;
		
		Offset = SetupTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else if(FuncBuf[pVIDEOSOURCE] == i && MenuPtr.Level == 0)
			WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else if(audioFlag && (i == 2 || i == 3))
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);
		else if(!audioFlag &&(i <= 3))
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);

		// kevin 20100318 for color temp user defined
		if(i== 8 && FuncBuf[pCOLORTEMP]!=4)
		{
			WriteSeqIndexPort(ROW11+2,tySRAM_CODE_CC_CD,SPACE,4);	
			WriteSeqIndexPort(ROW8+2,tySRAM_CODE_CC_CD,Offset,Length); 
		}else{
			WriteSeqIndexPort(ROW8+5,tySRAM_CODE_CC_CD,SPACE,1);
			WriteSeqIndexPort(SetupModeAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
			}
		}
	if(MenuPtr.Level == 1) 
		Osd_On(enWIN4);
}
#else
void ShowOption()
{
	Byte code ENG_REVERSE[]={
	7,"REVERSE"
	};
	Byte code ENG_MIRROR[]={
	7,"MIRROR "
	};

code unsigned char *SetupTab[][9]={
	{ENG_VOLUME,FRA_VOLUME,DEU_VOLUME,ESP_VOLUME,ITA_VOLUME,JAP_VOLUME,RUS_VOLUME,CHT_VOLUME,CHI_VOLUME},
	{ENG_MUTE,FRA_MUTE,DEU_MUTE,ESP_MUTE,ITA_MUTE,JAP_MUTE,RUS_MUTE,CHT_MUTE,CHI_MUTE},
	{ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE,ENG_REVERSE},
	{ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR,ENG_MIRROR},
	{ENG_COLOR,FRA_COLOR,DEU_COLOR,ESP_COLOR,ITA_COLOR,JAP_COLOR,RUS_COLOR,CHI_COLOR,CHI_COLOR},
	{ENG_EXIT,FRA_EXIT,DEU_EXIT,ESP_EXIT,ITA_EXIT,JAP_EXIT,RUS_EXIT,CHI_EXIT,CHI_EXIT},
};

	Byte i,End,x,y,*Offset,Length;
	Word code SetupModeAddr[9]={ROW3+2,ROW4+2,ROW5+2,ROW6+2,ROW7+2,ROW8+2};

	Osd_On(enWIN4|enWIN2);

	i = MenuPtr.Ptr;
	x = SetupModeAddr[i] % OSD_WIDTH;
	y = SetupModeAddr[i] / OSD_WIDTH;
	End = 6;

	for(i=0;i<End;i++)
		{
		Offset = SetupTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else if(FuncBuf[pVIDEOSOURCE] == i && MenuPtr.Level == 0)
			WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else if(audioFlag && (i == 2 || i == 3))
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);
		else if(!audioFlag &&(i <= 3))
			WriteWordIIC563(0x0cc,fGRAY|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		
		WriteSeqIndexPort(SetupModeAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	if(MenuPtr.Level == 1) 
		Osd_On(enWIN4);
}
#endif
#endif
/*
void ShowTextGraphicMode()
{
	Byte code TextModeTab[]={"Text"};
	Byte code GraphicModeTab[]={"Graphic"};
	if(TextMode && GTmodeEn)
		WriteWordIIC563(0x0cc,fRED|fBLACK);
	else
		WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
	WriteSeqIndexPort(ROW9+16,tySRAM_CODE_CC_CD,TextModeTab,4);
	if(!TextMode && GTmodeEn)
		WriteWordIIC563(0x0cc,fRED|fBLACK);
	else
		WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
	WriteSeqIndexPort(ROW9+7,tySRAM_CODE_CC_CD,GraphicModeTab,7);
}
*/
/*
void ShowYesNo(Byte Ptr)
{
code unsigned char ENG_NO[]={
	2,"No"
};
code unsigned char FRA_NO[]={
	3,"Non"
};
code unsigned char DEU_NO[]={
	4,"Nein"
};
code unsigned char ESP_NO[]={
	2,"No."
};
code unsigned char ITA_NO[]={
	2,"No"
};
code unsigned char JAP_NO[]={
	2,"No"
};
code unsigned char RSU_NO[]={
	3,_H,_e,_P05
};
code unsigned char CHT_NO[]={
	1,_C49
};
code unsigned char CHI_NO[]={
	1,_C49
};

code unsigned char ENG_YES[]={
	3,"Yes"
};
code unsigned char FRA_YES[]={
	3,"Oui"
};
code unsigned char DEU_YES[]={
	2,"Ja"
};
code unsigned char ESP_YES[]={
	2,"Si"
};
code unsigned char ITA_YES[]={
	2,"Si"
};
code unsigned char JAP_YES[]={
	3,"Yes"
};
code unsigned char RSU_YES[]={
	2,_P0f,_a
};
code unsigned char CHT_YES[]={
	1,_C4a
};
code unsigned char CHI_YES[]={
	1,_C4a
};

code unsigned char *YesNoTab[][9]={
	{ENG_NO,FRA_NO,DEU_NO,ESP_NO,ITA_NO,JAP_NO,RSU_NO,CHT_NO,CHI_NO},
	{ENG_YES,FRA_YES,DEU_YES,ESP_YES,ITA_YES,JAP_YES,RSU_YES,CHT_YES,CHI_YES}
};
	Byte i;
	Word code YesNoAddr[]={ROW8+17,ROW8+24};
	Byte *Offset,Length;

	
	for(i=0;i<2;i++)
	{
		Offset = YesNoTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		if(Ptr == i)
			if(((FuncPtr == pRECALL && MenuPtr.Ptr == 4)) && MenuPtr.Level == 1)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else
				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		WriteSeqIndexPort(YesNoAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
	}
}
*/
void ShowLanguage()
{
	code unsigned char ENGStr[]={
		7,"English"
	};

	Byte code *LanguageTab[][9]={
		{ENGStr,ENGStr,ENGStr,ENGStr,ENGStr,ENGStr,RUS_Russian,ENGStr,ENGStr},
		{RUS_English,RUS_English,RUS_English,RUS_English,RUS_English,RUS_English,RUS_Russian,RUS_English,RUS_English}
	};

	Byte *Offset,Length,language,clr_len;
	Word Addr;


		//if(FuncBuf[pLANGUAGE] == Language_ENG)
		//{
			language =0;
			Addr = ROW12+20;
			clr_len=10;
		//}else{
		//	language = 0;
		//	Addr = ROW12+20;
		//	clr_len=10;
		//}
		
		Offset = LanguageTab[language][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		
		if(PageOSDSetup[MenuPtr.Ptr]== pLANGUAGE && MenuPtr.Level == Level_SubItem)
			WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);	

		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,SPACE,clr_len);
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Offset,Length);

}
#if 0
void ShowColorMenu()
{
	Byte i,*Offset,Length;
	Byte code ColorTab[][7]={{"9300K  "},{"7500K  "},{"6500K  "},{"default"},{"USER   "}};
	Word RColor,GColor,BColor;
	Word code RGBAddr[3]={ROW8+2,ROW9+2,ROW10+2};
	Word Attr;

	if(FuncPtr == pCOLORTEMP)
		if(MenuPtr.Level == 1)
		WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else
		WriteWordIIC563(0x0cc,fGREEN|fBLACK);
	else			
		WriteWordIIC563(0x0cc,fWHITE|fBLACK);
	WriteSeqIndexPort(ROW7+23,tySRAM_CODE_CC_CD,ColorTab[FuncBuf[pCOLORTEMP]],7);

	// if color temp == user mode
	if(FuncBuf[pCOLORTEMP]==4)
	{
		for(i=0;i<=2;i++)
		{
			// MenuPtr.Ptr = 5 = pRCOLOR
			// MenuPtr.Ptr = 6 = pGCOLOR
			// MenuPtr.Ptr = 7 = pBCOLOR
			if((i == (MenuPtr.Ptr-5)) && (MenuPtr.Page== 2))
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else			
				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
			
			Offset = Tab_RGB[FuncBuf[pLANGUAGE]*6+i];
			Length = *Offset;
			Offset++;
			WriteSeqIndexPort(RGBAddr[i],tySRAM_CODE_CC_CD,Offset,Length);		
		}
		//Show user pRCOLOR,pGCOLOR,pBCOLOR
		if(MenuPtr.Ptr > 4 && MenuPtr.Ptr < 8)
		{
			RColor = FuncBuf[pRCOLOR];
			GColor = FuncBuf[pGCOLOR];
			BColor = FuncBuf[pBCOLOR];
		}
		else{
			RColor = Read24C16(ep_Color5_R);
			GColor = Read24C16(ep_Color5_G);
			BColor = Read24C16(ep_Color5_B);
		}
		
		GetFuncRange(pRCOLOR);
		
		Attr = fWHITE|fBLACK;		
		//if r color is selected then change to magenta color
		if(MenuPtr.Ptr == 5)
			Attr = fMAGENTA|fBLACK;	
		// show r color value
		ShowValue(RGBAddr[0]+21,Attr,RColor);
		
		//if g color is selected then change to magenta color
		Attr = fWHITE|fBLACK;
		if(MenuPtr.Ptr == 6)
			Attr = fMAGENTA|fBLACK;		
		// show g color value
		ShowValue(RGBAddr[1]+21,Attr,GColor);
		
		//if b color is selected then change to magenta color
		Attr = fWHITE|fBLACK;
		if(MenuPtr.Ptr == 7)
			Attr = fMAGENTA|fBLACK;	
		// show b color value
		ShowValue(RGBAddr[2]+21,Attr,BColor);
		
	}
	else{
		WriteSeqIndexPort(ROW8+6,tySRAM_CODE_CC_CD,SPACE,23);
		WriteSeqIndexPort(ROW9+2,tySRAM_CODE_CC_CD,SPACE,27);
		WriteSeqIndexPort(ROW10+2,tySRAM_CODE_CC_CD,SPACE,27);
	}
	
}	
#else
void ShowColorMenu()
{
	code unsigned char ENG_CT9300K[]={
		5,"9300K"
	};
	code unsigned char ENG_CT7500K[]={
		5,"7500K"
	};
	code unsigned char ENG_CT6500K[]={
		5,"6500K"
	};
	code unsigned char ENG_DEFAULT[]={
		7,"Default"
	};
	
	Byte code *ColorTab[][4]={
							{ ENG_CT9300K,ENG_CT7500K,ENG_CT6500K,ENG_DEFAULT},
							{ ENG_CT9300K,ENG_CT7500K,ENG_CT6500K,RUS_Default},
						};
	
	Byte *Offset,Length,language,clr_len;
	Word Addr;
	//printf("FuncPtr=%d \n",(unsigned short)FuncPtr);

	if(FuncBuf[pLANGUAGE] == Language_ENG)
	{
		language = 0;
		Addr = ROW14+22;
		clr_len = 8;
	}else{
		language = 1;
		Addr = ROW14+23;
		clr_len = 12;
	}

	if(FuncBuf[pCOLORTEMP] > ColorTemp_Default)
		FuncBuf[pCOLORTEMP] = ColorTemp_Default;	

	Offset = ColorTab[language][FuncBuf[pCOLORTEMP]];
	Length = *Offset;
	Offset++;
		
	if(PageImageSetup[MenuPtr.Ptr] == pCOLORTEMP && MenuPtr.Level == Level_SubItem )
	{
		WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
	}else{		
		WriteWordIIC563(0x0cc,fWHITE|fBLACK);
	}		

	WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,SPACE,clr_len);
	WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Offset,Length);

}

#endif
#if 0
void ShowRGBMenu()		
{
	//Byte *Offset,i,Length;
	Word code RGBAddr[6]={ROW3+4,ROW4+4,ROW5+4,ROW6+4,ROW7+4,ROW8+4};
/*
	for(i=1;i<4;i++)
		{
		if((((i+3) == MenuPtr.Ptr) && (MenuPtr.Level == 2))||((i == MenuPtr.Ptr+1) && (MenuPtr.Level == 1)&& (MenuPtr.Menu == 2)))
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		Offset = Tab_RGB[FuncBuf[pLANGUAGE]*3+(i-1)];
		Length = *Offset;
		Offset++;
		if(MenuPtr.Menu != 2)			
			WriteSeqIndexPort(RGBAddr[i-1],tySRAM_CODE_CC_CD,Offset,Length);
		else
			WriteSeqIndexPort(RGBAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
*/
//	if(MenuPtr.Menu != 2)
//	{
		
		#if PRINT_MESSAGE == 1
		printf("control %d\n",(unsigned short)MenuPtr.Ptr);
		#endif
		switch(MenuPtr.Ptr){
			case 4:
//				if((MenuPtr.Ptr == 4) && (MenuPtr.Level == 2)){
					//ShowBar(RGBAddr[0]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pRCOLOR]);
					ShowValue(RGBAddr[0]+17,fMAGENTA|fBLACK,FuncBuf[pRCOLOR]);		
//				}
//				else{
//					ShowBar(RGBAddr[0]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pRCOLOR]);
//					ShowValue(RGBAddr[0]+19,fWHITE|fBLACK,FuncBuf[pRCOLOR]);
//				}
			break;
			case 5:
//				if((MenuPtr.Ptr == 5) && (MenuPtr.Level == 2)){
					//ShowBar(RGBAddr[1]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pGCOLOR]);
					ShowValue(RGBAddr[1]+17,fMAGENTA|fBLACK,FuncBuf[pGCOLOR]);
//				}
//				else{
//					ShowBar(RGBAddr[1]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGCOLOR]);
//					ShowValue(RGBAddr[1]+19,fWHITE|fBLACK,FuncBuf[pGCOLOR]);
//				}
				break;
			case 6:
//				if((MenuPtr.Ptr == 6) && (MenuPtr.Level == 2)){
					//ShowBar(RGBAddr[2]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pBCOLOR]);
					ShowValue(RGBAddr[2]+17,fMAGENTA|fBLACK,FuncBuf[pBCOLOR]);
//				}
//				else{
//					ShowBar(RGBAddr[2]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBCOLOR]);
//					ShowValue(RGBAddr[2]+19,fWHITE|fBLACK,FuncBuf[pBCOLOR]);
//				}
				break;
			case 7:
				//	ShowBar(RGBAddr[3]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
				//	ShowValue(RGBAddr[3]+17,fMAGENTA|fBLACK,FuncBuf[pFTCORRECTION]);
				break;
			case 8:
				//	ShowBar(RGBAddr[4]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
				//	ShowValue(RGBAddr[4]+17,fMAGENTA|fBLACK,FuncBuf[pGREENENHANCE]);

				break;
			case 9:
				//	ShowBar(RGBAddr[5]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
				//	ShowValue(RGBAddr[5]+17,fMAGENTA|fBLACK,FuncBuf[pEXIT]);
				break;
		}
//		}
//	else
//	{
		//ShowBar(RGBAddr[1]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pRCOLOR]);
		//ShowBar(RGBAddr[2]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGCOLOR]);
		//ShowBar(RGBAddr[3]+10,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBCOLOR]);
//		ShowValue(RGBAddr[1]+19,fWHITE|fBLACK,FuncBuf[pRCOLOR]);
//		ShowValue(RGBAddr[2]+19,fWHITE|fBLACK,FuncBuf[pGCOLOR]);
//		ShowValue(RGBAddr[3]+19,fWHITE|fBLACK,FuncBuf[pBCOLOR]);
//	}
}
#endif
void ClearOSD(void)
{
// Clear osd ram
	WriteWordIIC563(0x0cc,fBLACK|bBLACK|caMIX);
	WriteIIC563(0x0ce,0x20);
	WriteIIC563(0x0cf,0x01);
	// Wait for Clear Ready....
	LocalTimer = 10;
	while(((ReadIIC563(0x0cf) & BIT_0) != 0) && LocalTimer != 0){
		if(FuncBuf[pVIDEOSOURCE] <= DVIinput) //jacky20040324
			CheckModeChange();
	}
	//Sleep(1);
}

void ClearTextArea()
{
	Byte i;
	
	WriteWordIIC563(0x0cc,fBLACK|fBLACK);
	
	for(i=0;i<OSD_HIGH;i++)
	{
		WriteSeqIndexPort((ROW1*i),tySRAM_CODE_CC_CD,SPACE,OSD_WIDTH);
	}
}
/*
void ClearLine(Byte Line)
{
	WriteWordIIC563(0x0cc,fBLACK|fBLACK);
	WriteSeqIndexPort(Line*OSD_WIDTH+1,tySRAM_CODE_CC_CD,SPACE,29);
}
*/

void OsdInit(void)
{
	Byte i;

	
	Sleep(50); // don't take away, wait until the iic bus stable
	
	FuncBuf[pTRANSOSD] = Read24C16(ep_Osd_Effect);
	FuncBuf[pOSDRotate]= Read24C16(ep_Osd_Rotate);
	SetOsdTrans();
	
	ClearOSD();

	
	WriteIIC563(0x080,FuncBuf[pOSDRotate]);
	WriteIIC563(0x095,0x55);
	WriteIIC563(0x083,OSD_WIDTH-1);
	WriteIIC563(0x086,OSD_HIGH-1);
	WriteIIC563(0x0a2,0);
	WriteWordIIC563(0x088,ONE_BIT_START_ADDR);
	WriteWordIIC563(0x08a,TWO_BIT_START_ADDR);
	WriteWordIIC563(0x08c,FOUR_BIT_START_ADDR);

	

	// ¸ü¤J 1bit palette
	for(i=0;i<13;i++)
		WriteWordIndexPort(i,tySRAM_PALETTE,Palette1Bit[i]);
	
	// ¸ü¤J 2bit palette
	//for(i=0;i<14*4;i++)
	//	WriteWordIndexPort(i+OFFSET_2BIT,tySRAM_PALETTE,Palette2Bit[i]);

	// ¸ü¤J logo 2bit palette
	//for(i=0;i< 4;i++)
	//	WriteWordIndexPort(i+14+OFFSET_2BIT,tySRAM_PALETTE,logo_Palette2Bit[i]);
	
	// ¸ü¤J 4bit palette
	for(i=0;i<10*16;i++)
		WriteWordIndexPort(i+OFFSET_4BIT,tySRAM_PALETTE,test_Palette4Bit[i]);

	for(i=0;i<3*4;i++)
		WriteWordIndexPort(i+OFFSET_2BIT,tySRAM_PALETTE,bar_Palette4Bit[i]);
	

	//if(FuncBuf[pLANGUAGE]== Language_ENG){

		// ¸ü¤J 1bit Font
		WriteSeqIndexPort(0,tySRAM_1BIT,OneBitFont,MaxOneBitFont*27);
	//}else{
		//WriteSeqIndexPort(0xa0,tySRAM_1BIT,OneBitFont2,48*27);
		WriteSeqIndexPort(RussianFontStart,tySRAM_1BIT,OneBitFontPY,46*27);
	//}
	// ¸ü¤J 1bit Font OSD Tital
	//WriteSeqIndexPort(0x100,tySRAM_1BIT,Title_OneBitFont,89*27);
	
	WriteSeqIndexPort(OneBitIconStart,tySRAM_1BIT,OneBitIcon,2*27);
	//WriteSeqIndexPort(0x100,tySRAM_1BIT,OneBitIcon,39*27);

	// ¸ü¤J 2bit Font
	//WriteSeqIndexPort(0	,tySRAM_2BIT,	TwoBitFont,15*54);
	//WriteSeqIndexPort(15	,tySRAM_2BIT,	TwoBitIcon,36*54);
	WriteSeqIndexPort(0,tySRAM_2BIT,bar_TwoBitIcon,20*54);

	// ¸ü¤J logo 4bit Font
	WriteSeqIndexPort(0,tySRAM_4BIT,test_FourBitFont,20*108);


	OsdPosition();
	MenuPtr.Menu = 0;
	MenuPtr.Level = 0;
	MenuPtr.Ptr = 0;
	MenuPtr.Op = 0;
	Osd_re_build=0;

}

#if 1
void OsdPosition()
{
	long OSD_HPOS,OSD_VPOS;
	Byte Temp;
	
	Temp = ReadIIC563(0x080);
	WriteIIC563(0x080,(Temp & (~enOSD))); // OSD disable
	
	
	OSD_HPOS = (PanelWidth-(OSD_WIDTH*12));
	OSD_HPOS = OSD_HPOS *  FuncBuf[pOSDHPOSITION];
	OSD_HPOS = OSD_HPOS /100;
	
	OSD_VPOS = (PanelHeight-(OSD_HIGH*18));
	OSD_VPOS = OSD_VPOS  *  FuncBuf[pOSDVPOSITION];	
	OSD_VPOS = OSD_VPOS /100;

	if(OSD_HPOS > (PanelWidth-(OSD_WIDTH*12)))
	{
		OSD_HPOS = (PanelWidth-(OSD_WIDTH*12));
	}
	
	if(OSD_VPOS > (PanelHeight-(OSD_HIGH*18)))
	{
		OSD_VPOS = (PanelHeight-(OSD_HIGH*18));
	}
	WriteWordIIC563(0x081,(unsigned short)OSD_HPOS);
	WriteWordIIC563(0x084,(unsigned short)OSD_VPOS);
	WriteIIC563(0x080,Temp); // OSD enable

	//printf("X:%d , osd x=%d \n",FuncBuf[pOSDHPOSITION],(unsigned short)OSD_HPOS);
	//printf("Y:%d , osd y=%d \n",FuncBuf[pOSDVPOSITION],(unsigned short)OSD_VPOS);

}
#else

void OsdPosition()
{
/*	
	#ifdef X15
	Word code OSD_PX[]={
		64,600,332,64,600
	};
	Word code OSD_PY[]={
		64,64,249,434,434
	};
	Word code OSD_PY1[]={
		64,64,330,434,434
	};
	#endif
	#ifdef X17
	Word code OSD_PX[]={
		80,840,460,80,840
	};
	Word code OSD_PY[]={
		85,85,375,669,669
	};
	Word code OSD_PY1[]={
		85,85,458,669,669
	};
	#endif
	#ifdef X19
	Word code OSD_PX[]={
		80,840,460,80,840
	};
	Word code OSD_PY[]={
		85,85,375,669,669
	};
	Word code OSD_PY1[]={
		85,85,458,669,669
	};
	#endif
	#ifdef X21
	Word code OSD_PX[]={
		99,1129,614,99,1129
	};
	Word code OSD_PY[]={
		99,99,465,831,831
	};
	Word code OSD_PY1[]={
		99,99,548,831,831
	};
	#endif
*/	
	Word code OSD_PX[]={
		OSD_X1, OSD_X2, OSD_X3, OSD_X1, OSD_X2
	};
	Word code OSD_PY[]={
		OSD_Y1, OSD_Y1, OSD_Y3, OSD_Y2, OSD_Y2
	};
	Word code OSD_PY1[]={
		OSD_Y1, OSD_Y1, OSD_Y4, OSD_Y2, OSD_Y2
	};

	Word OSD_HPOS,OSD_VPOS;
	Byte Temp;

	Temp = ReadIIC563(0x080);
	WriteIIC563(0x080,(Temp & (~enOSD))); // OSD disable
	OSD_HPOS = OSD_PX[FuncBuf[pOSDHPOSITION]];
	//if(MenuPtr.Menu == 3)
	//	OSD_VPOS = OSD_PY1[FuncBuf[pOSDHPOSITION]];
	//else
		OSD_VPOS = OSD_PY[FuncBuf[pOSDHPOSITION]];
	
	if(OSD_HPOS < (PanelWidth-OSD_WIDTH*12))
		WriteWordIIC563(0x081,OSD_HPOS);
	else
		WriteWordIIC563(0x081,PanelWidth-OSD_WIDTH*12);
	
	if(OSD_VPOS < (PanelHeight-OSD_HIGH*18))
		WriteWordIIC563(0x084,OSD_VPOS);
	else
		WriteWordIIC563(0x084,PanelHeight-OSD_HIGH*18);

	WriteIIC563(0x080,Temp); // OSD enable
/*
	if(FuncBuf[pOSDHPOSITION] < (PanelWidth-OSD_WIDTH*12)/5)
		WriteWordIIC563(0x081,FuncBuf[pOSDHPOSITION]*5);
	else
		WriteWordIIC563(0x081,PanelWidth-OSD_WIDTH*12);
	if(FuncBuf[pOSDVPOSITION] < (PanelHeight-OSD_HIGH*18)/5)
		WriteWordIIC563(0x084,FuncBuf[pOSDVPOSITION]*5);
	else
		WriteWordIIC563(0x084,PanelHeight-OSD_HIGH*18);
*/
}
#endif

void Osd_On(Byte Win)
{
	Byte tmp;
	tmp = ReadIIC563(0x080);
	tmp |=(enOSD|Win); 
	WriteIIC563(0x080,tmp);
	OsdEnable = 1;
}

void Osd_Off(void)
{
	WriteIIC563(0x080,0);
	MenuPtr.Menu = 0;
	MenuPtr.Level = 0;
	MenuPtr.Ptr = 0;
	MenuPtr.Op = 0;
	OsdEnable = 0;
}
/* kevin take it off 06/28/2007
void ShowIcon(Byte Ptr)
{
	Word Addr,Attr;
	Byte Icon;

	Icon = MenuItem[Ptr].Icon;
	Addr = MenuItem[Ptr].Address;
	//if(Icon == 6)///1bit
	//	{
	//	Attr = ((Icon-6)*16)<<8;
		//Icon = Icon -6;
	//	WriteWordIIC563(0x0cc,Attr|caFOUR_BIT);
	//	}
	//else
	//	{
		Attr = (Icon*4+OFFSET_2BIT+8)<<8;
	//	}

	Icon = Icon*6+15;
	if(MenuPtr.Page == Ptr)
		{
		if(Icon == 51)///1bit
		{
			Icon = 0;
			WriteWordIIC563(0x0cc,Attr|caONE_BIT_512|caBLINK);
		}
		else
			WriteWordIIC563(0x0cc,Attr|caTWO_BIT|caBLINK);
		//WriteWordIIC563(0x0cc,fCYAN|caONE_BIT_512);
		//WriteWordIIC563(0x0cc,fCYAN|caTWO_BIT);
		}
	else
		{
		if(Icon == 51)///1bit
		{
			Icon = 0;
			WriteWordIIC563(0x0cc,Attr|caONE_BIT_512);
		}
		else
			WriteWordIIC563(0x0cc,Attr|caTWO_BIT);
		//WriteWordIIC563(0x0cc,fWHITE|caONE_BIT_512);
		//WriteWordIIC563(0x0cc,fWHITE|caTWO_BIT);
		}
	WriteIndexPort(Addr,tySRAM_CODE_CC_CD,Icon);
	WriteIndexPort(Addr+1,tySRAM_CODE_CC_CD,Icon+1);
	WriteIndexPort(Addr+2,tySRAM_CODE_CC_CD,Icon+2);
	WriteIndexPort(Addr+ROW1,tySRAM_CODE_CC_CD,Icon+3);
	WriteIndexPort(Addr+ROW1+1,tySRAM_CODE_CC_CD,Icon+4);
	WriteIndexPort(Addr+ROW1+2,tySRAM_CODE_CC_CD,Icon+5);
}
*/
void ShowMain(Byte Page)
{
	//Byte i;

	WriteIIC563(0x080,0); // OSD disable

	//if(FuncBuf[pLANGUAGE] == Language_ENG)
	//{
	//	WriteSeqIndexPort(0,tySRAM_1BIT,OneBitFont,MaxOneBitFont*27);
	//}else{
	//	WriteSeqIndexPort(RussianFontStart,tySRAM_1BIT,OneBitFontPY,47*27);
	//}
	WriteSeqIndexPort(OneBitIconStart,tySRAM_1BIT,OneBitIcon,2*27);
	
	ClearOSD();

	OsdPosition();
	
	OpenWindow(WINDOW4,WIN4_X1,WIN4_X2,WIN4_Y1,WIN4_Y2,WIN4_ATTR,WIN4_COLOR);
	
	SetRotate();
	
	SetOsdTrans();

	ShowSubMenu(Page,OSD_Page_MainMenu);

	WriteIIC563(0xcc,caTWO_BIT);
	
	//for(i=0;i< 40;i++)
	//{
	//	WriteIndexPort(i,tySRAM_CODE_CC_CD,i);
	//}
}
/*
void ShowTvSystem()
{
#if video_decoder == tw9906
	Byte code SourceTab1[][7]={" D-SUB "," YPbPr ","  DVI  "," AV1   "," CCD   ","S-Video"};
#else
	Byte code SourceTab1[][7]={" D-SUB "," YPbPr ","  DVI  "," AV1   "," AV2   ","S-Video"};
#endif
	Byte code TvTab[][9]={"PAL 4.43 ","NTSC 4.43"," NTSC N  ","  SECAM  "," UNKNOW  "," SECAM   ","  MONO   ",
						  " NTSC M  ","PAL 4.43 ","NTSC 4.43","  PAL M  "," NTSC J  "," UNKNOW  ","  MONO   "};
	int temprow;
	if(MenuPtr.Menu == 3)
		temprow = ROW2;
	else
		temprow = ROW14;
	WriteWordIIC563(0x0cc,RES_COLOR);
	//WriteSeqIndexPort(MAINMENU_RES_ADDR-3,tySRAM_CODE_CC_CD,TvTab[TvMode],9);
	//WriteSeqIndexPort(MAINMENU_RES_ADDR+7,tySRAM_CODE_CC_CD,SourceTab1[FuncBuf[pVIDEOSOURCE]],7);
	WriteSeqIndexPort(temprow+12,tySRAM_CODE_CC_CD,TvTab[TvMode],9);
	WriteSeqIndexPort(temprow+21,tySRAM_CODE_CC_CD,SourceTab1[FuncBuf[pVIDEOSOURCE]],7);
}
*/
/*
void ShowFactMenu()
{
	Byte i;
	Byte code FactTab[][7]={{"AdcOft "},{"AdcGain"},{"9300K  "},{"7500K  "},{"6500K  "},{"Cont.  "},{"Brig.  "}};
	Word code FactAddr[]={ROW5+1,ROW6+1,ROW7+1,ROW8+1,ROW9+1,ROW10+1,ROW11+1};
	Byte *Offset,Length;//,Win_X1,Win_X2,Win_Y1,Win_Y2;

	ClearOSD();
	ShowOSDFrame(0);
	WriteWordIIC563(0x081,OSD_FACT_HPOS);
	WriteWordIIC563(0x084,OSD_FACT_VPOS);
	OpenWindow(WINDOW4,WIN4_X1,WIN4_X2,WIN4_Y1,WIN4_Y2,WIN4_ATTR,WIN4_COLOR);
	//OpenWindow(WINDOW3,WIN3_X1,WIN3_X2,WIN3_Y1,WIN3_Y2,WIN3_ATTR,WIN3_COLOR);
	ShowVersion();
	ShowMode(ROW12+1,MODE_COLOR);
	//ShowBackLightTime();
	for(i=0;i<7;i++)
	{
		if(i < 3)
			WriteWordIIC563(0x0cc,fYELLOW|bBLUE);
		else			
		WriteWordIIC563(0x0cc,fWHITE|bBLUE);
		WriteSeqIndexPort(FactAddr[i],tySRAM_CODE_CC_CD,FactTab[i],7);
	}
	for(i=0;i<MAX_FACT_ITEM;i++)
	{
//		Addr = FactItem[i].Address;
		Offset = FactItem[i].Tab;
		Length = *Offset;
		Offset++;
		if(i == MenuPtr.Ptr && MenuPtr.Level == 0)
		{
			WriteWordIIC563(0x0cc,fYELLOW|bLBLUE);
			WriteSeqIndexPort(FactItem[i].Address,tySRAM_CODE_CC_CD,Offset,Length);
		}
		else
		{
			WriteWordIIC563(0x0cc,fYELLOW|bBLUE);
			WriteSeqIndexPort(FactItem[i].Address,tySRAM_CODE_CC_CD,Offset,Length);
		}
		if(i > 2 && i != 26){
			if(i >= 11 && i <= 19){
				FuncBuf[pCOLORTEMP] = (i - 11)/3;
				LoadColor(FuncBuf[pCOLORTEMP]);
			}
			else{
				FuncBuf[pCOLORTEMP] = 2;
				LoadColor(FuncBuf[pCOLORTEMP]);
			}
			FuncPtr = FactItem[i].Index;
			GetFuncRange(FuncPtr);
			//GetFuncRange(FactItem[i].Index);		
			if(i == MenuPtr.Ptr && MenuPtr.Level == 1)
			{
				ShowValue(FactItem[i].SelAddr,fYELLOW|bLBLUE,FuncBuf[FactItem[i].Index]);
			}
			else
			{
				ShowValue(FactItem[i].SelAddr,fWHITE|bBLUE,FuncBuf[FactItem[i].Index]);
			}
		}
	}

	ShowFactCursor();
}

void ShowFact()
{
	//Byte *Offset,Length,Win_X1,Win_X2,Win_Y1,Win_Y2;
	
	if(MenuPtr.Ptr > 2 && MenuPtr.Level == 1)
	{
		ShowValue(FactItem[MenuPtr.Ptr].SelAddr,fWHITE|bBLUE,FuncBuf[FactItem[MenuPtr.Ptr].Index]);
	}
	
	if(MenuPtr.Op != oDECVAL && MenuPtr.Op != oINCVAL){
		ShowFactCursor();
	}
}
void ShowFactCursor()
{
	Byte *Offset,Length,Win_X1,Win_X2,Win_Y1,Win_Y2;
	Osd_On(enWIN4); 	
	Offset = FactItem[MenuPtr.Ptr].Tab;
	Length = *Offset;
	if(MenuPtr.Level == 1 && MenuPtr.Ptr > 2){
		Win_X1 = FactItem[MenuPtr.Ptr].SelAddr % OSD_WIDTH;
		Win_X2 = Win_X1 + 2;
	}
	else{
		Win_X1 = FactItem[MenuPtr.Ptr].Address % OSD_WIDTH;
		Win_X2 = Win_X1 + Length - 1;
	}
	Win_Y1 = FactItem[MenuPtr.Ptr].SelAddr / OSD_WIDTH;
	Win_Y2 = FactItem[MenuPtr.Ptr].SelAddr / OSD_WIDTH;
	OpenWindow(WINDOW3,Win_X1,Win_X2,Win_Y1,Win_Y2,WIN3_ATTR,WIN3_COLOR);
	Osd_On(enWIN4|enWIN3);	
}
*/
/*
void RefreshIcon()
{
	Byte i,x,y;

	for(i=0;i<7;i++)
		ShowIcon(i);
	x = MenuItem[MenuPtr.Page].Address % OSD_WIDTH;
	y = MenuItem[MenuPtr.Page].Address / OSD_WIDTH;
	Osd_On(enWIN4);
	OpenWindow(WINDOW2,x,x+2,y,y+1,WIN2_ATTR,SET_ICON_COLOR);

	Osd_On(enWIN4);
}
*/
void ShowDesc(Byte Ptr,Word Addr,Word Attr)
{
	Byte *Offset,Length;
	Offset = DescTab[Ptr][FuncBuf[pLANGUAGE]];
	Length = *Offset;
	Offset++;
	WriteWordIIC563(0x0cc,Attr);
	WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Offset,Length);
}

void WriteIndexPort(Word Addr,Byte Type,Byte Ch)
{
	CheckRegPage(0xe0);
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(Type);
	IIC_Tx(Addr&0x00ff);
	IIC_Tx(Addr>>8);
	IIC_Tx(Ch);
	IIC_Stop();
}

void WriteWordIndexPort(Word Addr,Byte Type,Word Ch)
{
	CheckRegPage(0xe0);
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(Type);
	IIC_Tx(Addr&0x00ff);
	IIC_Tx(Addr>>8);
	IIC_Tx(Ch&0x00ff);
	IIC_Tx(Ch>>8);
	IIC_Stop();
}

void WriteSeqIndexPort(Word Addr,Byte Type,Byte *Ptr,Word Leng)
{
	Word i;
	CheckRegPage(0xe0);
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(Type&(~tyWITH_LANGTH));
	if(Type & tyWITH_LANGTH)	//if(tyWITH_LENGH) get string length from table 
		Leng = *Ptr++;
	IIC_Tx(Addr&0x00ff);
	IIC_Tx(Addr>>8);
	for(i=0; i<Leng; i++)
		IIC_Tx(*Ptr++);
	IIC_Stop();
}

void OpenWindow(Byte Win,Byte x1,Byte x2,Byte y1,Byte y2,Byte Attr,Byte Col)
{
	CheckRegPage(0xe0);
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xa5);
	IIC_Tx(Win);
	IIC_Tx(x1);
	IIC_Tx(x2);
	IIC_Tx(y1);
	IIC_Tx(y2);
	if(FuncBuf[pTRANSOSD] != 0 && Win == WINDOW4)
		IIC_Tx(Attr|0x10);
	else
		IIC_Tx(Attr);
	IIC_Tx(Col);
	IIC_Stop();
}

void BCD(Word Value,Byte *VaSeq,Byte Zero)
{
	Byte temp,i;
	for(i=5;i>0;i--)
		{
		temp = Value % 10;
		*(VaSeq+i-1) =  temp+'0';
		Value /= 10;
		}
	for(i=0;i<(5-Zero);i++)
		{
		if(*(VaSeq+i) == '0')
			*(VaSeq+i) = ' ';
		else
			break;
		}
}

void ShowValue(Word Addr,Word Attr,Word Value)
{
	Word Percent;
	unsigned long temp;
	Byte Ptr[5];
	
	if(FuncPtr == pCONTRAST){
		Percent = Value - FuncMin;
		temp = Percent * 100;
	}
	else{
		Percent = Value;
		temp = Percent * FuncMax;
	}
	
	Percent = temp / (FuncMax - FuncMin);
	switch(FuncPtr)
		{
		case pHPOSITION:
		case pOSDVPOSITION:
			Percent = 100 - Percent;
		break;
		case pOSDTIMER:
			Percent = Value;
			temp = Percent * FuncMax;
			Percent = temp / (FuncMax - 0);
		break;
		case pVCMVOL:
		//case pLIGHTHSTART:
		//case pLIGHTHWIDTH:
		//case pLIGHTVSTART:
		//case pLIGHTVHIGHT:
			Percent = Value;
		break;
		
		};
	WriteWordIIC563(0x0cc,Attr);
	BCD(Percent,Ptr,1);
	WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Ptr+2,3);
}

#ifdef CUSTOM_OSD
#if 0
void ShowBarDigitalValue(Word Addr,Word Attr, Word value,Byte Ptr,Byte indicator,Byte Onselect)
{
	//Byte ret_val=0;
	Byte b0,b1,b2;
	Byte offset;

	//printf("Value= %d\n",(unsigned short)value);
	//printf("ptr= %d\n",(unsigned short)Ptr);
	//printf("indicator= %d\n",(unsigned short)indicator);
	//printf("Onselect= %d\n",(unsigned short)Onselect);


	if(Onselect)
		Attr = BAR_2bitIcon_OnSelect_Attr;
	else
		Attr = BAR_2bitIcon_DeSelect_Attr;

	offset = BAR_ValueOnly_WithBar;	
	
	
	// Åã¥Ü¦Ê¦ì =============================
	b0 = value/100;
	if(b0 > 0)
	{
		value = value - (b0*100);
		WriteWordIIC563(0x0cc,Attr); 	
		WriteIndexPort(Addr-1,tySRAM_CODE_CC_CD,offset+b0);
	}
	
	// Åã¥Ü¤Q¦ì =============================
	// ­pºâ¤Q¼Æ¦ì
	b1 = value/10;
	// ­pºâ¥Ø«e«ü¥Ü±øªº¦ì¸m
	if((Ptr+1)==BAR_CENTER)
	{
		offset = BAR_ValueAndIndecateStart;
		
		b1 = b1 + (indicator * 10);

		//printf("xx_offset= %d\n",(unsigned short)offset);
		//printf("xx_b1= %d\n",(unsigned short)b1);
	}

	if(Onselect)
	{
		if((Ptr+1) < BAR_CENTER)
			Attr = BAR_2bitIcon_DeSelect_Attr;
	}else
		Attr = BAR_2bitIcon_DeSelect_Attr;
	
	WriteWordIIC563(0x0cc,Attr); 	
	WriteIndexPort(Addr,tySRAM_CODE_CC_CD,offset+b1);
	
	// Åã¥Ü­Ó¦ì =============================
	offset = BAR_ValueOnly_WithBar;
	b2 =	value %10;
	if((Ptr+1)==BAR_CENTER+1)
	{			
		offset = BAR_ValueAndIndecateStart;
		
		b2 = b2 + (indicator * 10);

		//printf("xx_offset= %d\n",(unsigned short)offset);
		//printf("xx_b2= %d\n",(unsigned short)b2);
	}

	if(Onselect)
	{
		if((Ptr+1) < BAR_CENTER+1)
			Attr = BAR_2bitIcon_DeSelect_Attr;
	}else
		Attr = BAR_2bitIcon_DeSelect_Attr;
	WriteWordIIC563(0x0cc,Attr); 	
	WriteIndexPort(Addr+1,tySRAM_CODE_CC_CD,offset+b2);


	//printf("\n");
}
	
void ShowBarMaxAndMinValue(Word Addr,Word Min,Word Max)
{
	Word value;
	Byte b;
	Byte offset,i;

	//printf("Max= %d\n",(unsigned short)Max);
	//printf("Min= %d\n",(unsigned short)Min);

	offset = BAR_ValueOnlyStart;	
	WriteWordIIC563(0x0cc,BAR_2bitIcon_DeSelect_Attr);
	
	// show min value on bar left ============================
	value = Min;
	for(i=1;i<3;i++)
	{		
		if(i==1)
			b=value%10;

		if(i==2)
			b=value/10;
		
			 	
		WriteIndexPort(Addr-i,tySRAM_CODE_CC_CD,offset+b);
	}

	// show max value on bar right ============================
	value = Max;
	for(i=0;i<3;i++)
	{		
		if(i==0)
		{
			if(value >= 100)
			{
				b=value/100;
				value -= b*100;
			}
		}
		
		if(i==1)
			b=value/10;

		if(i==2)
			b=value%10;

		WriteIndexPort(Addr+ BAR_LENGTH + 2 + i,tySRAM_CODE_CC_CD,offset+b);

	}
	
}


void ShowBar(Word Addr,Word Attr,Word Min,Word Max,Word Value)
{
	Byte i,Ptr,onselect;
	Word Percent,temp;
	unsigned long Wtemp;
	Byte indicator_pos=0;

	WriteWordIIC563(0x0cc,Attr);
	
	temp = (Value - Min);
	
	Wtemp = temp * BAR_LENGTH * BAR_SEGMENT;
	
 	Percent = Wtemp / (Max-Min);
	
	if(Percent > (BAR_LENGTH * BAR_SEGMENT))
		Percent = BAR_LENGTH * BAR_SEGMENT;

	//============================================
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(tySRAM_CODE_CC_CD);
	IIC_Tx(Addr&0x00ff);	
	IIC_Tx(Addr>>8);

	////bar '|'	=================================================
	IIC_Tx(BAR_Left);									
	//char_count = 1;
	
	//============================================
	Ptr = Percent / BAR_SEGMENT;
	for(i=0;i<Ptr;i++)
	{
		temp = IIC_Tx(BAR_Mid_Fill);										//bar full icon Data		
	}
		
	//============================================	
	Ptr = Percent % BAR_SEGMENT;
	
	if(Percent<(BAR_LENGTH * BAR_SEGMENT) && Percent>0)
	{
		IIC_Tx(BAR_Indecate_1+Ptr);										//bar mid icon Data
		i++;
	}


	//============================================
	for(;i<BAR_LENGTH;i++)
	{

		IIC_Tx(BAR_Mid_Empty);										//bar empty icon Data		
	}	
	//============================================		
	IIC_Tx(BAR_Right);													//bar ']' icon Data

	//============================================
	IIC_Stop();

	//============================================
	temp = BAR_2bitIcon_OnSelect_Attr;
	if(Attr == temp)
	{
		onselect=1;
	}else{
		onselect =0;
	}
	
	ShowBarDigitalValue(Addr+BAR_CENTER,Attr
							,Value,
							Percent / BAR_SEGMENT,Percent % BAR_SEGMENT, 
							onselect);
	

	ShowBarMaxAndMinValue(Addr,Min,Max);

	
}
#else

void ShowBarValueAndMin(Word Addr,Word Min,Word Value)
{
	Word tmp,Percent;
	Byte b;
	Byte offset,i;

	GetFuncRange(FuncPtr);

	switch(FuncPtr)
	{
		case pOSDTIMER:
			Min = FuncMin;
			Percent = Value;
			break;
		default:						
			Percent = Value - FuncMin;
			tmp = Percent * 100;	
			Percent = tmp / (FuncMax - FuncMin);
			Min=0;
			break;
	};		
	/*
	printf("FuncPtr= %d\n",(unsigned short)FuncPtr);
	printf("Max= %d\n",(unsigned short)FuncMax);
	printf("Min= %d\n",(unsigned short)FuncMin);
	printf("Value= %d\n",(unsigned short)Value);
	printf("Percent= %d\n",(unsigned short)Percent);
	printf("\n");
	*/

	
	offset = BAR_ValueOnlyStart;	
	WriteWordIIC563(0x0cc,BAR_2bitIcon_DeSelect_Attr);
#if 0	
	// show min value on bar left ============================
	tmp = Min;
	for(i=1;i<3;i++)
	{		
		if(i==1)
			b=tmp%10;

		if(i==2)
			b=tmp/10;
		
			 	
		WriteIndexPort(Addr-i,tySRAM_CODE_CC_CD,offset+b);
	}
#endif
	// show value on bar right ============================
	tmp = Percent;
	for(i=0;i<3;i++)
	{		
		b=0;
		
		if(i==0)
		{
			if(tmp >= 100)
			{
				b=tmp/100;
				tmp -= b*100;
			}
		}
		
		if(i==1)
			b=tmp/10;

		if(i==2)
			b=tmp%10;

		WriteIndexPort(Addr+ BAR_LENGTH + 2 + i,tySRAM_CODE_CC_CD,offset+b);

	}
	
}


void ShowBar(Word Addr,Word Attr,Word Min,Word Max,Word Value)
{
	Byte i,Ptr;
	Word Percent,temp;
	unsigned long Wtemp;
	Byte indicator_pos=0;


	WriteWordIIC563(0x0cc,Attr);
	
	temp = (Value - Min);
	
	Wtemp = temp * BAR_LENGTH * BAR_SEGMENT;
	
 	Percent = Wtemp / (Max-Min);
	
	if(Percent > (BAR_LENGTH * BAR_SEGMENT))
		Percent = BAR_LENGTH * BAR_SEGMENT;

	//============================================
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(tySRAM_CODE_CC_CD);
	IIC_Tx(Addr&0x00ff);	
	IIC_Tx(Addr>>8);

	////bar '|'	=================================================
	IIC_Tx(BAR_Left);									
	//char_count = 1;
	
	//============================================
	Ptr = Percent / BAR_SEGMENT;
	for(i=0;i<Ptr;i++)
	{
		temp = IIC_Tx(BAR_Mid_Fill);										//bar full icon Data		
	}
		
	//============================================	
	Ptr = Percent % BAR_SEGMENT;
	
	if(Percent<(BAR_LENGTH * BAR_SEGMENT) && Percent>0)
	{
		IIC_Tx(BAR_Indecate_1+Ptr);										//bar mid icon Data
		i++;
	}


	//============================================
	for(;i<BAR_LENGTH;i++)
	{

		IIC_Tx(BAR_Mid_Empty);										//bar empty icon Data		
	}	
	//============================================		
	IIC_Tx(BAR_Right);													//bar ']' icon Data

	//============================================
	IIC_Stop();

	//============================================
	ShowBarValueAndMin(Addr,Min,Value);

}

void ShowOnSelectFrame(unsigned char onSelectRow,unsigned char en)
{

	Byte *Offset;
	//Byte win_begin= ((MAINMENU_BAR_ADDR) % OSD_WIDTH)-3;
	Byte win_begin= 1;
	Byte win_height=3;
	Word firstLine = ((MAINMENU_DESC_ADDR) / OSD_WIDTH);
	//Word win_width= BAR_LENGTH + 8;
	Word win_width;// ((OSD_WIDTH) - (win_begin *2));	
	Byte win_color= OnSelectWin_Color;
	Byte bevel_color= OnSelectWin_Bevel_Color;
	Byte bevel_width = OnSelectWin_Bevel_width;
	Byte x1,x2,y1,y2;

	
	Offset = DescTab[FuncPtr][FuncBuf[pLANGUAGE]];
	win_width = *Offset+2;

	//printf("%d-len=%d \n",(Word)FuncPtr,(Word)win_width);

	if(en)
	{
		// open win-3 ,width,height, background color
		x1= win_begin;
		//x2= win_begin + win_width;
		x2= win_width;
		y1= (firstLine-1)+ (onSelectRow * 2);
		y2= y1 + (win_height-1);
		OpenWindow(WINDOW3, x1, x2,y1,y2,atWIN_SHADOW_2L,win_color);
		Osd_On(enWIN3);

		// bevel enable	
		WriteIIC563(0x0aa,ReadIIC563(0xaa)|0x80);
		
		// set bevel width
		WriteIIC563(0x0ad,bevel_width);

		// set bevel color
		WriteIIC563(0x0ae,bevel_color);
		WriteIIC563(0x0af,bevel_color);
	}else{
		// disable window-3
		WriteIIC563(0x080,ReadIIC563(0x080) & (~enWIN3));
	}
		/*
		printf("firstLine=%d \n",(unsigned short)firstLine);
		printf("win_width=%d \n",(unsigned short)win_width);
		printf("x1=%d \n",(unsigned short)x1);
		printf("x2=%d \n",(unsigned short)x2);
		printf("y1=%d \n",(unsigned short)y1);
		printf("y2=%d \n",(unsigned short)y2);	
		printf("\n");
		*/
}
/*
void OSDAPI_RotateFont(Byte *des, Byte *src)
{
#define B0(a) (a&0x01)
#define B1(a) ((a&0x02)>>1)
#define B2(a) ((a&0x04)>>2)
#define B3(a) ((a&0x08)>>3)
#define B4(a) ((a&0x10)>>4)
#define B5(a) ((a&0x20)>>5)
#define B6(a) ((a&0x40)>>6)
#define B7(a) ((a&0x80)>>7)

    des[26] = (B4(src[25])<<7) | (B0(src[24])<<6) | (B4(src[22])<<5) | (B0(src[21])<<4) | 
              (B4(src[19])<<3) | (B0(src[18])<<2) | (B4(src[16])<<1) | (B0(src[15])<<0);
    des[25] = (B4(src[13])<<7) | (B0(src[12])<<6) | (B4(src[10])<<5) | (B0(src[ 9])<<4) | 
              (B4(src[ 7])<<3) | (B0(src[ 6])<<2) | (B4(src[ 4])<<1) | (B0(src[ 3])<<0);
    des[24] = (B4(src[ 1])<<7) | (B0(src[ 0])<<6) | (B5(src[25])<<5) | (B1(src[24])<<4) | 
              (B5(src[22])<<3) | (B1(src[21])<<2) | (B5(src[19])<<1) | (B1(src[18])<<0);
    des[23] = (B5(src[16])<<7) | (B1(src[15])<<6) | (B5(src[13])<<5) | (B1(src[12])<<4) | 
              (B5(src[10])<<3) | (B1(src[ 9])<<2) | (B5(src[ 7])<<1) | (B1(src[ 6])<<0);
    des[22] = (B5(src[ 4])<<7) | (B1(src[ 3])<<6) | (B5(src[ 1])<<5) | (B1(src[ 0])<<4) | 
              (B6(src[25])<<3) | (B2(src[24])<<2) | (B6(src[22])<<1) | (B2(src[21])<<0);
    des[21] = (B6(src[19])<<7) | (B2(src[18])<<6) | (B6(src[16])<<5) | (B2(src[15])<<4) | 
              (B6(src[13])<<3) | (B2(src[12])<<2) | (B6(src[10])<<1) | (B2(src[ 9])<<0);
    des[20] = (B6(src[ 7])<<7) | (B2(src[ 6])<<6) | (B6(src[ 4])<<5) | (B2(src[ 3])<<4) | 
              (B6(src[ 1])<<3) | (B2(src[ 0])<<2) | (B7(src[25])<<1) | (B3(src[24])<<0);
    des[19] = (B7(src[22])<<7) | (B3(src[21])<<6) | (B7(src[19])<<5) | (B3(src[18])<<4) | 
              (B7(src[16])<<3) | (B3(src[15])<<2) | (B7(src[13])<<1) | (B3(src[12])<<0);
    des[18] = (B7(src[10])<<7) | (B3(src[ 9])<<6) | (B7(src[ 7])<<5) | (B3(src[ 6])<<4) | 
              (B7(src[ 4])<<3) | (B3(src[ 3])<<2) | (B7(src[ 1])<<1) | (B3(src[ 0])<<0);
    des[17] = (B0(src[26])<<7) | (B4(src[24])<<6) | (B0(src[23])<<5) | (B4(src[21])<<4) | 
              (B0(src[20])<<3) | (B4(src[18])<<2) | (B0(src[17])<<1) | (B4(src[15])<<0);  
    des[16] = (B0(src[14])<<7) | (B4(src[12])<<6) | (B0(src[11])<<5) | (B4(src[ 9])<<4) | 
              (B0(src[ 8])<<3) | (B4(src[ 6])<<2) | (B0(src[ 5])<<1) | (B4(src[ 3])<<0);   
    des[15] = (B0(src[ 2])<<7) | (B4(src[ 0])<<6) | (B1(src[26])<<5) | (B5(src[24])<<4) | 
              (B1(src[23])<<3) | (B5(src[21])<<2) | (B1(src[20])<<1) | (B5(src[18])<<0);
    des[14] = (B1(src[17])<<7) | (B5(src[15])<<6) | (B1(src[14])<<5) | (B5(src[12])<<4) | 
              (B1(src[11])<<3) | (B5(src[ 9])<<2) | (B1(src[ 8])<<1) | (B5(src[ 6])<<0);
    des[13] = (B1(src[ 5])<<7) | (B5(src[ 3])<<6) | (B1(src[ 2])<<5) | (B5(src[ 0])<<4) | 
              (B2(src[26])<<3) | (B6(src[24])<<2) | (B2(src[23])<<1) | (B6(src[21])<<0);
    des[12] = (B2(src[20])<<7) | (B6(src[18])<<6) | (B2(src[17])<<5) | (B6(src[15])<<4) | 
              (B2(src[14])<<3) | (B6(src[12])<<2) | (B2(src[11])<<1) | (B6(src[ 9])<<0);
    des[11] = (B2(src[ 8])<<7) | (B6(src[ 6])<<6) | (B2(src[ 5])<<5) | (B6(src[ 3])<<4) | 
              (B2(src[ 2])<<3) | (B6(src[ 0])<<2) | (B3(src[26])<<1) | (B7(src[24])<<0);
    des[10] = (B3(src[23])<<7) | (B7(src[21])<<6) | (B3(src[20])<<5) | (B7(src[18])<<4) | 
              (B3(src[17])<<3) | (B7(src[15])<<2) | (B3(src[14])<<1) | (B7(src[12])<<0);
    des[ 9] = (B3(src[11])<<7) | (B7(src[ 9])<<6) | (B3(src[ 8])<<5) | (B7(src[ 6])<<4) | 
              (B3(src[ 5])<<3) | (B7(src[ 3])<<2) | (B3(src[ 2])<<1) | (B7(src[ 0])<<0);    
    des[ 8] = (B4(src[26])<<7) | (B0(src[25])<<6) | (B4(src[23])<<5) | (B0(src[22])<<4) | 
              (B4(src[20])<<3) | (B0(src[19])<<2) | (B4(src[17])<<1) | (B0(src[16])<<0);
    des[ 7] = (B4(src[14])<<7) | (B0(src[13])<<6) | (B4(src[11])<<5) | (B0(src[10])<<4) | 
              (B4(src[ 8])<<3) | (B0(src[ 7])<<2) | (B4(src[ 5])<<1) | (B0(src[ 4])<<0);
    des[ 6] = (B4(src[ 2])<<7) | (B0(src[ 1])<<6) | (B5(src[26])<<5) | (B1(src[25])<<4) | 
              (B5(src[23])<<3) | (B1(src[22])<<2) | (B5(src[20])<<1) | (B1(src[19])<<0);
    des[ 5] = (B5(src[17])<<7) | (B1(src[16])<<6) | (B5(src[14])<<5) | (B1(src[13])<<4) | 
              (B5(src[11])<<3) | (B1(src[10])<<2) | (B5(src[ 8])<<1) | (B1(src[ 7])<<0);
    des[ 4] = (B5(src[ 5])<<7) | (B1(src[ 4])<<6) | (B5(src[ 2])<<5) | (B1(src[ 1])<<4) | 
              (B6(src[26])<<3) | (B2(src[25])<<2) | (B6(src[23])<<1) | (B2(src[22])<<0);
    des[ 3] = (B6(src[20])<<7) | (B2(src[19])<<6) | (B6(src[17])<<5) | (B2(src[16])<<4) | 
              (B6(src[14])<<3) | (B2(src[13])<<2) | (B6(src[11])<<1) | (B2(src[10])<<0);
    des[ 2] = (B6(src[ 8])<<7) | (B2(src[ 7])<<6) | (B6(src[ 5])<<5) | (B2(src[ 4])<<4) | 
              (B6(src[ 2])<<3) | (B2(src[ 1])<<2) | (B7(src[26])<<1) | (B3(src[25])<<0);
    des[ 1] = (B7(src[23])<<7) | (B3(src[22])<<6) | (B7(src[20])<<5) | (B3(src[19])<<4) | 
              (B7(src[17])<<3) | (B3(src[16])<<2) | (B7(src[14])<<1) | (B3(src[13])<<0);
    des[ 0] = (B7(src[11])<<7) | (B3(src[10])<<6) | (B7(src[ 8])<<5) | (B3(src[ 7])<<4) | 
              (B7(src[ 5])<<3) | (B3(src[ 4])<<2) | (B7(src[ 2])<<1) | (B3(src[ 1])<<0);

#undef B0
#undef B1
#undef B2
#undef B3
#undef B4
#undef B5
#undef B6
#undef B7
}

void OSDAPI_RotateIdentFont(Byte *des, Word *src)
{
	Byte i;
	xdata Byte buffer[27];

    	for (i = 0; i < 9; i++) {
        		buffer[3*i]   = (Byte)(src[2*i]>>4);
        		buffer[3*i+1] = (Byte)((src[2*i+1]&0x00F0) | (src[2*i]>>12));
        		buffer[3*i+2] = (Byte)(src[2*i+1]>>8);

    	}

    	OSDAPI_RotateFont(des, buffer);
}

static xdata Word usIndentFontBuffer[18];
xdata Byte volatile SC_INDEX_DATA;

void OSDAPI_LoadIndentFont(void)
{
	Byte i;
        	xdata Byte  buffer[27];

	OSDAPI_RotateIdentFont(buffer,usIndentFontBuffer);

	for (i=0; i<27; i++) {
            SC_INDEX_DATA = buffer[i];
        	}        
    
}
*/
/*
void ShowBarMaxAndMinValue(Word Addr,Word Min,Word value)
{
	Word Attr;
	Byte Ptr[5];
	Byte number;


	Attr = fWHITE;
	// show min value on bar left ============================
	WriteWordIIC563(0x0cc,Attr);

	if(Min< 10)
		number = 1;
	else
		number = 2;
	BCD(Min,Ptr,number);
	WriteSeqIndexPort(Addr-number,tySRAM_CODE_CC_CD,Ptr+(number+1),number);


	// show value on bar right ============================
	if(value < 100)
		number = 2;
	else
		number = 3;

	BCD(value,Ptr,number);
	WriteSeqIndexPort(Addr+ BAR_LENGTH + 2,tySRAM_CODE_CC_CD,Ptr+(number+1),number);


	
}

Byte tp;

void ShowBar(Word Addr,Word Attr,Word Min,Word Max,Word Value)
{
	Byte i,Ptr;
	Word Percent,temp;
	unsigned long Wtemp;

	if(MenuPtr.Op == oINCVAL)
		tp++;
	else
		tp--;

	if(tp> 100)
		tp = 100;

	Value = tp;
	Max = 100;
	Min =0;
	
	#if PRINT_MESSAGE
		printf("Bar Value= %d\n",(unsigned short)Value);
	#endif

	if(MenuPtr.Level == Level_SubItem)
		Attr = tySRAM_1BIT | fYELLOW;
	else
		Attr = tySRAM_1BIT | fWHITE;

	WriteWordIIC563(0x0cc,Attr);
	temp = (Value - Min);
	Wtemp = temp * BAR_LENGTH * BAR_SEGMENT;
 	Percent = Wtemp / (Max-Min);
	if(Percent > (BAR_LENGTH * BAR_SEGMENT))
		Percent = BAR_LENGTH * BAR_SEGMENT;
	switch(FuncPtr)
		{
		case pHPOSITION:
			Percent = (BAR_LENGTH*BAR_SEGMENT) - Percent;
			break;
		
		};
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(tySRAM_CODE_CC_CD);
	IIC_Tx(Addr&0x00ff);	
	IIC_Tx(Addr>>8);
	IIC_Tx(0x19);									//bar '['	Data
	Ptr = Percent / BAR_SEGMENT;
	for(i=0;i<Ptr;i++)
		temp = IIC_Tx(0x1e);						//bar full icon Data
	Ptr = Percent % BAR_SEGMENT;
	//if(Ptr == 6)
	//	Ptr --;
	if(Percent<(BAR_LENGTH * BAR_SEGMENT) && Percent>0)
		{
		IIC_Tx(0x1a+Ptr);							//bar mid icon Data
		i++;
		}
	for(;i<BAR_LENGTH;i++)
		IIC_Tx(0x1a);								//bar empty icon Data
	IIC_Tx(0x1f);									//bar ']' icon Data
	IIC_Stop();	

	
	ShowBarMaxAndMinValue(Addr,Min,Value);
}
*/
#endif

void showLogo()
{
	Word Attr,i;

	for(i =0;i <3*10 ;i++)
	{		
		Attr = ((palette_index[i] * 16)+OFFSET_4BIT)<<8;
		WriteWordIIC563(0x0cc,Attr|caFOUR_BIT);		
		WriteIndexPort((ROW1*(i/10))+(i % 10)+(OSD_WIDTH - LogoWidth - 2),tySRAM_CODE_CC_CD,font_index[i]);
	}
}

void showTitle(unsigned char title_id)
{
	
#if 1
	Byte *Offset,Length;
	Word start_add;

	Offset = TitalTab[title_id][FuncBuf[pLANGUAGE]];
	Length = *Offset;
	Offset++;
	start_add = (OSD_WIDTH - Length)/2;
	
	//printf("%d-len=%d \n",(Word)title_id,(Word)start_add);
	
	WriteWordIIC563(0x0cc,DESC_COLOR);
	WriteSeqIndexPort(ROW2+start_add,tySRAM_CODE_CC_CD,Offset,Length);	

#if 0
	WriteWordIIC563(0x0cc,DESC_COLOR);

	switch(title_id)
	{
		case OSD_Title_MainMenu:
			WriteSeqIndexPort(ROW2+6,tySRAM_CODE_CC_CD,"Main menu",9);
			break;
		case  = :
			WriteSeqIndexPort(ROW2+6,tySRAM_CODE_CC_CD,"Image setup",11);
			break;
			
		case OSD_Title_SourceSelect:
			WriteSeqIndexPort(ROW2+6,tySRAM_CODE_CC_CD,"Source select",13);
			break;			
			
		case OSD_Title_OSDSetup:
			WriteSeqIndexPort(ROW2+6,tySRAM_CODE_CC_CD,"OSD setup",9);
			break;
	};
#endif	

	
	
#else
	unsigned char *title_font_index;	
	Word Attr,i;

	switch(title_id)
	{
		case OSD_Title_MainMenu:
			title_font_index = OSDTitle_MainMenu_font_index;
			break;
		case OSD_Title_ImageSetup:
			title_font_index = OSDTitle_ImageSetup_font_index;
			break;
			
		case OSD_Title_SourceSelect:
			title_font_index = OSDTitle_SourceSelect_font_index;
			break;			
			
		case OSD_Title_OSDSetup:
			title_font_index = OSDTitle_OSDSetup_font_index;
			break;
	};
	
	for(i =0;i <2*12 ;i++)
	{		
		WriteWordIIC563(0x0cc,caONE_BIT_512);		
		WriteIndexPort((ROW1*(i/12))+(i % 12)+2,tySRAM_CODE_CC_CD,OSDTitle_MainMenu_font_index[i]);
	}
#endif
}


void showRotate()
{


	if(PageOSDSetup[MenuPtr.Ptr] == pOSDRotate && MenuPtr.Level == Level_SubItem)
		WriteWordIIC563(0x0cc,VALUE_COLOR);
	else
		WriteWordIIC563(0x0cc,DESC_COLOR);

	switch(FuncBuf[pOSDRotate])
	{
		default:
		case 0:
			WriteIndexPort(ROW14+21,tySRAM_CODE_CC_CD,_Space);
			WriteIndexPort(ROW14+22,tySRAM_CODE_CC_CD,_Space);
			WriteIndexPort(ROW14+23,tySRAM_CODE_CC_CD,'0');
			WriteIndexPort(ROW14+24,tySRAM_CODE_CC_CD,_Degree);
			break;
		case 1:
			WriteIndexPort(ROW14+21,tySRAM_CODE_CC_CD,_Space);
			WriteIndexPort(ROW14+22,tySRAM_CODE_CC_CD,'9');
			WriteIndexPort(ROW14+23,tySRAM_CODE_CC_CD,'0');
			WriteIndexPort(ROW14+24,tySRAM_CODE_CC_CD,_Degree);
			break;
		case 2:
			WriteIndexPort(ROW14+21,tySRAM_CODE_CC_CD,'1');
			WriteIndexPort(ROW14+22,tySRAM_CODE_CC_CD,'8');
			WriteIndexPort(ROW14+23,tySRAM_CODE_CC_CD,'0');
			WriteIndexPort(ROW14+24,tySRAM_CODE_CC_CD,_Degree);
			break;
		case 3:
			WriteIndexPort(ROW14+21,tySRAM_CODE_CC_CD,'2');
			WriteIndexPort(ROW14+22,tySRAM_CODE_CC_CD,'7');
			WriteIndexPort(ROW14+23,tySRAM_CODE_CC_CD,'0');
			WriteIndexPort(ROW14+24,tySRAM_CODE_CC_CD,_Degree);
			break;
	}
}

#else
void ShowBar(Word Addr,Word Attr,Word Min,Word Max,Word Value)
{
	Byte i,Ptr;
	Word Percent,temp;
	unsigned long Wtemp;
	
	#if PRINT_MESSAGE
		printf("Bar Value= %d\n",(unsigned short)Value);
	#endif

	WriteWordIIC563(0x0cc,Attr);
	temp = (Value - Min);
	Wtemp = temp * BAR_LENGTH * BAR_SEGMENT;
 	Percent = Wtemp / (Max-Min);
	if(Percent > (BAR_LENGTH * BAR_SEGMENT))
		Percent = BAR_LENGTH * BAR_SEGMENT;
	//reverse
	switch(FuncPtr)
		{
		case pHPOSITION:
// Jacky 0201 removed
//		case pOSDVPOSITION:
			Percent = (BAR_LENGTH*BAR_SEGMENT) - Percent;
			break;
		
		};
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(tySRAM_CODE_CC_CD);
	IIC_Tx(Addr&0x00ff);	
	IIC_Tx(Addr>>8);
	IIC_Tx(0x19);									//bar '['	Data
	Ptr = Percent / BAR_SEGMENT;
	for(i=0;i<Ptr;i++)
		temp = IIC_Tx(0x1e);						//bar full icon Data
	Ptr = Percent % BAR_SEGMENT;
	//if(Ptr == 6)
	//	Ptr --;
	if(Percent<(BAR_LENGTH * BAR_SEGMENT) && Percent>0)
		{
		IIC_Tx(0x1a+Ptr);							//bar mid icon Data
		i++;
		}
	for(;i<BAR_LENGTH;i++)
		IIC_Tx(0x1a);								//bar empty icon Data
	IIC_Tx(0x1f);									//bar ']' icon Data
	IIC_Stop();	
}
#endif
/*
void ShowCBar(Word Addr,Word Attr,Word Min,Word Max,Word Value)
{
	Byte i,Ptr;
	Word Percent,temp;
	unsigned long Wtemp;
	WriteWordIIC563(0x0cc,Attr);
	temp = (Value - Min);
	Wtemp = temp * CBAR_LENGTH * CBAR_SEGMENT;
 	Percent = Wtemp / (Max-Min);
	if(Percent > (CBAR_LENGTH * CBAR_SEGMENT))
		Percent = CBAR_LENGTH * CBAR_SEGMENT;
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(0xe0);
	IIC_Tx(tySRAM_CODE_CC_CD);
	IIC_Tx(Addr&0x00ff);	
	IIC_Tx(Addr>>8);
	IIC_Tx(0x19);		//bar '['	Data
	Ptr = Percent / CBAR_SEGMENT;
	for(i=0;i<Ptr;i++)
		temp = IIC_Tx(0x1e);	//bar full icon Data
	Ptr = Percent % CBAR_SEGMENT;
	//if(Ptr == 6)
	//	Ptr --;
	if(Percent<(CBAR_LENGTH * CBAR_SEGMENT) && Percent>0)
		{
		IIC_Tx(0x1a+Ptr);	//bar mid icon Data
		i++;
		}
	for(;i<CBAR_LENGTH;i++)
		IIC_Tx(0x1a);	//bar empty icon Data
	IIC_Tx(0x1f);		//bar ']' icon Data
	IIC_Stop();	
}
*/

void ShowResolution(Word Addr,Word Attr)
{
	
	Byte code SourceTab1[][7]={"  VGA  "," YPbPr "," DVI-D "," CVBS1 "," AV2   ","S-Video"};
	Byte code TvTab[][9]={"PAL 4.43 ","NTSC 4.43"," NTSC N  ","  SECAM  "," UNKNOW  "," SECAM   ","  MONO   ",
						  " NTSC M  ","PAL 4.43 ","NTSC 4.43","  PAL M  "," NTSC J  "," UNKNOW  ","  MONO   "};
	Byte VaSeq[5];
	
		WriteWordIIC563(0x0cc,Attr);

	if(FuncBuf[pVIDEOSOURCE] <= DVIinput)
	{
		if(FuncBuf[pVIDEOSOURCE] == DVIinput)
		{
			H_SYNC=V_SYNC=Hresolution=Vresolution=HV_Pol=0;
		}
		//Hsync, Vsync, Hres, Vres , pol
		BCD(H_SYNC/10,VaSeq,2);
		WriteSeqIndexPort(Addr+2,tySRAM_CODE_CC_CD,VaSeq+3,2);
		WriteIndexPort(Addr+4,tySRAM_CODE_CC_CD,'k');
		WriteIndexPort(Addr+5,tySRAM_CODE_CC_CD,'H');
		WriteIndexPort(Addr+6,tySRAM_CODE_CC_CD,'z');
		WriteIndexPort(Addr+7,tySRAM_CODE_CC_CD,_DOT);

		BCD(V_SYNC/10,VaSeq,2);
		WriteSeqIndexPort(Addr+8,tySRAM_CODE_CC_CD,VaSeq+3,2);
		WriteIndexPort(Addr+10,tySRAM_CODE_CC_CD,'H');
		WriteIndexPort(Addr+11,tySRAM_CODE_CC_CD,'z');
		WriteIndexPort(Addr+12,tySRAM_CODE_CC_CD,_DOT);

		BCD(Hresolution,VaSeq,2);
		WriteSeqIndexPort(Addr+13,tySRAM_CODE_CC_CD,VaSeq+1,4);
		WriteIndexPort(Addr+18,tySRAM_CODE_CC_CD,'x');

		BCD(Vresolution,VaSeq,2);
		WriteSeqIndexPort(Addr+20,tySRAM_CODE_CC_CD,VaSeq+1,4);
		WriteIndexPort(Addr+24,tySRAM_CODE_CC_CD,_DOT);

		// show Hsync pol
		if(HV_Pol & 0x10)
			WriteSeqIndexPort(Addr+25,tySRAM_CODE_CC_CD,"P",1);
		else
			WriteSeqIndexPort(Addr+25,tySRAM_CODE_CC_CD,"N",1);

		// show Vsync pol
		if(HV_Pol & 0x20)
			WriteSeqIndexPort(Addr+26,tySRAM_CODE_CC_CD,"P",1);
		else
			WriteSeqIndexPort(Addr+26,tySRAM_CODE_CC_CD,"N",1);
		
	}else{

		
		WriteSeqIndexPort(Addr+2,tySRAM_CODE_CC_CD,SourceTab1[FuncBuf[pVIDEOSOURCE]],7);
		WriteIndexPort(Addr+9,tySRAM_CODE_CC_CD,_DOT);
		WriteSeqIndexPort(Addr+10,tySRAM_CODE_CC_CD,TvTab[TvMode],9);

	}
	
		

		/*
		printf("Hsync=%d \n",(unsigned short)H_SYNC);
		printf("Vsync=%d \n",(unsigned short)V_SYNC);
		printf("Hresolution=%d \n",(unsigned short)Hresolution);
		printf("Vresolution=%d \n",(unsigned short)Vresolution);
		printf("HV_Pol=%d \n",(unsigned short)HV_Pol);
		*/
}

/*
void ShowMode(Word Addr,Word Attr)
{
	Byte VaSeq[5];
	WriteWordIIC563(0x0cc,Attr);
	BCD(ModePoint,VaSeq,2);
	if(ModePoint < UserModeSt)
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,"Preset Mode:",12);
	else
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,"  User Mode:",12);
	WriteSeqIndexPort(Addr+12,tySRAM_CODE_CC_CD,VaSeq+3,2);
}
*/
void ShowMessage(Byte No)
{

	Byte code FuncCode[]={pOUTOFRANGE,pNOSIGNAL,pNOSIGNAL,pAUTOADJUST,pAUTOCOLOR ,pEEPROMINIT};
	if(No == MSG_OUTRANGE)
		ShowOutOfRange();
	else{
		ClearOSD();
		WriteWordIIC563(0x081,(PanelWidth-24*12)/2);	// osd H start
		WriteWordIIC563(0x084,(PanelHeight-5*18)/2);	// osd v start
		OpenWindow(WINDOW4,0,24,0,4,atWIN_SHADOW_4L,bLIGHT>>4);
		WriteWordIIC563(0x0cc,fMAGENTA|caONE_BIT_512);		
		ShowDesc(FuncCode[No],ROW2+5,fWHITE|bBLACK);
	}
	
	//if(No == MSG_OUTRANGE)
	//	ShowStatus(ROW5+2,fWHITE|bBLACK);

	if(No == MSG_NOSYNC)
	{
		switch(FuncBuf[pVIDEOSOURCE])
		{
			case VGAinput:
				WriteSeqIndexPort(ROW2,tySRAM_CODE_CC_CD,"  VGA :",7);
				break;
			case DVIinput:
				WriteSeqIndexPort(ROW2,tySRAM_CODE_CC_CD," DVI-D:",7);
				break;

			case CVBSinput:
				WriteSeqIndexPort(ROW2,tySRAM_CODE_CC_CD," CVBS1:",7);
				break;

			case Svideoinput:
				WriteSeqIndexPort(ROW2+1,tySRAM_CODE_CC_CD,"SVideo:",7);
				break;
		}
		OsdTimer = 2000; // show 20s no sync, kevin 20090709
	}

	Osd_On(enWIN4);

}

void ShowOutOfRange()
{
	Byte i;
	Byte VaSeq[5];
	BackLightOn();
	
	ClearOSD();
	WriteWordIIC563(0x081,(PanelWidth-16*12)/2);
	WriteWordIIC563(0x084,(PanelHeight-6*18)/2);
	OpenWindow(WINDOW4,0,16,0,5,atWIN_SHADOW_4L,bRED>>4);
	WriteWordIIC563(0x0cc,fWHITE|caONE_BIT_512);
	
	/*for(i=0;i<3;i++){
		WriteIndexPort(ROW2+3+i,tySRAM_CODE_CC_CD,i);
		WriteIndexPort(ROW3+3+i,tySRAM_CODE_CC_CD,3+i);
		//WriteIndexPort(ROW4+3+i,tySRAM_CODE_CC_CD,51+i);
		//WriteIndexPort(ROW5+3+i,tySRAM_CODE_CC_CD,57+i);
	}*/

	ShowDesc(pOUTOFRANGE,ROW1+2,fWHITE|bBLACK);

	WriteWordIIC563(0x0cc,fWHITE|bBLACK);
	
	WriteSeqIndexPort(ROW3+4,tySRAM_CODE_CC_CD,"H:",2);
	BCD(H_SYNC,VaSeq,2);
	WriteSeqIndexPort(ROW3+6,tySRAM_CODE_CC_CD,VaSeq+1,3);
	WriteIndexPort(ROW3+9,tySRAM_CODE_CC_CD,'.');
	WriteIndexPort(ROW3+10,tySRAM_CODE_CC_CD,VaSeq[4]);
	WriteSeqIndexPort(ROW3+11,tySRAM_CODE_CC_CD,"KHz",3);
	WriteSeqIndexPort(ROW4+4,tySRAM_CODE_CC_CD,"V:",2);
	BCD(V_SYNC,VaSeq,2);
	WriteSeqIndexPort(ROW4+6,tySRAM_CODE_CC_CD,VaSeq+1,3);
	WriteIndexPort(ROW4+9,tySRAM_CODE_CC_CD,'.');
	WriteIndexPort(ROW4+10,tySRAM_CODE_CC_CD,VaSeq[4]);
	WriteSeqIndexPort(ROW4+11,tySRAM_CODE_CC_CD,"Hz",2);
	/* kevin 09102009 WriteSeqIndexPort(ROW5+12,tySRAM_CODE_CC_CD,"Max:",4);
	if(FuncBuf[pVIDEOSOURCE] == YPbPrinput)
	{
	//#if version == F_version
	//	WriteSeqIndexPort(ROW5+16,tySRAM_CODE_CC_CD,"720P",4);
	//#else
		WriteSeqIndexPort(ROW5+16,tySRAM_CODE_CC_CD,"1080i",5);
	//#endif
	}else{
		BCD(PanelWidth,VaSeq,2);
		WriteSeqIndexPort(ROW5+16,tySRAM_CODE_CC_CD,VaSeq+1,4);
		WriteIndexPort(ROW5+20,tySRAM_CODE_CC_CD,'x');
		BCD(PanelHeight,VaSeq,2);
		WriteSeqIndexPort(ROW5+21,tySRAM_CODE_CC_CD,VaSeq+1,4);
	}*/
}
/*
void WritePage563(unsigned char addr1,unsigned char addr2,unsigned char *p)
{
unsigned char i,ch;
	IIC_Start();
	IIC_Tx(SCALER_ADDR);
	IIC_Tx(addr1);
	for(i=0; i<16; i++){
		ch = p[addr2 + i];
		IIC_Tx(ch);
	}
	IIC_Stop();
}

*/
/*
void ShowOSDDimmer()
{
	unsigned char DimStep;
	code unsigned char DimmerTab[][7]={"Dark   ","Normal ","Light  ","       "};
	#if PRINT_MESSAGE	
		printf("dimmer = %d\n",(unsigned short)FuncBuf[pBRIGHTNESS]);
	#endif
	switch(FuncBuf[pBRIGHTNESS])
		{
			case 30:
				DimStep = 0;
				break;
			case 50:
				DimStep = 1;
				break;
		
			case 70:
				DimStep = 2;
				break;

			default:
				DimStep = 3;
				break;
	}

	ClearOSD();
	//WriteWordIIC563(0x081,PanelWidth-8*12);
	WriteWordIIC563(0x081,ROW1+10);
	WriteWordIIC563(0x084,5);
	WriteWordIIC563(0x0cc,SOURCE_COLOR);
	WriteSeqIndexPort(ROW0,tySRAM_CODE_CC_CD,DimmerTab[DimStep],7);
	Osd_On(0);
	//OsdTimer = 10;
}
*/

void ShowSource()
{
#if video_decoder == tw9906
	code unsigned char SourceTab[][7]={"VGA    ","YPbPr  ","DVI-D  ","CVBS1  ","CCD    ","S-Video"};
#else
	code unsigned char SourceTab[][7]={"VGA    ","YPbPr  ","DVI-D  ","CVBS1  ","AV2    ","S-Video"};
#endif
	#if PRINT_MESSAGE	
		printf("video source = %d\n",(unsigned short)FuncBuf[pVIDEOSOURCE]);
	#endif

	ClearOSD();
	WriteWordIIC563(0x081,ROW1+10);
	WriteWordIIC563(0x084,5);
	WriteWordIIC563(0x0cc,SOURCE_COLOR);
	WriteSeqIndexPort(ROW0,tySRAM_CODE_CC_CD,SourceTab[FuncBuf[pVIDEOSOURCE]],7);
	Osd_On(0);
	OsdTimer = 200;

}
/*
void ShowVersion()
{
	Byte VaSeq[5];
	WriteWordIIC563(0x0cc,VERSION_COLOR);
	BCD(Year,VaSeq,4);
	WriteSeqIndexPort(VERSION_ADDR-1,tySRAM_CODE_CC_CD,VaSeq+1,4);
	WriteIndexPort(VERSION_ADDR+3,tySRAM_CODE_CC_CD,'/');
	BCD(Month,VaSeq,2);
	WriteSeqIndexPort(VERSION_ADDR+4,tySRAM_CODE_CC_CD,VaSeq+3,2);
	WriteIndexPort(VERSION_ADDR+6,tySRAM_CODE_CC_CD,'/');
	BCD(Date,VaSeq,2);
	WriteSeqIndexPort(VERSION_ADDR+7,tySRAM_CODE_CC_CD,VaSeq+3,2);

	WriteSeqIndexPort(VERSION_ADDR+10,tySRAM_CODE_CC_CD,ModelName,10);
	WriteIndexPort(VERSION_ADDR+21,tySRAM_CODE_CC_CD,'V');
	BCD(Version,VaSeq,1);
	WriteIndexPort(VERSION_ADDR+22,tySRAM_CODE_CC_CD,VaSeq[4]);
	WriteIndexPort(VERSION_ADDR+23,tySRAM_CODE_CC_CD,'.');
	BCD(Reversion,VaSeq,2);
	WriteSeqIndexPort(VERSION_ADDR+24,tySRAM_CODE_CC_CD,VaSeq+3,2);
}
*/
/*
void ShowBurnInPattern(bit Init)
{
	unsigned char temp;
	return;//benson
	VideoMute = 1;
	if(Init)
		{
		Osd_Off();
		ForceToBackground(0,0,0xff);
		BurnInPattern = 0;
		BurnInTimer = 0;
		//FastMuteOff();
		BackLightOn();
		}
	if(BurnInPattern > 4)
		BurnInPattern = 0;
	BurnInTimer = BURN_IN_TIME;
	temp = ReadIIC563(0x159) & 0xA0;
	if(BurnInPattern == 0){
		WriteIIC563(0x154,0x09);
		//temp = temp | BurnInPattern;
	}
	else{
		WriteIIC563(0x154,0xb1);
		temp = temp | ((BurnInPattern-1) & 0x07);
	}
	WriteIIC563(0x159,temp);	//RGB
	BurnInPattern++;
}
*/
/*
void ShowOSDFrame(Byte Type)
{
	Byte code LeftLine[]={
		0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0e,0x0d,0x09
	};
	Byte code RightLine[]={
		0x02,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x07,0x0c,0x0b
	};
	Byte code LeftLine2[]={
		0x00,0x08,0x08,0x0e,0x0d,0x09
	};
	Byte code RightLine2[]={
		0x02,0x08,0x08,0x07,0x0c,0x0b
	};
	Byte code LeftLine3[]={
		0x00,0x08,0x08,0x08,0x09
	}; 
	Byte code RightLine3[]={
		0x02,0x08,0x08,0x08,0x0b
	};
	Byte code Line0[]={
		0x01
	};
//	Byte code Line3[]={
//		0x04
//	};
	Byte code Line12[]={
		0x06		// top white shadow
	};
	Byte code Line14[]={
		0x0a		// bottom black shadow
	};
	
	Byte i,End;
	Word Attr;
	Attr = OFFSET_2BIT<<8;
	WriteWordIIC563(0x0cc,Attr|caTWO_BIT);

	for(i=1; i<31; i++){
		WriteIndexPort(ROW0+i,tySRAM_CODE_CC_CD,Line0[0]);
		//if(Type == 0)
		//	WriteIndexPort(ROW3+i,tySRAM_CODE_CC_CD,Line3[0]);
		if(Type != 2){
			if(Type == 0)
				WriteIndexPort(ROW15+i,tySRAM_CODE_CC_CD,Line12[0]);
			else
				WriteIndexPort(ROW3+i,tySRAM_CODE_CC_CD,Line12[0]);
		}
	}

	if(Type == 2){
		WriteSeqIndexPort(9,tySRAM_2BIT,TwoBitFont2,3*54);
		WriteSeqIndexPort(0x100,tySRAM_1BIT,OneBitIcon+39*27,6*27);
		//Attr = (OFFSET_2BIT+4)<<8;
		//WriteWordIIC563(0x0cc,Attr|caTWO_BIT);
	}
	else{
		WriteSeqIndexPort(9,tySRAM_2BIT,TwoBitFont+9*54,3*54);
		WriteSeqIndexPort(0x100,tySRAM_1BIT,OneBitIcon,6*27);
	}
	
	for(i=1; i<31; i++){
		if(Type == 0)
			WriteIndexPort(ROW17+i,tySRAM_CODE_CC_CD,Line14[0]);
		else if(Type == 1)
			WriteIndexPort(ROW5+i,tySRAM_CODE_CC_CD,Line14[0]);
		else
			WriteIndexPort(ROW4+i,tySRAM_CODE_CC_CD,Line14[0]);
	}
	
	switch(Type)
		{
		case 0:
			End = 18;
			break;
		case 1:
			End = 6;
			break;
		case 2:
			End = 5;
			break;
		};
	for(i=0; i<End; i++){
		if(Type == 0){
			WriteIndexPort(OSD_WIDTH*i,tySRAM_CODE_CC_CD,LeftLine[i]);
			WriteIndexPort(OSD_WIDTH*i+(OSD_WIDTH-1),tySRAM_CODE_CC_CD,RightLine[i]);
		}
		else if(Type == 1){
			WriteIndexPort(OSD_WIDTH*i,tySRAM_CODE_CC_CD,LeftLine2[i]);
			WriteIndexPort(OSD_WIDTH*i+(OSD_WIDTH-1),tySRAM_CODE_CC_CD,RightLine2[i]);
		}
		else{
			WriteIndexPort(OSD_WIDTH*i,tySRAM_CODE_CC_CD,LeftLine3[i]);
			WriteIndexPort(OSD_WIDTH*i+(OSD_WIDTH-1),tySRAM_CODE_CC_CD,RightLine3[i]);
		}
	}

	WriteWordIIC563(0x0cc,Attr|0x0110);
	if(Type == 0)
		WriteSeqIndexPort(ROW18+1,tySRAM_CODE_CC_CD,SPACE,OSD_WIDTH-2);
	else if(Type == 1)
		WriteSeqIndexPort(ROW4+1,tySRAM_CODE_CC_CD,SPACE,OSD_WIDTH-2);

}
*/
/*
void showTest()
{

        	idata Byte  buffer[54];


	OSDAPI_RotateIdentFont(buffer,bar_TwoBitIcon);  
	WriteSeqIndexPort(0,tySRAM_2BIT,buffer,1*54);

		WriteWordIIC563(0x0cc,BAR_2bitIcon_DeSelect_Attr);	
		WriteIndexPort(0,tySRAM_CODE_CC_CD,0);

}
*/
void ShowSubMenu(Byte Page, Byte Ptr)
{
	Byte i,FuncPtrTemp;
	Word Attr;

	showLogo();
	
	showTitle(Page);

	if(Page == Menu_Main)
		ShowResolution(ROW20+((OSD_WIDTH - 26)/2),DESC_COLOR);
	
	//printf("MenuPtr.Level = %d\n",(unsigned short)MenuPtr.Level);
	//printf("Ptr = %d\n",(unsigned short)Ptr);
	//printf("Page = %d\n",(unsigned short)Page);

	ShowOnSelectFrame(0,FALSE);

	//showTest();

	FuncPtrTemp = FuncPtr;
	
	if(Page == OSD_Page_MainMenu){ 
		for(i=0; i<7; i++)
		{
			
			if(Ptr==i && MenuPtr.Level == Level_MainItem)
				Attr = SEL_FUN_COLOR;
			else
				Attr = FUN_COLOR;
			
			FuncPtr = PageMainMenu[i];
			ShowDesc(FuncPtr,MAINMENU_DESC_ADDR+(2*OSD_WIDTH*i),Attr);

			if(i == MenuPtr.Ptr)
				ShowOnSelectFrame(i,TRUE);
			
			switch(FuncPtr)
			{
				case pRECALL:
				case pAUTOADJUST:
					break;

				case pCONTRAST:
					GetFuncRange(FuncPtr);	
					
					//if(i == MenuPtr.Ptr)
					//	ShowOnSelectFrame(i,TRUE);

					if(Ptr==i && MenuPtr.Level == Level_SubItem)
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_OnSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);
					else
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_DeSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);							
					break;
				case pHotKey:
					showHotkeydefine();
					break;
			};
			
		}
	}else
	if(Page == OSD_Page_ImageSetup){
		
		for(i=0; i<9; i++)
		{
			
			if(Ptr==i && MenuPtr.Level == Level_MainItem )
				Attr = SEL_FUN_COLOR;
			else
				Attr = FUN_COLOR;
			
			FuncPtr = PageImageSetup[i];
			ShowDesc(FuncPtr,MAINMENU_DESC_ADDR+(2*OSD_WIDTH*i),Attr);
			
			if(i == MenuPtr.Ptr)
				ShowOnSelectFrame(i,TRUE);

			switch(FuncPtr)
			{
				case pSHARPNESS:
					ShowSharpness();
					break;
				case pCOLORTEMP:
					ShowColorMenu();
					break;
				default:
					GetFuncRange(FuncPtr);	

					//if(i == MenuPtr.Ptr)
					//	ShowOnSelectFrame(i,TRUE);

					if(Ptr==i && MenuPtr.Level == Level_SubItem)
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_OnSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);
					else
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_DeSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);							
					break;
			};
			
		}
		
	}else 
	if(Page == OSD_Page_SourceSelect){

		for(i=0; i<5; i++)
		{
			
			if(Ptr==i && MenuPtr.Level == Level_MainItem )
				Attr = SEL_FUN_COLOR;
			else
				Attr = FUN_COLOR;
			
			FuncPtr = PageSourceSelect[i];
			ShowDesc(FuncPtr,MAINMENU_DESC_ADDR+(2*OSD_WIDTH*i),Attr);
			
			if(i == MenuPtr.Ptr)
				ShowOnSelectFrame(i,TRUE);

			switch(FuncPtr)
			{
				case pSETCVBS1:
				case pSETCVBS2:
				case pSETSVIDEO:
				case pSETDSUB:
				case pSETDVID:
					ShowOnOff(MenuPtr.Ptr);
					break;
				case pDEFAULTCHANNEL:
					ShowSignalAndPlay();
					break;
			};
			
		}
		
	}else 
	if(Page == OSD_Page_OSDSetup){
		
		for(i=0; i<6; i++)
		{
			
			if(Ptr==i && MenuPtr.Level == Level_MainItem )
				Attr = SEL_FUN_COLOR;
			else
				Attr = FUN_COLOR;
			
			FuncPtr = PageOSDSetup[i];
			ShowDesc(FuncPtr,MAINMENU_DESC_ADDR+(2*OSD_WIDTH*i),Attr);
			
			if(i == MenuPtr.Ptr)
				ShowOnSelectFrame(i,TRUE);

			switch(FuncPtr)
			{
				case pLANGUAGE:
					ShowLanguage();
					break;
				case pOSDRotate:
					showRotate();
					break;
					
				default:
					GetFuncRange(FuncPtr);	

					//if(i == MenuPtr.Ptr)
					//	ShowOnSelectFrame(i,TRUE);

					if(Ptr==i && MenuPtr.Level == Level_SubItem)
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_OnSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);
					else
						ShowBar(MAINMENU_BAR_ADDR+(2*OSD_WIDTH*i),BAR_2bitIcon_DeSelect_Attr,FuncMin,FuncMax,FuncBuf[FuncPtr]);							
					break;
			};
			
		}
	}else
	if(Page == OSD_Page_ChannelSelect){ 
		
		PowerTimer = OsdTimer = POWER_TIME;
		
		for(i=0; i<5; i++)
		{
			
			if(Ptr==i && MenuPtr.Level == Level_MainItem)
				Attr = SEL_FUN_COLOR;
			else
				Attr = FUN_COLOR;
			
			FuncPtr = PageChannelSelect[i];
			ShowDesc(FuncPtr,MAINMENU_DESC_ADDR+(2*OSD_WIDTH*i),Attr);

			if(i == MenuPtr.Ptr)
				ShowOnSelectFrame(i,TRUE);

			switch(FuncPtr)
			{
				case pOPTION:					
					break;
				case pSETDSUB:
				case pSETDVID:
				case pSETSVIDEO:
				case pSETCVBS1:
					ShowOnOff(MenuPtr.Ptr);
					break;
			};
			
		}
	}	
	
	FuncPtr = FuncPtrTemp;
	
}
/*
void ShowStatus(Word Addr,Word Attr)
{
code unsigned char IPSourceIcon[][2]={
	//0x30,0x31,
	//0x32,0x33,
		12,13,
		14,15,
};
code unsigned char AudioStatusIcon[][2]={
	//0x34,0x35,
	//0x36,0x37,
		16,17,
		18,19,
};
code unsigned char ColorStatusIcon[][2]={
	//0x3c,0x3d,
	//0x3a,0x3b,
	//0x38,0x39,
	//0x40,0x41,
		24,25,
		22,23,
		20,21,
		28,29,
		28,29,
};

//unsigned char i;

	WriteWordIIC563(0x0cc,Attr|caONE_BIT_512);
	if(FuncBuf[pVIDEOSOURCE]==VGAinput)
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,IPSourceIcon[0],2);
	else if(FuncBuf[pVIDEOSOURCE]==DVIinput)
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,IPSourceIcon[1],2);

	WriteSeqIndexPort(Addr+2,tySRAM_CODE_CC_CD,ColorStatusIcon[FuncBuf[pCOLORTEMP]],2);
	// kevin 08202009 if no audio then mute always
	if(AudioMute || (!audioFlag))
		WriteSeqIndexPort(Addr+4,tySRAM_CODE_CC_CD,AudioStatusIcon[0],2);
	else
		WriteSeqIndexPort(Addr+4,tySRAM_CODE_CC_CD,AudioStatusIcon[1],2);

}
*/
/* // take of photo sensor on osd setup page 20090629
void ShowOnOff(Byte Ptr)
{

code unsigned char ENG_ON[]={
	6,"On    "
};
code unsigned char FRA_ON[]={
	6,"On    "
};
code unsigned char DEU_ON[]={
	6,"Ein   "
};
code unsigned char ESP_ON[]={
	6,"Act.  "
};
code unsigned char ITA_ON[]={
	6,"On    "
};
code unsigned char JAP_ON[]={
	6,"ON    "
};
code unsigned char RSU_ON[]={
	6,_B,_P00,_P19,_DOT,' ',' '
};
code unsigned char CHT_ON[]={
	6,_C55,' ',' ',' ',' ',' '
};
code unsigned char CHI_ON[]={
	6,_C1f,' ',' ',' ',' ',' '
};

code unsigned char ENG_OFF[]={
	6,"Off   ",
};
code unsigned char FRA_OFF[]={
	6,"Off   "
};
code unsigned char DEU_OFF[]={
	6,"Aus   "
};
code unsigned char ESP_OFF[]={
	6,"Desac."
};
code unsigned char ITA_OFF[]={
	6,"Off   "
};
code unsigned char JAP_OFF[]={
	6,"OFF   "
};
code unsigned char RSU_OFF[]={
	6,_B,_P1a,_P00,_P19,_DOT,' '
};
code unsigned char CHT_OFF[]={
	6,_C56,' ',' ',' ',' ',' '
};
code unsigned char CHI_OFF[]={
	6,_C40,' ',' ',' ',' ',' '
};

code unsigned char *OnOffTab[][9]={
	{ENG_OFF,FRA_OFF,DEU_OFF,ESP_OFF,ITA_OFF,JAP_OFF,RSU_OFF,CHT_OFF,CHI_OFF},
	{ENG_ON,FRA_ON,DEU_ON,ESP_ON,ITA_ON,JAP_ON,RSU_ON,CHT_ON,CHI_ON}
};
	Byte i;
	Byte on_off;
	Word code OnOffAddr[]={ROW5+17,ROW5+24};
	Word code OnOffAddr2[]={ROW7+17,ROW7+24};
	//Word code OnOffAddr3[]={ROW4+17,ROW4+24};
	//Word code OnOffAddr4[]={ROW3+24,ROW4+24,ROW5+24,ROW6+24,ROW7+24,ROW8+24};
	Word code OnOffAddr4[]={ROW3+20,ROW4+20,ROW5+20,ROW6+20,ROW7+20,ROW8+20};
	Word code OnOffAddr5[]={ROW3+24,ROW4+24,ROW5+24,ROW6+24,ROW7+24};
	Byte *Offset,Length;

	if(MenuPtr.Page == 1) // kevin 
	{	
		for(i=0;i<6;i++)
		{
			if(i==0)
				on_off = cvbs1;
			if(i==1)
				on_off = cvbs2;
			if(i==2)
				on_off = svideo;
		#if vga0_YPbPr1
			if(i==3)
				on_off = YPbPr;
		#else
			if(i==3)
				on_off = dsub;
		#endif
			if(i==4)
				on_off = dvid;
			if(i==5)						
				on_off = sensorFlag;
			Offset = OnOffTab[on_off][FuncBuf[pLANGUAGE]];
			Length = *Offset;
			Offset++;
#if decoder_TECHWELL
			if(i == MenuPtr.Ptr && i < 5)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else if(i ==5)
#else			
			if(i == MenuPtr.Ptr && i>2 && i < 5)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else if(i<=2  || i ==5)
#endif
				WriteWordIIC563(0x0cc,fGRAY|fBLACK);
			else
				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
			WriteSeqIndexPort(OnOffAddr4[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	}
	if(MenuPtr.Page == 2){
			for(i=1;i<4;i++)
			{
			if(i==1)
				on_off = AudioMute;
			if(i==2)
				on_off = 0;//Reverse_Flag;
			if(i==3)
				on_off = 0;//Mirror_Flag;
			Offset = OnOffTab[on_off][FuncBuf[pLANGUAGE]];
			Length = *Offset;
			Offset++;
			if(i == MenuPtr.Ptr)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else if(i == 2 || i == 3 ||!audioFlag)
				WriteWordIIC563(0x0cc,fGRAY|fBLACK);
			else
				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
			WriteSeqIndexPort(OnOffAddr5[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	}else{
	for(i=0;i<2;i++)
	{
		Offset = OnOffTab[i][FuncBuf[pLANGUAGE]];
		Length = *Offset;
		Offset++;
		if(Ptr == i)
			if(((FuncPtr == pMUTE && MenuPtr.Ptr == 1)||(FuncPtr == pAUTOSETTING && MenuPtr.Ptr == 3)) && MenuPtr.Level == 1)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else
				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		if(FuncPtr == pMUTE || FuncPtr == pVOLUME)
			WriteSeqIndexPort(OnOffAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		else if(FuncPtr == pAUTOSETTING)
			WriteSeqIndexPort(OnOffAddr2[i],tySRAM_CODE_CC_CD,Offset,Length);
		//else if(FuncPtr == pBF3MODE)
			//WriteSeqIndexPort(OnOffAddr3[i],tySRAM_CODE_CC_CD,Offset,Length);
	}
	}
}*/

void ShowOnOff(Byte Ptr)
{

code unsigned char ENG_ON[]={
	6,"On    "
};

code unsigned char ENG_OFF[]={
	6,"Off   ",
};


code unsigned char *OnOffTab[][9]={
	{ENG_OFF,ENG_OFF,ENG_OFF,ENG_OFF,ENG_OFF,ENG_OFF,RUS_Off,ENG_OFF,ENG_OFF},
	{ENG_ON,ENG_ON,ENG_ON,ENG_ON,ENG_ON,ENG_ON,RUS_On,ENG_ON,ENG_ON}
};
	Byte i;
	Byte on_off;
	//Word code OnOffAddr[]={ROW5+17,ROW5+24};
	//Word code OnOffAddr2[]={ROW7+17,ROW7+24};
	//Word code OnOffAddr4[]={ROW3+20,ROW4+20,ROW5+20,ROW6+20,ROW7+20,ROW8+20};
	//Word code OnOffAddr5[]={ROW3+24,ROW4+24,ROW5+24,ROW6+24,ROW7+24};
	Word code OnOffAddr[4]={ROW4+20,ROW6+20,ROW8+20,ROW10+20};
	Byte *Offset,Length;

	if(MenuPtr.Page == Menu_SourceSelect)
	{	
		for(i=0;i<4;i++)
		{
			if(i== 0)
				on_off = cvbs1;
			if(i==1)
				on_off = svideo;
			if(i==2)
				on_off = dsub;
			if(i==3)
				on_off = dvid;
			
			Offset = OnOffTab[on_off][FuncBuf[pLANGUAGE]];
			Length = *Offset;
			Offset++;
		
			if(i == MenuPtr.Ptr && MenuPtr.Level == Level_SubItem)
				WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
			else
				WriteWordIIC563(0x0cc,fWHITE|fBLACK);
			
			WriteSeqIndexPort(OnOffAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
	}
	
}
/*
void ShowUserRGBMenu()
{
	
	Byte *Offset,i,Length;
	Word RColor,GColor,BColor;
	Word code RGBAddr[6]={ROW3+4,ROW4+4,ROW5+4,ROW6+4,ROW7+4,ROW8+4};
	//ClearTextArea();
	for(i=4;i<7;i++)
		{
		if((i == (MenuPtr.Ptr)) && (MenuPtr.Level == 2))
			WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		Offset = Tab_RGB[FuncBuf[pLANGUAGE]*3+(i-4)];
		Length = *Offset;
		Offset++;
		WriteSeqIndexPort(RGBAddr[i-4],tySRAM_CODE_CC_CD,Offset,Length);
		}
	//Show user pRCOLOR,pGCOLOR,pBCOLOR
	if(MenuPtr.Ptr < 4){
		RColor = Read24C16(ep_Color4_R);
		GColor = Read24C16(ep_Color4_G);
		BColor = Read24C16(ep_Color4_B);
	}
	else{
		RColor = FuncBuf[pRCOLOR];
		GColor = FuncBuf[pGCOLOR];
		BColor = FuncBuf[pBCOLOR];
	}
	
	GetFuncRange(pRCOLOR);
	
	if(MenuPtr.Level == 2){	//change set color on user color
		if(MenuPtr.Ptr == 4){
			ShowBar(RGBAddr[0]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fMAGENTA|fBLACK,RColor);		
			ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
			ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
			ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
			ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
			ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		if(MenuPtr.Ptr == 5){
			ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
			ShowBar(RGBAddr[1]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fMAGENTA|fBLACK,GColor);
			ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
			ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
			ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
			ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		if(MenuPtr.Ptr == 6){
			ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
			ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
			ShowBar(RGBAddr[2]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fMAGENTA|fBLACK,BColor);
			ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
			ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
			ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		if(MenuPtr.Ptr == 7){
			ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
			ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
			ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
			ShowBar(RGBAddr[3]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			ShowValue(RGBAddr[3]+17,fMAGENTA|fBLACK,FuncBuf[pFTCORRECTION]);
			ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
			ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		if(MenuPtr.Ptr == 8){
			ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
			ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
			ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
			//ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			//ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
			//ShowBar(RGBAddr[4]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			//ShowValue(RGBAddr[4]+17,fMAGENTA|fBLACK,FuncBuf[pGREENENHANCE]);
			//ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			//ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		if(MenuPtr.Ptr == 9){
			//ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
			ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
			//ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
			ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
			//ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
			ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
			//ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
			//ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
			//ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
			//ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
			//ShowBar(RGBAddr[5]+8,fMAGENTA|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
			//ShowValue(RGBAddr[5]+17,fMAGENTA|fBLACK,FuncBuf[pBLUEENHANCE]);
		}
		//ShowKeyMenu(2,MenuPtr.Level);
	}
	else{		//the user set white char on color temp 
		//ShowBar(RGBAddr[0]+8,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
		ShowValue(RGBAddr[0]+17,fWHITE|fBLACK,RColor);
		//ShowBar(RGBAddr[1]+8,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
		ShowValue(RGBAddr[1]+17,fWHITE|fBLACK,GColor);
		//ShowBar(RGBAddr[2]+8,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
		ShowValue(RGBAddr[2]+17,fWHITE|fBLACK,BColor);
		//ShowBar(RGBAddr[3]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pFTCORRECTION]);
		//ShowValue(RGBAddr[3]+17,fWHITE|fBLACK,FuncBuf[pFTCORRECTION]);
		//ShowBar(RGBAddr[4]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pGREENENHANCE]);
		//ShowValue(RGBAddr[4]+17,fWHITE|fBLACK,FuncBuf[pGREENENHANCE]);
		//ShowBar(RGBAddr[5]+8,fWHITE|fBLACK,FuncMin,FuncMax,FuncBuf[pBLUEENHANCE]);
		//ShowValue(RGBAddr[5]+17,fWHITE|fBLACK,FuncBuf[pBLUEENHANCE]);
	}
//	ShowBar(RGBAddr[0]+10,fWHITE|fBLACK,FuncMin,FuncMax,RColor);
//	ShowBar(RGBAddr[1]+10,fWHITE|fBLACK,FuncMin,FuncMax,GColor);
//	ShowBar(RGBAddr[2]+10,fWHITE|fBLACK,FuncMin,FuncMax,BColor);
//	ShowValue(RGBAddr[0]+19,fWHITE|fBLACK,RColor);
//	ShowValue(RGBAddr[1]+19,fWHITE|fBLACK,GColor);
//	ShowValue(RGBAddr[2]+19,fWHITE|fBLACK,BColor);
		

	//ShowKeyMenu(2,MenuPtr.Level);

}
*/
/*
void ShowKeyMenu(Byte Page, Byte Level)
{
	Byte *Offset,Length;
	Byte code IncDecIcon[3]={
	//0x42,0x43,0x44
	30,31,32
	};
	Byte code MenuIcon[3]={
	//0x45,0x46,0x47
	33,34,35
	};
//	Byte code KeyStr[][6]={"Enter ","Select","Adjust"};

	WriteWordIIC563(0x0cc,fBLACK|bGRAY);
	if(MenuPtr.Menu == 3)
		WriteSeqIndexPort(ROW4+1,tySRAM_CODE_CC_CD,SPACE,28);
	else
		WriteSeqIndexPort(ROW16+1,tySRAM_CODE_CC_CD,SPACE,28);
	
	WriteWordIIC563(0x0cc,fBLACK|bGRAY|caONE_BIT_512);

	if(MenuPtr.Menu == 3){
		WriteSeqIndexPort(ROW4+1,tySRAM_CODE_CC_CD,MenuIcon,3);
		WriteSeqIndexPort(ROW4+15,tySRAM_CODE_CC_CD,IncDecIcon,3);
	}
 	else{
		WriteSeqIndexPort(ROW16+1,tySRAM_CODE_CC_CD,MenuIcon,3);
		if((Level == 0) ||(Level == 2)||(Page != 2 && Page != 3)||((Page == 2 && FuncBuf[pCOLORTEMP] == 3) ))
			WriteSeqIndexPort(ROW16+15,tySRAM_CODE_CC_CD,IncDecIcon,3);
//		else 
//			if(Page != 2 && Page != 3)
//				WriteSeqIndexPort(ROW13+18,tySRAM_CODE_CC_CD,IncDecIcon,3);
//			else if((Page == 2 && FuncBuf[pCOLORTEMP] == 3) && (Page != 3))
//				WriteSeqIndexPort(ROW13+18,tySRAM_CODE_CC_CD,IncDecIcon,3);
	}
	WriteWordIIC563(0x0cc,fBLACK|bGRAY);
	if(MenuPtr.Menu == 3){
		Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+2];
		Length = *Offset++;
		//if(MenuPtr.Ptr == 0 ){
		//	ShowDesc(pCONTRAST,ROW4+4,fBLACK|bGRAY);
		//	//WriteSeqIndexPort(ROW4+21,tySRAM_CODE_CC_CD,KeyStr[2],6);
		//}
		//else if(MenuPtr.Ptr == 1 ){
		//	ShowDesc(pBACKLIGHT,ROW4+4,fBLACK|bGRAY);
		//	//WriteSeqIndexPort(ROW4+21,tySRAM_CODE_CC_CD,KeyStr[2],6);
		//}
		WriteSeqIndexPort(ROW4+18,tySRAM_CODE_CC_CD,Offset,Length);

		Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3];
		Length = *Offset++;
		WriteSeqIndexPort(ROW4+4,tySRAM_CODE_CC_CD,Offset,Length);
	}		
	else
	{
		if(Level == 0 )
		{
			Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3];
			Length = *Offset++;
			WriteSeqIndexPort(ROW16+4,tySRAM_CODE_CC_CD,Offset,Length);
			//WriteSeqIndexPort(ROW13+4,tySRAM_CODE_CC_CD,KeyStr[0],6);
			Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+1];
			Length = *Offset++;
			WriteSeqIndexPort(ROW16+18,tySRAM_CODE_CC_CD,Offset,Length);
			//WriteSeqIndexPort(ROW13+18,tySRAM_CODE_CC_CD,KeyStr[1],6);
		}
		else if(Level == 1)
		{
			if((Page == 0) || (Page == 1) || (Page == 2)){
				Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+1];
				Length = *Offset++;
				WriteSeqIndexPort(ROW16+4,tySRAM_CODE_CC_CD,Offset,Length);
				Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+2];
				Length = *Offset++;
				WriteSeqIndexPort(ROW16+18,tySRAM_CODE_CC_CD,Offset,Length);
			}
			else if(Page == 5){
				Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+1];
				Length = *Offset++;
				WriteSeqIndexPort(ROW16+4,tySRAM_CODE_CC_CD,Offset,Length);
				Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3];
				Length = *Offset++;
				WriteSeqIndexPort(ROW16+18,tySRAM_CODE_CC_CD,Offset,Length);
			}
			else{
				Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+1];
				Length = *Offset++;
				WriteSeqIndexPort(ROW16+4,tySRAM_CODE_CC_CD,Offset,Length);
				if(Page == 2 && FuncBuf[pCOLORTEMP] == 3){
					Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3];
					Length = *Offset++;
					WriteSeqIndexPort(ROW14+18,tySRAM_CODE_CC_CD,Offset,Length);
				}
			}
		}
		else
		{
			Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+1];
			Length = *Offset++;
			WriteSeqIndexPort(ROW16+4,tySRAM_CODE_CC_CD,Offset,Length);
			Offset = KeyStrTab[FuncBuf[pLANGUAGE]*3+2];
			Length = *Offset++;
			WriteSeqIndexPort(ROW16+18,tySRAM_CODE_CC_CD,Offset,Length);
		}
	}
}
*/

/*
void ShowOSDPosition()
{
code unsigned char OSDPos1Str[]={
	1,"1"
};
code unsigned char OSDPos2Str[]={
	1,"2"
};
code unsigned char OSDPos3Str[]={
	1,"3"
};
code unsigned char OSDPos4Str[]={
	1,"4"
};
code unsigned char OSDPos5Str[]={
	1,"5"
};
	Byte code *OSDPosTab[]={
	OSDPos1Str,OSDPos2Str,OSDPos3Str,OSDPos4Str,OSDPos5Str
};

	Word code OSDPosAddr[5]={ROW5+17,ROW5+19,ROW5+21,ROW5+23,ROW5+25};
	Byte *Offset,i,Length;

	for(i=0;i<5;i++)
		{
		if(FuncBuf[pOSDHPOSITION] == i)
			if(MenuPtr.Ptr == 1 && MenuPtr.Level == 1)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else
				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		Offset = OSDPosTab[i];
		Length = *Offset;
		Offset++;
		WriteSeqIndexPort(OSDPosAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
		}
}
*/
/*
void ShowHotKey_BrightnessOrVolume()
{
	Byte *Offset,Length;
	ClearOSD();

	if(audioFlag)
	{		
		FuncPtr = OptionPageItem[0];
		Offset = DescTab[21][FuncBuf[pLANGUAGE]];
	}else{		
		FuncPtr = Page0Item[1];
		Offset = DescTab[0][FuncBuf[pLANGUAGE]];
	}
	
	Length = *Offset;
	Offset++;

	GetFuncRange(FuncPtr);
	ShowValue(ROW0+12,SOURCE_COLOR,FuncBuf[FuncPtr]);
	
	WriteWordIIC563(0x081,ROW1+12);
	WriteWordIIC563(0x084,5);
	WriteWordIIC563(0x0cc,SOURCE_COLOR);
	WriteSeqIndexPort(ROW0,tySRAM_CODE_CC_CD,Offset,Length);
	Osd_On(0);
	//OsdTimer = 10;
}
*/
/*
void ShowBrightnessMenu()
{
	WriteIIC563(0x080,0); // OSD disable
	ClearOSD();
	ShowOSDFrame(1);
	OsdPosition();
	OpenWindow(WINDOW4,WIN4_X1,WIN4_X2,WIN4_Y1,WIN4_Y2-9,WIN4_ATTR,WIN4_COLOR);
	OpenWindow(WINDOW3,WIN3_X1,WIN3_X2,WIN3_Y1,WIN3_Y2,WIN3_ATTR,WIN3_COLOR);
	if(FuncBuf[pVIDEOSOURCE] < 3){
		ShowResolution(HOTKEYMENU_RES_ADDR,RES_COLOR);
		//ShowStatus(ROW2+2,fGREEN|bBLACK);
	}
	else
		ShowTvSystem();
	ShowKeyMenu(0,MenuPtr.Level);
	
	FuncPtr = Page0Item[MenuPtr.Ptr];
	ShowDesc(FuncPtr,HOTKEYMENU_DESC_ADDR,SEL_FUN_COLOR);
	GetFuncRange(FuncPtr);
	//ShowBar(HOTKEYMENU_BAR_ADDR,SEL_FUN_COLOR,FuncMin,FuncMax,FuncBuf[FuncPtr]);
	ShowValue(HOTKEYMENU_VALUE_ADDR,SEL_FUN_COLOR,FuncBuf[FuncPtr]);
}*/

void ShowSharpness()
{
code unsigned char Sharp1Str[]={
	1,"1"
};
code unsigned char  Sharp2Str[]={
	1,"2"
};
code unsigned char  Sharp3Str[]={
	1,"3"
};
code unsigned char  Sharp4Str[]={
	1,"4"
};
code unsigned char  Sharp5Str[]={
	1,"5"
};
	Byte code * SharpTab[]={
	 Sharp1Str, Sharp2Str, Sharp3Str, Sharp4Str, Sharp5Str
};

	Word code  SharpAddr[5]={19,21,23,25,27};
	Byte *Offset,i,Length;

	for(i=0;i<5;i++)
	{
		if(PageImageSetup[MenuPtr.Ptr] == pSHARPNESS && 
			MenuPtr.Level == Level_SubItem &&
			FuncBuf[pSHARPNESS] == i)	
		{
			WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
		}else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
			
		Offset = SharpTab[i];
		Length = *Offset;
		Offset++;
		WriteSeqIndexPort( ROW12+SharpAddr[i],tySRAM_CODE_CC_CD,Offset,Length);

		
		// show onder bar
		if(FuncBuf[pSHARPNESS] == i)
		{
			WriteIndexPort( ROW13+SharpAddr[i],tySRAM_CODE_CC_CD,_OnderBar);
		}else
			WriteIndexPort( ROW13+SharpAddr[i],tySRAM_CODE_CC_CD,_Space);
	}
}

/*
void ShowTurboModeMenu()
{
	Byte i;
	//Byte *Offset,Length;

	WriteIIC563(0x080,0); // OSD disable
	ClearOSD();
	ShowOSDFrame(2);
//	OsdPosition();
	WriteWordIIC563(0x081,(PanelWidth-30*12)/2);
	WriteWordIIC563(0x084,(PanelHeight-5*18)/2);
//	WriteWordIIC563(0x081,460);
//	WriteWordIIC563(0x084,467);
	OpenWindow(WINDOW4,WIN4_X1,WIN4_X2,WIN4_Y1,WIN4_Y2-8,WIN4_ATTR,WIN4_COLOR);
	OpenWindow(WINDOW3,WIN3_X1,WIN3_X2,WIN3_Y1,WIN3_Y2,WIN3_ATTR,WIN3_COLOR);

	WriteWordIIC563(0x0cc,fMAGENTA|caONE_BIT_512);
	for(i=0;i<3;i++){
		WriteIndexPort(ROW1+3+i,tySRAM_CODE_CC_CD,i);
		WriteIndexPort(ROW2+3+i,tySRAM_CODE_CC_CD,3+i);
	}
	ShowTurboMode();
	ShowStatus(ROW3+2,fGREEN|bBLACK);
}
*/
/*
void ShowTurboMode()
{
	Byte *Offset,Length;
	Word temp;

	
	//show description
	WriteWordIIC563(0x0cc,fWHITE|bBLACK);
	WriteSeqIndexPort(ROW2+8,tySRAM_CODE_CC_CD,SPACE,20);
	
	if(!TextMode && !EconomyMode)
		temp = FuncBuf[pLANGUAGE]*3+0;
		//Offset = TurboStrTab[FuncBuf[pLANGUAGE]*3+0];
	else if(TextMode && !EconomyMode)
		temp = FuncBuf[pLANGUAGE]*3+1;
		//Offset = TurboStrTab[FuncBuf[pLANGUAGE]*3+1];
	else
		temp = FuncBuf[pLANGUAGE]*3+2;
		//Offset = TurboStrTab[FuncBuf[pLANGUAGE]*3+2];
	Offset = TurboStrTab[temp];
	Length = *Offset++;
	WriteSeqIndexPort(ROW2+7,tySRAM_CODE_CC_CD,Offset,Length);
}
*/
/*
void ShowBackLightTime()
{
	Byte VaSeq[5],Minute,Second;
	unsigned long Hour;

	Second = (Byte)(BackLightTimer % 60);
	Minute = (Byte) ((BackLightTimer / 60) % 60);
	//Hour = BackLightTimer / 3600;
	
	WriteWordIIC563(0x0cc,fGREEN|bBLACK);
	Hour = BackLightTimer /3600 / 10000;
	BCD(Hour,VaSeq,5);
	WriteSeqIndexPort(BACKLIGHTTIME_ADDR,tySRAM_CODE_CC_CD,VaSeq+4,1);
	Hour = (BackLightTimer /3600) % 10000;
	BCD(Hour,VaSeq,5);
	WriteSeqIndexPort(BACKLIGHTTIME_ADDR+1,tySRAM_CODE_CC_CD,VaSeq+1,4);
	WriteIndexPort(BACKLIGHTTIME_ADDR+5,tySRAM_CODE_CC_CD,':');
	BCD(Minute,VaSeq,2);
	WriteSeqIndexPort(BACKLIGHTTIME_ADDR+6,tySRAM_CODE_CC_CD,VaSeq+3,2);

	WriteSeqIndexPort(BACKLIGHTTIME_ADDR-4,tySRAM_CODE_CC_CD,"B/L",3);
	//WriteIndexPort(BACKLIGHTTIME_ADDR-4,tySRAM_CODE_CC_CD,'B');
	//WriteIndexPort(BACKLIGHTTIME_ADDR-3,tySRAM_CODE_CC_CD,OSD_index_general+11);//"/"
	//WriteIndexPort(BACKLIGHTTIME_ADDR-2,tySRAM_CODE_CC_CD,'L');
#if 0	
	WriteIndexPort(ACKLIGHTTIME_ADDR+7,tySRAM_CODE_CC_CD,':');
	BCD(Second,VaSeq,2);
	WriteSeqIndexPort(ACKLIGHTTIME_ADDR+8,tySRAM_CODE_CC_CD,VaSeq+3,2);
	Hour = BackLightTimer / 10000;
	BCD(Hour,VaSeq,5);
	WriteSeqIndexPort(ROW1+19,tySRAM_CODE_CC_CD,VaSeq,5);
	Hour = BackLightTimer % 10000;
	BCD(Hour,VaSeq,4);
	WriteSeqIndexPort(ROW1+24,tySRAM_CODE_CC_CD,VaSeq+1,4);
#endif	
}

void ShowScalerMode(Byte Ptr)
{
code unsigned char ENG_43[]={
	3,"4:3"
};
code unsigned char ENG_54[]={
	3,"5:4"
};



code unsigned char *ScalerModeTab[]={
	ENG_43,ENG_54,
};
	Byte i;
	Word code ScalerModeAddr[]={ROW9+17,ROW9+24};
	Byte *Offset,Length;

	
	for(i=0;i<2;i++)
	{
		Offset = ScalerModeTab[i];
		Length = *Offset;
		Offset++;
		if(Ptr == i)
			if(((FuncPtr == pSCALERMODE && MenuPtr.Ptr == 5)) && MenuPtr.Level == 1)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else
				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		WriteSeqIndexPort(ScalerModeAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
	}
}


void ShowFEngine()
{

code unsigned char ENG_MOVIE[]={
	4,"Mov1"
};
code unsigned char ENG_TEXT[]={
	4,"Mov2"
};
code unsigned char ENG_USER[]={
	5,"Photo"
};
code unsigned char ENG_NORMAL[]={
	6,"Normal"
};

code unsigned char *OnOffTab[]={
	ENG_MOVIE,ENG_TEXT,ENG_USER,ENG_NORMAL
};
	Byte i,Ptr;
	Word code OnOffAddr[]={ROW5+3,ROW5+9,ROW5+15,ROW5+22};
	Byte *Offset,Length;

	Ptr = FuncBuf[pBF3MODE] & 0x03;

	for(i=0;i<4;i++)
	{
		Offset = OnOffTab[i];
		Length = *Offset;
		Offset++;
		if(Ptr == i) {
			if(MenuPtr.Level == 1 && FuncPtr == pBF3MODE)
				WriteWordIIC563(0x0cc,fMAGENTA|fBLACK);
			else
				WriteWordIIC563(0x0cc,fGREEN|fBLACK);
		}
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);
		
		WriteSeqIndexPort(OnOffAddr[i],tySRAM_CODE_CC_CD,Offset,Length);
	}
}
*/

void ShowHotKey_Contrast()
{
	Byte *Offset,Length;
	ClearOSD();
	
	Offset = DescTab[pCONTRAST][FuncBuf[pLANGUAGE]];		
	Length = *Offset;
	Offset++;
	
	GetFuncRange(pCONTRAST);
	ShowValue(ROW0+Length+ 1,SOURCE_COLOR,FuncBuf[pCONTRAST]);
		
	WriteWordIIC563(0x081,ROW0);
	WriteWordIIC563(0x084,5);
	WriteWordIIC563(0x0cc,SOURCE_COLOR);
	WriteSeqIndexPort(ROW0,tySRAM_CODE_CC_CD,Offset,Length);
	Osd_On(0);

}

void showHotkeydefine(void)
{
	
code unsigned char ENG_NONE[]={
	17,"      None       "
};
code unsigned char ENG_CHANNELSWITCHING[]={
	17,"Channel switching"
};
code unsigned char ENG_CONTRAST[]={
	17,"    Contrast     "
};

code unsigned char *StrTab[][3]={
	{ENG_NONE,ENG_CHANNELSWITCHING,ENG_CONTRAST},
	{ RUS_None,RUS_ChannelSwitching,RUS_CONTRAST}
};
	Byte *Offset,Length,language,clr_len;
	Word Addr;

		if(FuncBuf[pLANGUAGE] == Language_ENG)
		{
			language =0;
			Addr = ROW14+ 13;
			clr_len=17;
		}else{
			language = 1;
			Addr = ROW14+ 18;
			clr_len=20;
		}
		
		Offset = StrTab[language][FuncBuf[pHotKey]];
		Length = *Offset;
		Offset++;
		
		if(PageMainMenu[MenuPtr.Ptr] == pHotKey && MenuPtr.Level == Level_SubItem) 
			WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
		else
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);		

		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,SPACE,clr_len);
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Offset,Length);
}

void ShowSignalAndPlay()
{

code unsigned char noneStr[]={
	4,"None"
};
code unsigned char cvbsStr[]={
	5,"CVBS1"
};
code unsigned char svStr[]={
	7,"S_Video"
};
code unsigned char vgaStr[]={
	3,"VGA"
};

code unsigned char dviStr[]={
	5,"DVI-D"
};

code unsigned char autoStr[]={
	4,"Auto"
};

	Byte code *signalTab[][6]={
							{ noneStr, cvbsStr, svStr, vgaStr, dviStr, autoStr},
							{ RUS_None, cvbsStr, svStr, vgaStr, dviStr, RUS_Auto}
	};

	Byte *Offset,Length,language,clr_len;
	Word Addr;

		if(FuncBuf[pLANGUAGE] == Language_ENG)
		{
			language =0;
			Addr = ROW12+20;
			clr_len=10;
		}else{
			language = 1;
			Addr = ROW12+22;
			clr_len=12;
		}
		Offset = signalTab[language][FuncBuf[pDEFAULTCHANNEL]];
		Length = *Offset;
		Offset++;
		
		if(PageSourceSelect[MenuPtr.Ptr] == pDEFAULTCHANNEL&& MenuPtr.Level == Level_SubItem)
			WriteWordIIC563(0x0cc,fYELLOW|fBLACK);
		else			
			WriteWordIIC563(0x0cc,fWHITE|fBLACK);		

		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,SPACE,clr_len);
		WriteSeqIndexPort(Addr,tySRAM_CODE_CC_CD,Offset,Length);

		
}

#endif
