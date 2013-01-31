#include "MATH.H"
#include "F63XDEF.H"
#include "UserAdj.H"
#include "RAM.H"
#include "IIC.H"
#include "MyDef.H"
#include "stdio.h"
#include "Scaler.H"
#include "F63XREG.H"
#include "IIC.H"
#include "Rom_map.H"
#include "OSD_TAB.H"
#include "sRGB.H"
#include "AUTOADJ.H"
#include "MODEHANDLE.H"
#include "PANEL.H"
#include "MCU.H"
//#include "SAA7114.H"
#include "TW9919.H"

void AdjustProcess(void)
{
	Bit SaveFlag;
	//Byte Temp,Temp1;

	if(MenuPtr.Op == 0)
		return;
//Main Menu ----------------------------------------------------------------
	SaveFlag = 0;
	if(MenuPtr.Menu == MenuType_General || MenuType_HotKey)
	{
		if(MenuPtr.Level == Level_MainItem)
		{
			switch(MenuPtr.Page)
			{
				case Menu_Main:
					FuncPtr	= PageMainMenu[MenuPtr.Ptr];
					BackItem	= PageMainMenu[BackPtr];
					break;		
				case Menu_ImageSetup:
					FuncPtr	= PageImageSetup[MenuPtr.Ptr];
					BackItem	= PageImageSetup[BackPtr];
					break;		
				case Menu_SourceSelect:
					FuncPtr	= PageSourceSelect[MenuPtr.Ptr];
					BackItem	= PageSourceSelect[BackPtr];					
					break;		
				case Menu_OSDSetup:					
					FuncPtr	= PageOSDSetup[MenuPtr.Ptr];
					BackItem	= PageOSDSetup[BackPtr];					
					break;
				case Menu_ChannelSelect:				
					FuncPtr	= PageChannelSelect[MenuPtr.Ptr];
					BackItem	= PageChannelSelect[BackPtr];					
					break;			
				case HotKeyMenu_Contrast:
					FuncPtr	= pCONTRAST;
					break;	
				case HotKeyMenu_ChannelSelect:					
					FuncPtr	= PageHotKeyChannelSelect[MenuPtr.Ptr];
					BackItem	= PageHotKeyChannelSelect[BackPtr];					
					break;		
			};

				
		}

		if(FuncPtr >= pFACTMENU && FuncPtr < pFTCORRECTION) 
		{
			if(MenuPtr.Op == oENTER)
			{
				switch(FuncPtr)
				{
					case pRECALL:
						Recall(1);
						MenuPtr.Menu = MenuType_General;
						MenuPtr.Ptr = 0;			
						MenuPtr.Page = Menu_Main;
						MenuPtr.Level = Level_MainItem;
						ClearTextArea();
						ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
						break;
					case pAUTOADJUST:	
						
						if(FuncBuf[pVIDEOSOURCE] == VGAinput&& SyncMode !=2)
						{
							IRCode=0;
							Osd_Off();
							ShowMessage(MSG_AUTOTUNE);
							AutoTune();//AutoTest();
							Osd_Off();
						}else{
							MenuPtr.Menu = MenuType_General;
							MenuPtr.Ptr = 0;			
							MenuPtr.Page = Menu_Main;
							MenuPtr.Level = Level_MainItem;
						}
						break;
					};
				}
		}
		else{
			
			GetFuncRange(FuncPtr);
			if(MenuPtr.Op == oDECVAL || MenuPtr.Op == oINCVAL ||MenuPtr.Op == oENTER)
			{
				SetFuncMaxMin();
				
				switch(FuncPtr)
				{
					case pBRIGHTNESS:
						if(FuncBuf[pVIDEOSOURCE] <= DVIinput)
							SetBrightness();
						else
							SetVideoBrightness();
						SaveFlag = 1;
						break;
					case pCONTRAST:
						if(FuncBuf[pVIDEOSOURCE] <= DVIinput)
							SetContrast();
						else
							SetVideoContrast();
						SaveFlag = 1;
						break;
					case pPHASE:
						if(FuncBuf[pVIDEOSOURCE] == VGAinput)
						{
							SetADC_Phase();
							SaveFlag = 1;
						}
						break;
					case pCLOCK:
						if(FuncBuf[pVIDEOSOURCE] == VGAinput)
						{
							SetADC_PLL();
							SaveFlag = 1;
						}
						break;
					case pHPOSITION:
						if(FuncBuf[pVIDEOSOURCE] != DVIinput)
						{
							SetHP();
							SaveFlag = 1;
						}
						break;
					case pVPOSITION:
						if(FuncBuf[pVIDEOSOURCE] != DVIinput)
						{
							SetVP();
							SaveFlag = 1;
						}
						break;
					case pOSDHPOSITION:
					case pOSDVPOSITION:
						OsdPosition();
						SaveFlag = 1;
						break;
					case pOSDTIMER:
						SaveFlag = 1;
						break;
					case pRCOLOR:
					case pGCOLOR:
					case pBCOLOR:
						SetContrast();
						SaveFlag = 1;
						break;
					case pCOLORTEMP:
						LoadColor(FuncBuf[pCOLORTEMP]);					
						SetContrast();
						SetInverter();						
						ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
						SaveFlag = 1;
						break;
					case pDEFAULTCHANNEL:	
						setAutoPowerOnSignal();							
						ShowOnOff(MenuPtr.Ptr);						
						ShowSignalAndPlay();
						SaveFlag = 1;
						break;
					case pLANGUAGE:										
						ClearTextArea();
						ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);
						ShowLanguage();
						SaveFlag = 1;
						break;
					case pSHARPNESS:
						if(FuncBuf[pVIDEOSOURCE] < CVBSinput)
							SetSharpness();
						else
							SetVideoSharpness();
						SaveFlag = 1;
						break;
					case pTRANSOSD:
						SetOsdTrans();
						SaveFlag = 1;
						break;
					case pVIDEOSOURCE:
//							Write24C16(ep_Sync_Source,FuncBuf[pVIDEOSOURCE]);//benson_test
							FastMuteEnable = 0;
							MessageShow = 0;	//show msg if Video sync off
						break;
					case pSETCVBS1:
						if(MenuPtr.Op != oENTER)
						{
							cvbs1 = !cvbs1;
							// if all input is turn off then force the input from cvbs1
							if((InputMode & 0x3f) == 0)
									InputMode |= 0x01;

							Write24C16(ep_inputmode,InputMode);

							if((!cvbs1) && (FuncBuf[pAUTOPOWERON] == AV1input))
							{
								FuncBuf[pDEFAULTCHANNEL] =DefaultChannel_None;
								setAutoPowerOnSignal();								
								ShowSignalAndPlay();						
								Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
							}
						}
						break;
					case pSETCVBS2:
						
						if(MenuPtr.Op != oENTER)
						{
							cvbs2 = !cvbs2;
							// if all input is turn off then force the input from cvbs2
							if((InputMode & 0x3f) == 0)
									InputMode |= 0x02;
							Write24C16(ep_inputmode,InputMode);
							
							if((!cvbs2) && (FuncBuf[pAUTOPOWERON] == AV2input))
							{
								FuncBuf[pDEFAULTCHANNEL] =DefaultChannel_None;
								setAutoPowerOnSignal();	
								ShowSignalAndPlay();						
								Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
							}
						}
						break;
					case pSETSVIDEO:	
						
						if(MenuPtr.Op != oENTER)
						{
							svideo = !svideo;
							// if all input is turn off then force the input from svideo
							if((InputMode & 0x3f) == 0)
									InputMode |= 0x04;
							Write24C16(ep_inputmode,InputMode);
							
							if((!svideo) && (FuncBuf[pAUTOPOWERON] == Svideoinput))
							{
								FuncBuf[pDEFAULTCHANNEL] =DefaultChannel_None;
								setAutoPowerOnSignal();	
								ShowSignalAndPlay();						
								Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
							}
						}
						break;		

					case pSETDSUB:
						
						if(MenuPtr.Op != oENTER)
						{
							dsub = !dsub;
							// if all input is turn off then force the input from dsub
							if((InputMode & 0x3f) == 0)
									InputMode |= 0x08;
							Write24C16(ep_inputmode,InputMode);
							
							if((!dsub && FuncBuf[pAUTOPOWERON] == VGAinput) ||
								(!dsub && !dvid && FuncBuf[pDEFAULTCHANNEL] ==DefaultChannel_AUTO))
							{
								FuncBuf[pDEFAULTCHANNEL] =DefaultChannel_None;
								setAutoPowerOnSignal();	
								ShowSignalAndPlay();						
								Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
							}
						}
						break;

					case pSETDVID:
						
						if(MenuPtr.Op != oENTER)
						{
							dvid = !dvid;
							// if all input is turn off then force the input from dvid
							if((InputMode & 0x3f) == 0)
									InputMode |= 0x10;
							Write24C16(ep_inputmode,InputMode);
							
							if((!dvid && FuncBuf[pAUTOPOWERON] == DVIinput) ||
							     (!dsub && !dvid && FuncBuf[pDEFAULTCHANNEL] ==DefaultChannel_AUTO))
							{
								FuncBuf[pDEFAULTCHANNEL] =DefaultChannel_None;
								setAutoPowerOnSignal();	
								ShowSignalAndPlay();						
								Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
							}
						}
						break;
					case pSELECTDSUB:
						if(dsub)
							FuncBuf[pVIDEOSOURCE] =VGAinput;
						else
							GoToNextAvailableChannel();

						ChannelSelect();							
						break;
					case pSELECTDVID:
						if(dvid)
							FuncBuf[pVIDEOSOURCE] =DVIinput;
						else
							GoToNextAvailableChannel();

						ChannelSelect();	
						break;
					case pSELECTSVIDEO:
						if(svideo)
							FuncBuf[pVIDEOSOURCE] =Svideoinput;
						else
							GoToNextAvailableChannel();
						
						ChannelSelect();
						break;
					case pSELECTCVBS:
						if(cvbs1)
							FuncBuf[pVIDEOSOURCE] =AV1input;
						else
							GoToNextAvailableChannel();
						
						ChannelSelect();
						break;
					case pHotKey:	
						//printf("hot key=%d \n",(unsigned short)FuncBuf[pHotKey]);
						SaveFlag = 1;
						break;
					
					case pOSDRotate:
						//OSDAPI_LoadIndentFont();
						SetRotate();
						
						break;
					case pOPTION:
						if(MenuPtr.Op == oENTER)
						{
							//MenuPtr.Menu = MenuType_General;
							//MenuPtr.Ptr = 0;							
							//MenuPtr.Page = Menu_SourceSelect;		
							//MenuPtr.Level = Level_MainItem;
							//MenuPtr.Op = oENTER;
							//FuncPtr	= PageSourceSelect[MenuPtr.Ptr];
							OSDChannelSelectMode=TRUE;	
							//ClearTextArea();
							//ShowSubMenu(MenuPtr.Page,MenuPtr.Ptr);							
						}
						break;
				};
				SaveFlag = 1;
			}
		}
		
		if(SaveFlag)
		{
			SavePointer = FuncPtr;
			SaveTimer = SAVE_FUNC_TIME;
		}
	}
}

void setAutoPowerOnSignal(void)
{
	// default channel  0:None, 1:CVBS 2:S-video, 3:VGA, 4:DVI-D 5:Auto
	Byte default_channel_list[6]={ noneinput, CVBSinput, Svideoinput, VGAinput, DVIinput, AutoPower};
	Bool not_find=TRUE;
	

	// get avaliable channel
	while(not_find)
	{
	
		// get auto poweron on signal 
		FuncBuf[pAUTOPOWERON] = default_channel_list[FuncBuf[pDEFAULTCHANNEL]];

		switch(FuncBuf[pAUTOPOWERON])
		{
			default:
			case AutoPower:
				if(dsub ||dvid) not_find = FALSE;
				break;
			case noneinput:
				not_find = FALSE;
				break;
			case CVBSinput:
				if(cvbs1) not_find = FALSE;
				break;
			case Svideoinput:
				if(svideo) not_find = FALSE;
				break;
			case VGAinput:
				if(dsub) not_find = FALSE;
				break;
			case DVIinput:
				if(dvid) not_find = FALSE;
				break;	
		}
		if(not_find)
		{
			if(++FuncBuf[pDEFAULTCHANNEL] > DefaultChannel_AUTO)
				FuncBuf[pDEFAULTCHANNEL] = DefaultChannel_None;
		}
	}

	Write24C16(ep_Default_Channel,FuncBuf[pDEFAULTCHANNEL]);
	//printf("auto-power=%d \n",FuncBuf[pAUTOPOWERON]);
}

void SetFuncMaxMin()
{
	Byte code PolTab[7]={oINCVAL,oDECVAL,oDECVAL,oINCVAL,oINCFUNC,oDECFUNC,oUPDATE};
	Byte Polary,Units;
	bit Type;
	Word Min,Max;
	if(FuncPtr >= pFACTMENU && FuncPtr < pFTCORRECTION)
		return;
	Type = 0;
	switch(FuncPtr)
		{
		case pHPOSITION:
			Max = FuncMax;	//limited
			Min = FuncMin;
			Polary = 2;
			Units = 1;
			break;
		case pVIDEOSOURCE:	//cycle
			Type = 1;
			Max = FuncMax;
			Min = FuncMin;
			Polary = 0;
			Units = 1;
			break;
		case pRCOLOR:	
		case pGCOLOR:	
		case pBCOLOR:	
			Max =FuncMax;
			Min = FuncMin;
			Polary = 0;
			Units = 2;
			break;
		case pCOLORTEMP:
		case pOSDHPOSITION:
		case pOSDVPOSITION:		
		case pDEFAULTCHANNEL:
			Max = FuncMin;
			Min = FuncMax;
			Polary = 0;
			Units = 1;
			break;
		case pLANGUAGE:
			Max = FuncMin;
			Min = FuncMax;
			Polary = 0;
			Units = Language_Rus;
			break;
		case pVOLUME:
		case pOSDTIMER:
			Max = FuncMax;
			Min = FuncMin;
			Polary = 0;
			Units = 1;
			break;
		case pBLUEENHANCE:
		case pGREENENHANCE:
		case pFTCORRECTION:
			Max = FuncMax;
			Min = FuncMin;
			Polary = 0;
			Units = 2;
			break;
		default:
			Max = FuncMax;
			Min = FuncMin;
			Polary = 0;
			Units = 1;
			break;
		};

	if(!Type)
	{
		if(MenuPtr.Op == PolTab[Polary])
			{
			if(FuncBuf[FuncPtr] < FuncMax && (FuncBuf[FuncPtr] + Units) <= FuncMax)
				FuncBuf[FuncPtr] = FuncBuf[FuncPtr] + Units;
			else
				FuncBuf[FuncPtr] = Max;
			}
		if(MenuPtr.Op == PolTab[Polary+1])
			{
			if(FuncBuf[FuncPtr] > FuncMin && FuncBuf[FuncPtr] >= Units)
				FuncBuf[FuncPtr] = FuncBuf[FuncPtr] - Units;
			else
				FuncBuf[FuncPtr] = Min;}
	}
	else
	{
		if((MenuPtr.Op == PolTab[Polary]))
		{
			if(FuncBuf[FuncPtr] > FuncMax)
				FuncBuf[FuncPtr] = Max;
		}
	}

		
	#if PRINT_MESSAGE == 1
	printf("FTC control %d\n",(unsigned char)FuncBuf[pFTCORRECTION]);
	#endif
}

void GetFuncRange(Byte Ptr)
{
	FuncMin = 0;
	switch(Ptr)
		{
		case pBRIGHTNESS:
			FuncMax = 100;
			break;
		case pCONTRAST:
			FuncMax = FuncBuf[pContMax];
			FuncMin= FuncBuf[pContMin];
			break;
		case pPHASE:
			FuncMax = 0x3f;
			break;
		case pCLOCK:
			FuncMax = ClockBase + 50;
			FuncMin = ClockBase - 50;
			break;
		case pBACKLIGHT:
		case pBACKLIGHT2:
			FuncMax = FuncBuf[pBrigMax];
			FuncMin= FuncBuf[pBrigMin];
			break;
		case pHPOSITION:
			if(FuncBuf[pVIDEOSOURCE]<DVIinput)
				{
					if(ModePoint >= UserModeSt){
					// Set Minimum and Maximum
						FuncMin = 1;
						FuncMax = ClockBase - Hresolution - 2;
					}
					else{
					// Set Minimum and Maximum
						if(HPositionBase > (ClockBase - Hresolution)/4)
							FuncMin = HPositionBase - (ClockBase - Hresolution)/4;
						else
							FuncMin = 1;
						FuncMax = HPositionBase + (ClockBase - Hresolution)/4;
					}
				}else{
					FuncMax = 182;
					FuncMin = 82;
				}
			break;
		case pVPOSITION:
			if(FuncBuf[pVIDEOSOURCE] < DVIinput)
				// kevin 09042009 if((SyncMode == 2) &&(ModePoint ==9 ||ModePoint ==75 ||ModePoint ==77 ||ModePoint ==80))// kevin 07022008
				if(Interlance)
				{
					FuncMax = (VTotal * 2) - Vresolution + 20;
				}else
					FuncMax = VTotal - Vresolution + 20;
			else
				FuncMax = 100;
			FuncMin = 0;
			
			break;
		case pOSDHPOSITION:
		case pOSDVPOSITION:
			FuncMax = 100;
			break;
		case pOSDTIMER:
			FuncMin = 10;
			FuncMax = 60;
			break;
		case pSATURATION:
			FuncMax = 100;
			break;
		case pTINT:
			FuncMax = 100;
			break;
		case pRCOLOR:	case pGCOLOR:	case pBCOLOR:
		case pRADC:		case pGADC:		case pBADC:
		case pLIGHTSATURATION:
		case pLIGHTHUE: 
		case pROFFSET:	case pGOFFSET:	case pBOFFSET:
		case pGREENENHANCE:
		case pBLUEENHANCE:
		case pFTCORRECTION:
			FuncMax = 255;
			break;
		case pCOLORTEMP:
			FuncMax = MAX_COLOR_TEMP;
			break;
		case pLANGUAGE:
			FuncMax = Language_Rus;
			break;
		case pSHARPNESS:
				FuncMax = 4;
			break;
		case pTRANSOSD:
			FuncMax = 0x07;
			break;
		case pVIDEOSOURCE:
			//if(Tw9919En)
				FuncMax = Svideoinput;
			//else
			//	FuncMax = DVIinput;
			break;
		case pVOLUME:
			FuncMax = 100;
			break;
		case pVCMVOL:
			FuncMax = 0xe6;
			FuncMin = 0x63;
			break;
		case pContMax:
			FuncMax = 100;
			FuncMin = 0;
			break;
		case pContMin:
			FuncMax = 50;
			FuncMin = 0;
			break;
		case pBrigMax:
			FuncMax = 127;
			FuncMin = 0;
			break;
		case pBrigMin:
			FuncMax = 50;
			FuncMin = 0;
			break;
		case pLIGHTBRIGHT:
		case pLIGHTCONTRAST:
			FuncMax = 255;
			break;
		case pLIGHTHSTART:
		case pLIGHTHWIDTH:
		case pLIGHTVSTART:
		case pLIGHTVHIGHT:
			FuncMin = 0;
			FuncMax = 100;
			break;
		case pYPEAKING:
		case pCPEAKING:  //only control Y/C Gain, 1~4 control, 0 mean disable
			FuncMin = 0;
			FuncMax = 4;
			break;
		case pACCMODE:
		case pRCMMODE:
		case pBF3MODE:
			FuncMin = 0;
			FuncMax = 3;
			break;
		//case pAUTOPOWERON:
		//	FuncMin = VGAinput;
		//	FuncMax = Svideoinput;
		//	break;
		case pDEFAULTCHANNEL:
			FuncMin = DefaultChannel_None;
			FuncMax = DefaultChannel_AUTO;
			break;
		case pOSDRotate:
			FuncMin = OSD_Rotate_0;
			FuncMax = OSD_Rotate_270;
			break;
		case pHotKey:
			FuncMin = HotKey_None;
			FuncMax = HotKey_Contrast;
			break;
		};
}
/*
void Recall(bit All)
{
	if(All)
	{
		if(FuncBuf[pVIDEOSOURCE] == 0)
		{
			FuncBuf[pBRIGHTNESS] = 50;
			FuncBuf[pCONTRAST] = 50;
			FuncBuf[pCLOCK] = 50;
			FuncBuf[pPHASE] = 20;
			SetBrightness();
			LoadColor(FuncBuf[pCOLORTEMP]);
			SetContrast();
			//SetADC_PLL();
			//SetADC_Phase();
			SetSharpness();
		}
		if(FuncBuf[pVIDEOSOURCE] == 1)
		{
			FuncBuf[pBRIGHTNESS] = 50;
			FuncBuf[pCONTRAST] = 50;
			FuncBuf[pSHARPNESS] = 10;
			SetBrightness();
			LoadColor(FuncBuf[pCOLORTEMP]);
			SetContrast();
			SetSharpness();
		}
		if(FuncBuf[pVIDEOSOURCE] == 3)
		{
			FuncBuf[pBRIGHTNESS] = 50;
			FuncBuf[pCONTRAST] = 50;
			FuncBuf[pTINT] = 50;
			FuncBuf[pSATURATION] = 50;
			FuncBuf[pSHARPNESS] = 10;
			SetVideoBrightness();
			SetVideoContrast();
			SetVideoSaturation();
			SetVideoSharpness();
			SetVideoTint();
		}
		if(FuncBuf[pVIDEOSOURCE] == 4)
		{
			FuncBuf[pBRIGHTNESS] = 50;
			FuncBuf[pCONTRAST] = 50;
			FuncBuf[pTINT] = 50;
			FuncBuf[pSATURATION] = 50;
			FuncBuf[pSHARPNESS] = 10;
			SetVideoBrightness();
			SetVideoContrast();
			SetVideoSaturation();
			SetVideoSharpness();
			SetVideoTint();
		}
	}
}
*/

void Recall(bit All)
{
	Word addr;
	Byte i;
	if(FuncBuf[pVIDEOSOURCE] == VGAinput ||FuncBuf[pVIDEOSOURCE] == YPbPrinput)
		{
		if(ModePoint < UserModeSt)
			{
			addr = ModePoint * 8;
			for(i=0; i<6; i++)
				{
					if(i>=1 && i <=3)	// kevin 02/26/2008 by pass the h and v position
						;
					else
				Write24C16(addr+ep_Sync_Data,EEP_SyncMap[addr]);
				addr++;
				}
			addr++;
			Write24C16(addr+ep_Sync_Data,EEP_SyncMap[addr]);
			}
		if(All)
			{
			Write24C16(ep_Brightness,EEP_GeoMap[ep_Brightness-0x200]);	
			Write24C16(ep_Contrast,EEP_GeoMap[ep_Contrast-0x200]);	
			}
		LoadModeIndependentSettings();
		addr = ep_Sync_Data + (ModePoint * 8);
		FuncBuf[pSHARPNESS] = Read24C16(addr++);
		addr +=3;
		FuncBuf[pCLOCK] = Read24C16(addr++);
		FuncBuf[pCLOCK] <<= 8;
		FuncBuf[pCLOCK] |= Read24C16(addr++);
		addr++;
		FuncBuf[pPHASE] = Read24C16(addr);			
		SetADC_PLL();
		SetADC_Phase();
		
		//if(SyncMode != 3)
		//	{
		//	SetHP();
		//	SetVP();
		//	}
		if(Interlance){
#if PANEL == CMI_V470H1_L03 // kevin 02/15/2008 To fixed the 1920x1200 data overflow
			if(ResolutionPtr == 0x15 || ResolutionPtr == 0x17)
				WriteWordIIC563(0x032,Vresolution -1);		// Capture V_Active
			else
#endif			
			WriteWordIIC563(0x032,Vresolution);		// Capture V_Active
		}
		else{
//			WriteWordIIC563(0x032,Vresolution); 	// Capture V_Active
		}
		WriteWordIIC563(0x036,Hresolution); 	// Capture H_Active
//		WriteWordIIC563(0x036,Hresolution + H_ActErrTab[ResolutionPtr]); 	// Capture H_Active
//		SetScaler();
		// kevin 20090713 SetSharpness();
		// kevin 20090713 SetBrightness();
		// kevin 20090713 SetContrast();
		sRGB(50,(Byte)FuncBuf[pRCOLOR],(Byte)FuncBuf[pGCOLOR],(Byte)FuncBuf[pBCOLOR]);
		}
	if(FuncBuf[pVIDEOSOURCE] == DVIinput)
		{
		//Write24C16(ep_DVI_Contrast,EEP_GeoMap[ep_DVI_Contrast-0x200]);	
		Write24C16(ep_Contrast,EEP_GeoMap[ep_Contrast-0x200]);	
		Write24C16(ep_Brightness,EEP_GeoMap[ep_Brightness-0x200]);	
		Write24C16(ep_Color_Ptr,EEP_GeoMap[ep_Color_Ptr-0x200]);	
		//FuncBuf[pCONTRAST] = Read24C16(ep_DVI_Contrast);
		FuncBuf[pCONTRAST] = Read24C16(ep_Contrast);
		FuncBuf[pBRIGHTNESS] = Read24C16(ep_Brightness);
		// kevin 20090713 SetContrast();
		addr = ModePoint * 8;
		Write24C16(addr+ep_Sync_Data,EEP_SyncMap[addr]);
		FuncBuf[pSHARPNESS] = EEP_SyncMap[addr];
		addr = ep_Shape_Offset + (ModePoint * 8);
		FuncBuf[pSHARPNESS] = Read24C16(addr);
		// kevin 20090713 SetSharpness();
		}

	if(FuncBuf[pVIDEOSOURCE] > DVIinput)
		{
		Write24C16(ep_Video_Contrast,EEP_GeoMap[ep_Video_Contrast-0x200]);	
		Write24C16(ep_Video_Brightness,EEP_GeoMap[ep_Video_Brightness-0x200]);	
		Write24C16(ep_Video_Saturation,EEP_GeoMap[ep_Video_Saturation-0x200]);
		Write24C16(ep_Video_Tint,EEP_GeoMap[ep_Video_Tint-0x200]);		
		Write24C16(ep_Video_Shapness,EEP_GeoMap[ep_Video_Shapness-0x200]);		
		FuncBuf[pBRIGHTNESS] = Read24C16(ep_Video_Brightness);
		FuncBuf[pCONTRAST] = Read24C16(ep_Video_Contrast);
		FuncBuf[pSATURATION] = Read24C16(ep_Video_Saturation);
		FuncBuf[pSHARPNESS] = Read24C16(ep_Video_Shapness);
		FuncBuf[pTINT] = Read24C16(ep_Video_Tint);
		SetVideoBrightness();
		SetVideoContrast();
		SetVideoSaturation();
		SetVideoSharpness();
		SetVideoTint();
		sRGB(50,(Byte)FuncBuf[pRCOLOR],(Byte)FuncBuf[pGCOLOR],(Byte)FuncBuf[pBCOLOR]);
		}

	
	//common area
	//---9300k color
	Write24C16(ep_Color1_R,EEP_GeoMap[ep_Color1_R-0x200]);	//R 
	Write24C16(ep_Color1_G,EEP_GeoMap[ep_Color1_G-0x200]);	//G 
	Write24C16(ep_Color1_B,EEP_GeoMap[ep_Color1_B-0x200]);	//B 
	//---user color
	Write24C16(ep_Color2_R,EEP_GeoMap[ep_Color2_R-0x200]);	//R 
	Write24C16(ep_Color2_G,EEP_GeoMap[ep_Color2_G-0x200]);	//G 
	Write24C16(ep_Color2_B,EEP_GeoMap[ep_Color2_B-0x200]);	//B 
	//---user color
	Write24C16(ep_Color3_R,EEP_GeoMap[ep_Color3_R-0x200]);	//R 
	Write24C16(ep_Color3_G,EEP_GeoMap[ep_Color3_G-0x200]);	//G 
	Write24C16(ep_Color3_B,EEP_GeoMap[ep_Color3_B-0x200]);	//B 
	//---default color
	Write24C16(ep_Color4_R,EEP_GeoMap[ep_Color4_R-0x200]);	//R 
	Write24C16(ep_Color4_G,EEP_GeoMap[ep_Color4_G-0x200]);	//G 
	Write24C16(ep_Color4_B,EEP_GeoMap[ep_Color4_B-0x200]);	//B 
	//---Color tempeture
	Write24C16(ep_Color_Ptr,EEP_GeoMap[ep_Color_Ptr-0x200]); //Reset Color	
	FuncBuf[pCOLORTEMP] = Read24C16(ep_Color_Ptr);
	LoadColor(FuncBuf[pCOLORTEMP]);
	
	//---Osd Timer
	Write24C16(ep_Osd_Timer,EEP_GeoMap[ep_Osd_Timer-0x200]);
	//---Osd Position
	Write24C16(ep_OsdH_Pos,EEP_GeoMap[ep_OsdH_Pos-0x200]);	
	//Write24C16(ep_OsdV_Pos,EEP_GeoMap[ep_OsdV_Pos-0x200]);	
	//---Osd Effect
	Write24C16(ep_Osd_Effect,EEP_GeoMap[ep_Osd_Effect-0x200]);	
	//---Back light
	Write24C16(ep_BackLight,EEP_GeoMap[ep_BackLight-0x200]);	
	//---Audio Volume
	//Write24C16(ep_Volume,EEP_GeoMap[ep_Volume-0x200]);	
	//---Color tempeture
	FuncBuf[pCOLORTEMP] = Read24C16(ep_Color_Ptr);
	//---Osd Timer
	FuncBuf[pOSDTIMER] = Read24C16(ep_Osd_Timer);
	//---Osd Position
	FuncBuf[pOSDHPOSITION] = Read24C16(ep_OsdH_Pos);
	FuncBuf[pOSDVPOSITION] = Read24C16(ep_OsdV_Pos);
	//---Osd Effect
	FuncBuf[pTRANSOSD] = Read24C16(ep_Osd_Effect);
	//---Back light
	FuncBuf[pBACKLIGHT] = Read24C16(ep_BackLight);
	//---Audio Volume
	//FuncBuf[pVOLUME] = Read24C16(ep_Volume);
	
	LoadColor(FuncBuf[pCOLORTEMP]);
	SetBrightness();
	SetContrast();
	SetSharpness();
	SetOsdTrans();
	OsdPosition();
	SetInverter();
	AutoSetting = 1;
	Write24C16(ep_Status,StatusFlag);
}

void LoadModeIndependentSettings(void)
{
	Word Addr;
	FuncBuf[pBRIGHTNESS] = Read24C16(ep_Brightness);		//0xd0 = FuncBuf[pBRIGHTNESS]
	FuncBuf[pCONTRAST] = Read24C16(ep_Contrast);			//0xd1 = FuncBuf[pCONTRAST]
	FuncBuf[pSATURATION] = 50;							//0xc8 = FuncBuf[pSATURATION]
	FuncBuf[pTINT] = 50;									//0xc9 = FuncBuf[pTINT]
	FuncBuf[pOSDHPOSITION] = Read24C16(ep_OsdH_Pos);		//0xdd = osd position x
	FuncBuf[pOSDVPOSITION] = Read24C16(ep_OsdV_Pos);		//0xdd = osd position y
	FuncBuf[pOSDTIMER] = Read24C16(ep_Osd_Timer);			//0xcf = osd timer
	FuncBuf[pLANGUAGE] = Read24C16(ep_Language);			//0xe0 = Language
	FuncBuf[pBACKLIGHT2] = Read24C16(ep_sRGB_Backlight);
	FuncBuf[pTRANSOSD] = Read24C16(ep_Osd_Effect);
	if(FuncBuf[pTRANSOSD] > 7)
		FuncBuf[pTRANSOSD] = 0;
	FuncBuf[pCOLORTEMP] = Read24C16(ep_Color_Ptr);
	if(FuncBuf[pCOLORTEMP] > MAX_COLOR_TEMP)
		FuncBuf[pCOLORTEMP] = 0;
	Addr = FuncBuf[pCOLORTEMP] * 3 + ep_Color1_R;			//Select color temperature
	FuncBuf[pRCOLOR] = Read24C16(Addr++);
	FuncBuf[pGCOLOR] = Read24C16(Addr++);
	FuncBuf[pBCOLOR] = Read24C16(Addr);
	FuncBuf[pBACKLIGHT] = Read24C16(ep_BackLight);		//0xd5 = FuncBuf[pBACKLIGHT]
	FuncBuf[pVCMVOL] = Read24C16(ep_VCMVol);	//0x0f = vcom vol
	FuncBuf[pVOLUME] = Read24C16(ep_Volume);	//0xcf = volume
	FuncBuf[pContMax] = Read24C16(ep_ContMax);
	FuncBuf[pContMin] = Read24C16(ep_ContMin);
	FuncBuf[pBrigMax] = Read24C16(ep_BrigMax);
	FuncBuf[pBrigMin] = Read24C16(ep_BrigMin);
	FuncBuf[pBF3MODE] = 3;//ray 0509 Read24C16(ep_BF3_MODE);
	
	FuncBuf[pLIGHTBRIGHT] = Read24C16(ep_Light_Bright);
	FuncBuf[pLIGHTCONTRAST] = Read24C16(ep_Light_Contrast);
	FuncBuf[pLIGHTSATURATION] = Read24C16(ep_pLIGHTSATURATION);
	FuncBuf[pLIGHTHUE] = Read24C16(ep_pLIGHTHUE);
	FuncBuf[pBFINTENSITY] = Read24C16(ep_pBFINTENSITY);
	//inital for Demo with leftside screen BF1 turn-on
	FuncBuf[pLIGHTHSTART] = 0;
	FuncBuf[pLIGHTVSTART] = 0;
	FuncBuf[pLIGHTHWIDTH] = 50;
	FuncBuf[pLIGHTVHIGHT] = 100;
	/*
	FuncBuf[pLIGHTHSTART] = Read24C16(ep_Light_HSMSB);
	FuncBuf[pLIGHTHSTART] <<= 8;
	FuncBuf[pLIGHTHSTART] |= Read24C16(ep_Light_HSLSB);
	FuncBuf[pLIGHTHWIDTH] = Read24C16(ep_Light_HWMSB);
	FuncBuf[pLIGHTHWIDTH] <<= 8;
	FuncBuf[pLIGHTHWIDTH] |= Read24C16(ep_Light_HWLSB);
	FuncBuf[pLIGHTVSTART] = Read24C16(ep_Light_VSMSB);
	FuncBuf[pLIGHTVSTART] <<= 8;
	FuncBuf[pLIGHTVSTART] |= Read24C16(ep_Light_VSLSB);
	FuncBuf[pLIGHTVHIGHT] = Read24C16(ep_Light_VHMSB);
	FuncBuf[pLIGHTVHIGHT] <<= 8;
	FuncBuf[pLIGHTVHIGHT] |= Read24C16(ep_Light_VHLSB);
	*/
	
		
	FuncBuf[pFTCORRECTION] = Read24C16(ep_pFTCORRECTION);
	FuncBuf[pGREENENHANCE] = Read24C16(ep_pGREENENHANCE);
	FuncBuf[pBLUEENHANCE] = Read24C16(ep_pBLUEENHANCE);
	
	FuncBuf[pACCMODE] = Read24C16(ep_pACCMODE);  //bit0,1 = ACCMODE
	FuncBuf[pRCMMODE] = FuncBuf[pACCMODE]>>4;
	FuncBuf[pACCMODE] = FuncBuf[pACCMODE]&0x0f;
	if(FuncBuf[pACCMODE]>3)  FuncBuf[pACCMODE] = 3;
	if(FuncBuf[pRCMMODE]>3)  FuncBuf[pRCMMODE] = 3;

	FuncBuf[pYPEAKING] = Read24C16(ep_pYPEAKING);
	if(FuncBuf[pYPEAKING]>4)  FuncBuf[pYPEAKING] = 4;
	FuncBuf[pCPEAKING] = Read24C16(ep_pCPEAKING);
	if(FuncBuf[pCPEAKING]>4)  FuncBuf[pCPEAKING] = 4;

	BackLightTimer = Read24C16(ep_BkLightTime_Offset+3);
	BackLightTimer <<= 8;
	BackLightTimer |= Read24C16(ep_BkLightTime_Offset+2);
	BackLightTimer <<= 8;
	BackLightTimer |= Read24C16(ep_BkLightTime_Offset+1);
	BackLightTimer <<= 8;
	BackLightTimer |= Read24C16(ep_BkLightTime_Offset);

}

void LoadColor(Word Ptr)
{
	Word Addr;
	Addr = Ptr * 3 + ep_Color1_R;			//Select color temperature
	FuncBuf[pRCOLOR] = Read24C16(Addr++);
	FuncBuf[pGCOLOR] = Read24C16(Addr++);
	FuncBuf[pBCOLOR] = Read24C16(Addr);
}

void LoadModeDependentSettings(void)
{
//	Sharp,V_BP,H_BP,H_Tatol,Phase
unsigned short addr;
	addr = ep_Sync_Data + (ModePoint * 8);
	FuncBuf[pSHARPNESS] = Read24C16(addr++);
	FuncBuf[pVPOSITION] = Read24C16(addr++);
	FuncBuf[pHPOSITION] = Read24C16(addr++);
	FuncBuf[pHPOSITION] <<= 8;
	FuncBuf[pHPOSITION] |= Read24C16(addr++);
	FuncBuf[pCLOCK] = Read24C16(addr++);
	FuncBuf[pCLOCK] <<= 8;
	FuncBuf[pCLOCK] |= Read24C16(addr++);
	addr++;
	FuncBuf[pPHASE] = Read24C16(addr);	
}

void SaveModeDependentSettings(void)
{
unsigned short addr;
	addr = ep_Shape_Offset + (ModePoint * 8);
	Write24C16(addr++,(Byte)FuncBuf[pSHARPNESS]);
	Write24C16(addr++,(Byte)FuncBuf[pVPOSITION]);
	Write24C16(addr++,(Byte)(FuncBuf[pHPOSITION] >> 8));
	Write24C16(addr++,(Byte)FuncBuf[pHPOSITION]);
	Write24C16(addr++,(Byte)(FuncBuf[pCLOCK] >> 8));
	Write24C16(addr++,(Byte)FuncBuf[pCLOCK]);
	addr++;
	Write24C16(addr,(Byte)FuncBuf[pPHASE]);	
}

void SaveData(void)
{
unsigned short addr;
	switch(SavePointer)
		{
		case pBRIGHTNESS:
			if(FuncBuf[pVIDEOSOURCE] < AV1input)
			{	//Write24C16(ep_BackLight,FuncBuf[pBACKLIGHT]);
				Write24C16(ep_Brightness,FuncBuf[pBRIGHTNESS]);
				if(sensorFlag)					
					Write24C16(ep_PCADC,FuncBuf[pBRIGHTNESS]);
			}else{
				Write24C16(ep_Video_Brightness,FuncBuf[pBRIGHTNESS]);				
				if(sensorFlag)					
					Write24C16(ep_VideoADC,FuncBuf[pBRIGHTNESS]);
			}
			break;
		case pCONTRAST:
			switch(FuncBuf[pVIDEOSOURCE])
				{
				case VGAinput:
				case DVIinput:
					Write24C16(ep_Contrast,(Byte)FuncBuf[pCONTRAST]);
					break;				
				case CVBSinput:
				case Svideoinput:
					Write24C16(ep_Video_Contrast,(Byte)FuncBuf[pCONTRAST]);
					break;				
				};
			break;
		case pCLOCK:
			addr = ep_Clock_Offset + (ModePoint * 8);
			Write24C16(addr++,(Byte)(FuncBuf[pCLOCK] >> 8));
			Write24C16(addr,(Byte)FuncBuf[pCLOCK]);
			break;
		case pPHASE:
			addr = ep_Phase_Offset + (ModePoint * 8);
			Write24C16(addr,(Byte)FuncBuf[pPHASE]);
			break;
		case pHPOSITION:
			if(FuncBuf[pVIDEOSOURCE]<DVIinput)
				{
					addr = ep_Hpos_Offset + (ModePoint * 8);
					Write24C16(addr++,(Byte)(FuncBuf[pHPOSITION] >> 8));
					Write24C16(addr,(Byte)FuncBuf[pHPOSITION]);
				}else{
					if(Video_60Hz)
//					if(NTSCorPAL == 0)
						Write24C16(ep_CVBS_Hposition,(Byte)(FuncBuf[pHPOSITION]));
					else
						Write24C16(ep_PAL_Hposition,(Byte)(FuncBuf[pHPOSITION]));
				}
			break;
		case pVPOSITION:
			if(FuncBuf[pVIDEOSOURCE]<DVIinput)
				{
					addr = ep_Vpos_Offset + (ModePoint * 8);
					Write24C16(addr,(Byte)FuncBuf[pVPOSITION]);
				}else{
					if(Video_60Hz)
//					if(NTSCorPAL == 0)
						Write24C16(ep_CVBS_Vposition,(Byte)(FuncBuf[pVPOSITION]));
					else
						Write24C16(ep_PAL_Vposition,(Byte)(FuncBuf[pVPOSITION]));						
				}
			break;
		case pRCOLOR:
			addr = FuncBuf[pCOLORTEMP] * 3 + ep_Color1_R;
			Write24C16(addr,(Byte)FuncBuf[pRCOLOR]);
			break;
		case pGCOLOR:
			addr = FuncBuf[pCOLORTEMP] * 3 + ep_Color1_G;
			Write24C16(addr,(Byte)FuncBuf[pGCOLOR]);
			break;
		case pBCOLOR:
			addr = FuncBuf[pCOLORTEMP] * 3 + ep_Color1_B;
			Write24C16(addr,(Byte)FuncBuf[pBCOLOR]);
			break;
		case pOSDHPOSITION:
			Write24C16(ep_OsdH_Pos,(Byte)FuncBuf[pOSDHPOSITION]);
			break;
		case pOSDVPOSITION:
			Write24C16(ep_OsdV_Pos,(Byte)FuncBuf[pOSDVPOSITION]);
			break;
		case pOSDTIMER:
			Write24C16(ep_Osd_Timer,(Byte)FuncBuf[pOSDTIMER]);
			break;
		case pSATURATION:
			if(FuncBuf[pVIDEOSOURCE] < AV1input)
				Write24C16(ep_sRGB_Saturation,(Byte)FuncBuf[pSATURATION]);
			else
				Write24C16(ep_Video_Saturation,(Byte)FuncBuf[pSATURATION]);
			break;
		case pTINT:
			if(FuncBuf[pVIDEOSOURCE] < AV1input)
				Write24C16(ep_sRGB_Tint,(Byte)FuncBuf[pTINT]);
			else
				Write24C16(ep_Video_Tint,(Byte)FuncBuf[pTINT]);
			break;
		case pSHARPNESS:
			if(FuncBuf[pVIDEOSOURCE] < AV1input)
				{
				addr = ep_Shape_Offset + (ModePoint * 8);
				Write24C16(addr,(Byte)FuncBuf[pSHARPNESS]);
				}
			else
				Write24C16(ep_Video_Shapness,(Byte)FuncBuf[pSHARPNESS]);
			break;
		case pBACKLIGHT:
			Write24C16(ep_BackLight,(Byte)FuncBuf[pBACKLIGHT]);
			break;
		
		case pAUTOPOWERON:
			Write24C16(ep_Signal_in_Play,FuncBuf[pAUTOPOWERON]);
			break;
		case pLANGUAGE:
			Write24C16(ep_Language,FuncBuf[pLANGUAGE]);
			break;
		case pCOLORTEMP:
			Write24C16(ep_Color_Ptr,FuncBuf[pCOLORTEMP]);
			break;
		case pTRANSOSD:
			Write24C16(ep_Osd_Effect,FuncBuf[pTRANSOSD]);
			break;
		case pBACKLIGHT2:
			Write24C16(ep_sRGB_Backlight,FuncBuf[pBACKLIGHT2]);
			break;			
		case pVCMVOL:
			Write24C16(ep_VCMVol,(Byte)FuncBuf[pVCMVOL]);
			break;
		case pVOLUME:
			Write24C16(ep_Volume,(Byte)FuncBuf[pVOLUME]);
			break;
		case pRADC:
			if(FuncBuf[pVIDEOSOURCE] == VGAinput)
				Write24C16(ep_ADC_R_Gain,FuncBuf[pRADC]);
			break;
		case pGADC:
			if(FuncBuf[pVIDEOSOURCE] == VGAinput)
				Write24C16(ep_ADC_G_Gain,FuncBuf[pGADC]);
			break;
		case pBADC:
			if(FuncBuf[pVIDEOSOURCE] == VGAinput)
				Write24C16(ep_ADC_B_Gain,FuncBuf[pBADC]);
			break;
		case pROFFSET:
			if(FuncBuf[pVIDEOSOURCE] <= YPbPrinput)
				Write24C16(ep_ADC_R_Offset,FuncBuf[pROFFSET]);
			//else if(FuncBuf[pVIDEOSOURCE] == YPbPrinput)
			//	Write24C16(ep_YPbPr_R_Offset,FuncBuf[pROFFSET]);
			break;
		case pGOFFSET:
			if(FuncBuf[pVIDEOSOURCE] <= YPbPrinput)
				Write24C16(ep_ADC_G_Offset,FuncBuf[pGOFFSET]);
			//else if(FuncBuf[pVIDEOSOURCE] == YPbPrinput)
			//	Write24C16(ep_YPbPr_G_Offset,FuncBuf[pGOFFSET]);
			break;
		case pBOFFSET:
			if(FuncBuf[pVIDEOSOURCE] <= YPbPrinput)
				Write24C16(ep_ADC_B_Offset,FuncBuf[pBOFFSET]);
			//else if(FuncBuf[pVIDEOSOURCE] == YPbPrinput)
			//	Write24C16(ep_YPbPr_B_Offset,FuncBuf[pBOFFSET]);
			break;
		case pContMax:
			Write24C16(ep_ContMax,FuncBuf[pContMax]);
			break;
		case pContMin:
			Write24C16(ep_ContMin,FuncBuf[pContMin]);
			break;
		case pBrigMax:
			Write24C16(ep_BrigMax,FuncBuf[pBrigMax]);
			break;
		case pBrigMin:
			Write24C16(ep_BrigMin,FuncBuf[pBrigMin]);
			break;
		case pBF3MODE:
			Write24C16(ep_BF3_MODE,FuncBuf[pBF3MODE]);
			break;
		case pLIGHTBRIGHT:
			Write24C16(ep_Light_Bright,FuncBuf[pLIGHTBRIGHT]);
			break;
		case pBFINTENSITY:
			Write24C16(ep_pBFINTENSITY,FuncBuf[pBFINTENSITY]);
			break;
		case pLIGHTCONTRAST:
			Write24C16(ep_Light_Contrast,FuncBuf[pLIGHTCONTRAST]);
			break;
		case pLIGHTHSTART:
			Write24C16(ep_Light_HSMSB,(Byte)(FuncBuf[pLIGHTHSTART] >> 8));
			Write24C16(ep_Light_HSLSB,(Byte)FuncBuf[pLIGHTHSTART]);
			break;
		case pLIGHTHWIDTH:
			Write24C16(ep_Light_HWMSB,(Byte)(FuncBuf[pLIGHTHWIDTH] >> 8));
			Write24C16(ep_Light_HWLSB,(Byte)FuncBuf[pLIGHTHWIDTH]);
			break;
		case pLIGHTVSTART:
			Write24C16(ep_Light_VSMSB,(Byte)(FuncBuf[pLIGHTVSTART] >> 8));
			Write24C16(ep_Light_VSLSB,(Byte)FuncBuf[pLIGHTVSTART]);
			break;
		case pLIGHTVHIGHT:
			Write24C16(ep_Light_VHMSB,(Byte)(FuncBuf[pLIGHTVHIGHT] >> 8));
			Write24C16(ep_Light_VHLSB,(Byte)FuncBuf[pLIGHTVHIGHT]);
			break;
		case pLIGHTSATURATION:
			Write24C16(ep_pLIGHTSATURATION,FuncBuf[pLIGHTSATURATION]);
			break;
		case pLIGHTHUE:
			Write24C16(ep_pLIGHTHUE,FuncBuf[pLIGHTHUE]);
			break;
		case pFTCORRECTION:
			Write24C16(ep_pFTCORRECTION,FuncBuf[pFTCORRECTION]);
			break;
		case pGREENENHANCE:
			Write24C16(ep_pGREENENHANCE,FuncBuf[pGREENENHANCE]);
			break;
		case pBLUEENHANCE:
			Write24C16(ep_pBLUEENHANCE,FuncBuf[pBLUEENHANCE]);
			break;
		case pACCMODE:
		case pRCMMODE:
			addr = FuncBuf[pRCMMODE];
			addr = FuncBuf[pACCMODE] |(addr<<4);
			Write24C16(ep_pACCMODE,addr);
			break;
		case pYPEAKING:
			//Write24C16(ep_pYPEAKING+1,(Byte)(FuncBuf[ep_pYPEAKING] >> 8));
			Write24C16(ep_pYPEAKING,FuncBuf[pYPEAKING]);
			break;
		case pCPEAKING:
			//Write24C16(ep_pCPEAKING+1,(Byte)(FuncBuf[ep_pCPEAKING] >> 8));
			Write24C16(ep_pCPEAKING,FuncBuf[pCPEAKING]);
			break;
		case pOSDRotate:
			Write24C16(ep_Osd_Rotate,FuncBuf[pOSDRotate]);
			break;
		case pDEFAULTCHANNEL:
			Write24C16(ep_Default_Channel,FuncBuf[pDEFAULTCHANNEL]);
			break;
		case pHotKey:
			Write24C16(ep_HotKey,FuncBuf[pHotKey]);
			break;
		}
	#if PRINT_MESSAGE	
		printf("SavePointer =0x%x\r\n",(unsigned short)SavePointer);
	#endif
	SavePointer = 0xff;
}

void MiscProcess(Bit DoAll)
{
	if((DDC_CTL0 & UPD_DDC) && !SaveDDC0)
		{
		SaveDDC0 = 1;
		DDCTimer = SAVE_DDC_TIME;
		}
	if((DDC_CTL1 & UPD_DDC) && !SaveDDC1)
		{
		SaveDDC1 = 1;
		DDCTimer = SAVE_DDC_TIME;
		}
	
	// kevin take off 2010 04 27if(SaveDDC0 && DDCTimer == 0)
	// kevin take off 2010 04 27	SaveDDCData(0);
	// kevin take off 2010 04 27if(SaveDDC1 && DDCTimer == 0)
	// kevin take off 2010 04 27	SaveDDCData(1);
	
	if(DoAll&&PowerStatus)//benson test15
	{						//for osd always on
	
		if((OsdTimer == 0 && OSD_ALWAYS_ON ==0) && OsdEnable && OutOfRange == 0 && FactMode == 0) {
			
			OSDChannelSelectMode = FALSE;
			Osd_Off();
			SetBrightFrame(0); //ray, set bright frame full	
			// kevin 20100715 for CCD  osd display error
			if(!CheckAvailableChannel(FuncBuf[pVIDEOSOURCE])
						&& !CCDSignal_IN )
			{
				GoToNextAvailableChannel();		
				ChannelSelect();
			}
		}
		//if(BurnInTimer == 0 && BurnInMode && NoSyncFlag){
		//	ShowBurnInPattern(0);
			//printf("Burn Test C.\n");
		//}
		if(SavePointer != 0xff && SaveTimer == 0)	
			SaveData();
		if((BackLightTimer == 0) ||( ((BackLightTimer / 1800) >= 1)&&((BackLightTimer % 1800) < 60)))
		{
			SaveBackLightTime();
		}
	}
}

void SaveBackLightTime(void)
{
	Write24C16(ep_BkLightTime_Offset+3,(Byte)(BackLightTimer >> 24));
	Write24C16(ep_BkLightTime_Offset+2,(Byte)(BackLightTimer >> 16));
	Write24C16(ep_BkLightTime_Offset+1,(Byte)(BackLightTimer >> 8));
	Write24C16(ep_BkLightTime_Offset,(Byte)(BackLightTimer));
}

void LoadPresetModeSettings(Byte type)
{
	Word addr;
	if(ModePoint < UserModeSt)
		addr = ModePoint * 8;
	else
		addr = UserModeRefReso * 8;

	switch(type)
	{
		case 0:
			FuncBuf[pPHASE] = EEP_SyncMap[addr+7];	
			FuncBuf[pCLOCK] = EEP_SyncMap[addr+4];
			FuncBuf[pCLOCK] <<= 8;
			FuncBuf[pCLOCK] |= EEP_SyncMap[addr+5];
		case 1:
			FuncBuf[pVPOSITION] = EEP_SyncMap[addr+1];
			FuncBuf[pHPOSITION] = EEP_SyncMap[addr+2];
			FuncBuf[pHPOSITION] <<= 8;
			FuncBuf[pHPOSITION] |= EEP_SyncMap[addr+3];
			break;
		case 2:
			FuncBuf[pCLOCK] = EEP_SyncMap[addr+4];
			FuncBuf[pCLOCK] <<= 8;
			FuncBuf[pCLOCK] |= EEP_SyncMap[addr+5];
			break;
		}

}
