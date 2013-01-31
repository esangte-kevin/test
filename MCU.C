#include "8051.H"
#include "STDIO.H"
#include "RAM.H"
#include "MCU.H"
#include "MyDef.H"
#include "IIC.H"
#include "F63XREG.H"
#include "F63XDEF.H"
#include "UserAdj.H"
#include "Scaler.H"
#include "rom_map.h"
#include "AutoAdj.H"
#include "OSD.H"
#include "OSD_TAB.H"
#include "MODEHANDLE.H"
//#include "dvKeypad.h"
#include "PANEL.H"
#include "TW9919.H"
#include "IR.H"
#include <string.h>
#include "sRGB.H"
extern WarmUp();

void InitMCU(void)
{
	Byte xdata *Addr;
	Byte i;
	Addr = 0xf000;
	for(i=0; i<0x23; i++){
		*Addr++ = McuRegTab[i];
	}
// set timer 0
 	SCON = 0x52;
 	TMOD = 0x22;	//timer0 /timer1 8 bits auto reload
// 	TMOD = 0x21;	//timer0 16 bits/timer1 8 bits auto reload
 	PCON = 0x80;
// timer0 8 bits auto reload
	TL0 = -200;
	TH0 =-200;
//	TL0 = 256-(CPU_CLK/1000*100/12000); //time base 100us
//	TH0 = 256-(CPU_CLK/1000*100/12000);

// timer0 16 bits
//	TL0 = (65536-CPU_CLK/1200)%256;	//time base 10ms
//	TH0 = (65536-CPU_CLK/1200)/256;

// For 12.000 MHz 
 	TL1 = 256-(2*CPU_CLK/384/9600);
 	TH1 = 256-(2*CPU_CLK/384/9600);
// For 14.318 MHz 
 	//TL1 = 256-(2*CPU_CLK/384/4800);
 	//TH1 = 256-(2*CPU_CLK/384/4800);
	TR0 = 1;		//start timer0
 	TR1 = 1;		//start timer1
// set interrupt
	EX1 = 1;		//enable External interrupt1
	ET0 = 1;		//enable timer0 interrupt
	EA = 1;			//enable all interrupt
	T0_INT_CNT = 0;
	T0_10ms_Cnt = 0;
	Sleep(50);	
}

void Timer0(void) interrupt 1 using 2
{
#if 1	//Used mode2--auto reload
	if(IR_Timer != 990)
			IR_Timer++;

	if(++T0_INT_CNT >= 10){
		T0_INT_CNT = 0;
		if(Timer_1ms != 0)			// 1ms
			Timer_1ms--;
		if(++T0_1ms_Cnt >= 10){	// 10ms
			CLRWDT = 0x55;
			T0_1ms_Cnt = 0;
			PowerKey_Timer--;
			if(++T0_10ms_Cnt >= 100){//--1s				
				if(Timer5>0)
					Timer5--;
				T0_10ms_Cnt = 0;				
				if(BackLightTimerEn){
					if(BackLightTimer < 360000000)
						BackLightTimer += 1;
					else
						BackLightTimer = 0;
				}
			}
			if(OsdTimer > 0)
				OsdTimer--;
			if(LocalTimer > 0)
				LocalTimer--;
			if(PowerTimer > 0)
				PowerTimer--;
			if(RepeatTimer > 0)
				RepeatTimer--;
			if(DDCTimer > 0)
				DDCTimer--;
			if(SaveTimer > 0)
				SaveTimer--;
			if(VideoTimer > 0)
				VideoTimer--;
			if(BurnInTimer > 0)
				BurnInTimer--;
			if(MuteTimer > 0)
				MuteTimer--;		
			if(IRSerchTime > 0)
				IRSerchTime--;
			if(Chankey_Timer>0)
				Chankey_Timer--;
			if(Timer3>0)
				Timer3--;
			if(Timer4>0)
				Timer4--;

		}
	}

#else	//Used mode1--16 bits counter
	TL0 = (65536-CPU_CLK/1200)%256; //time base 10ms
	TH0 = (65536-CPU_CLK/1200)/256;
	CLRWDT = 0x55;
	if(++T0_10ms_Cnt >= 100){
		T0_10ms_Cnt = 0;
		if(BackLightTimerEn){
			if(BackLightTimer < 360000000)
				BackLightTimer += 1;
			else
				BackLightTimer = 0;
		}
	}
	if(OsdTimer > 0)
		OsdTimer--;
	if(LocalTimer > 0)
		LocalTimer--;
	if(RepeatTimer > 0)
		RepeatTimer--;
	if(PowerTimer > 0)
		PowerTimer--;
	if(DDCTimer > 0)
		DDCTimer--;
	if(SaveTimer > 0)
		SaveTimer--;
	if(VideoTimer > 0)
		VideoTimer--;
	if(BurnInTimer > 0)
		BurnInTimer--;
	if(MuteTimer > 0)
		MuteTimer--;
#endif
	
}

void Sleep(Word T)
{
	Timer_1ms = T;			/* timeout n ms */
	while(Timer_1ms != 0){
		if ((PTB_REG & PB0)==0)  //test0716
			PowerTooLow=1;
	}
}

void WaitSetup(unsigned short time)
{
	LocalTimer = time;			/* timeout n 10ms */
	while(LocalTimer != 0){
		if(FuncBuf[pVIDEOSOURCE] <= DVIinput) //jacky20040324
			CheckModeChange();
	}
}

Byte GetKey()
{
 	Byte i,KeyTemp,KeyTemp2;
	i = 0;
	
#ifdef key_type_5// benson 980614
		if(!(PTC_REG & PC6) && !PowerStatus)// pwr pressed during power off
			{
				KeyTemp = vkPOWER;
				return KeyTemp;
			}
		else if (!(PTC_REG & PC6) && PowerStatus)// pwr pressed during power on
			{
				PowerKey_Timer = 150;// the term to confirm a pwr key pressed during power on														
				KeyTemp = vkPOWER;	
				do{
					if(PTC_REG & PC6)
						{
						KeyTemp = vkSOURCE;	
						return KeyTemp;
						}
			
				      }while(PowerKey_Timer != 0); 
				return KeyTemp;
			}
#else			 
	if(!(PTC_REG & PC6) )
		{
			KeyTemp = vkPOWER;
			return KeyTemp;
		}
#endif			
 	while(i < 10)
// 	while(i < 10)
		{
		KeyTemp = 0;
		if(!(PTD_REG & PD1))
			KeyTemp |= vkMENU;
		if(!(PTD_REG & PD0))
			KeyTemp |= vkAUTO;
		if(!(PTA_REG & PA1))
			KeyTemp |= vkVALUP;
		if(!(PTA_REG & PA2))
			KeyTemp |= vkVADOWN;
		if(KeyTemp != KeyTemp2)
			{
			KeyTemp2 = KeyTemp;
			i = 0;
			}
		i++;
 		}
	return KeyTemp;
}


void ScanKeyPad(void)	//2003/5/19 alvin
{
	Byte KeyTemp;

	KeyTemp = GetKey();
#ifdef Remote
		if(IR_Ready)
		{
			KeyTemp = KeyCodeMapping();
			IR_Ready = 0;
			//#if PRINT_MESSAGE
				//printf("0x%x\r\n",(unsigned short)KeyTemp);
				//printf("0x%x,%x\r\n",(unsigned short)IRCode>>16,(unsigned short)IRCode);
				//printf("IRKeyRepeat=0x%x\n",(unsigned short)IRKeyRepeat);
				//printf("IRKeyTemp=0x%x\n",(unsigned short)IRKeyTemp);
				//printf("%d\n",(unsigned short)Chankey_Timer);
			//#endif			
			if(KeyTemp == vkIR_POWER)
				KeyTemp = vkPOWER;
			else if(KeyTemp == vkIR_6)
				KeyTemp = vkVALUP;
			else if(KeyTemp == vkIR_5)
				KeyTemp = vkMENU;
			else if(KeyTemp == vkIR_4)
				KeyTemp = vkVADOWN;
			else if(KeyTemp == vkIR_VolUP)
				KeyTemp = vkSOURCE;
			else if(KeyTemp == vkIR_dimmer)
					;// KeyTemp = vkAUTO;
			else 
				KeyTemp = 0;
			Sleep(50);	
			//printf("KeyTemp=0x%x\r\n",(unsigned short)KeyTemp);
		}
#endif
#ifdef RS232Key
		KeyTemp = GetRS232KeyIn(KeyTemp);
#endif
		//printf("KeyTemp=0x%x\r\n",(unsigned short)KeyTemp);
	if(PrevKey!=KeyTemp)
	{
		KeyLock = 0;
 		ChangeKey = 1;	//change key
		PrevKey = KeyTemp;
		KeyBuffer = KeyTemp;
		KeyRepeatCnt = 0;
		OsdTimer = FuncBuf[pOSDTIMER] * 100;
		RepeatTimer = KeyHoldTime;	// 1 Sec
	}
	else
	{
		KeyBuffer = 0;	//the other key
		if((RepeatTimer == 0))//&& MenuPtr.Menu > 0)
			{
			switch(KeyTemp)
				{
				case vkVADOWN:
				case vkVALUP:
					if(!KeyLock)
						KeyBuffer = KeyTemp;
					RepeatTimer = KeyRepeatTime;
					OsdTimer = FuncBuf[pOSDTIMER] * 100;
					IRKeyRepeat=0;
					break;
				};
			}
		}
}

#if 0
void KeyProcess(void)
{
	//Byte code MaxItemsTab[]={7,7,4,9,6,5,6,MAX_FACT_ITEM};  //define each menu's max item
	// 0:osd main,1:setup page,2:option page,3:null,4:null,5:null,6:null,7:null
	//Byte code MaxItemsTab[]={12,10,6,9,6,8,6,MAX_FACT_ITEM};  //define each menu's max item	
#ifdef CTA	
	// kevin 20100318 for color temp user defined
	Byte code MaxItemsTab[]={12,9,9,9,6,8,6,MAX_FACT_ITEM};  //define each menu's max item
	Byte temp,pre_channel;	
#else
	Byte code MaxItemsTab[]={12,9,6,9,6,8,6,MAX_FACT_ITEM};  //define each menu's max item
	Byte temp,tempFuncPtr,tempPtr,pre_channel;
#endif	
	
	
	if (KeyBuffer != Menu_Power_key)
	ScanKeyPad();

	if(OsdLock && KeyBuffer != (vkMENU|vkLEFT|vkVALUP))		//osd lock
		KeyBuffer = 0;

	
	
	if(OutOfRange != 0 && KeyBuffer != vkPOWER && KeyBuffer != vkSOURCE)
		KeyBuffer = 0;

	temp = MenuPtr.Page;
	if(MenuPtr.Menu == 2)
		temp = 7;

	MaxItem = MaxItemsTab[temp];	//load each page max item value
	
	if(FuncBuf[pVIDEOSOURCE]==DVIinput&& temp == 0)//for DVI MenuPtr.Page = 0
		MaxItem = 7;
	else if(FuncBuf[pVIDEOSOURCE]>DVIinput && temp == 0)//for CAVS and S-Video MenuPtr.Page = 0
		MaxItem = 11;
	if(!Tw9919En && temp == 5)
		MaxItem = 2;
	
	if(MenuPtr.Level == 1)	//for level 1 menu
		BackPtr = MenuPtr.Ptr;
	
	else if(MenuPtr.Level == 2)
		BackPtr = 0;

	if(MenuPtr.Menu == 0)	//if no OSD
	{
		switch(KeyBuffer)	
		{
			case vkSOURCE:
				if(CCDSignal_IN){
					KeyLock = 1;
					return;
					}
				if(!FactMode ){
					pre_channel=FuncBuf[pVIDEOSOURCE];
					
					do{
					FuncBuf[pVIDEOSOURCE]++;
					
					#ifdef decoder_TECHWELL
						if(FuncBuf[pVIDEOSOURCE]>Svideoinput)
					#else
						if(FuncBuf[pVIDEOSOURCE]>DVIinput)
					#endif
							if (vga0_YPbPr1)
								FuncBuf[pVIDEOSOURCE] = YPbPrinput;
							else
							FuncBuf[pVIDEOSOURCE] = VGAinput;
							
					}while (!CheckAvailableChannel(FuncBuf[pVIDEOSOURCE]));
						
						if (FuncBuf[pVIDEOSOURCE] == pre_channel)
						{
							KeyLock = 1;
							return;
						}
						ChannelSelect();
				}
				KeyLock = 1;
				return;
				break;

			case vkAUTOTEST:
			case vkIR_dimmer:
			case vkAUTO:
				if(FuncBuf[pVIDEOSOURCE] == VGAinput&& hotkey && MenuPtr.Menu == 0 && KeyBuffer != vkIR_dimmer && SyncMode != 2)
				{
					KeyLock = 1;
					ShowMessage(MSG_AUTOTUNE);
					AutoTune();//AutoTest();
				}else if(!hotkey || KeyBuffer == vkIR_dimmer)
					AdjustDimmer();	
					
				// kevin 20090715 ===========================
				if(FactMode)
				{
					FactMode = 0;
					Write24C16(ep_Status,StatusFlag);
				}			
				KeyLock = 1;
				return;
				break;

			case vkMENU:		//OPEN MAIN MENU

				if(!FactMode){
					MenuPtr.Menu = 1;
					if(FuncBuf[pVIDEOSOURCE] == YPbPrinput)
						MenuPtr.Ptr = 1;
					else
						MenuPtr.Ptr = 0;
					}
				else{				//MenuPtr.Menu = 2, means factory menu
					MenuPtr.Menu = 2;
					MenuPtr.Ptr = 3;
					FuncBuf[pCOLORTEMP] = 2;
					LoadColor(FuncBuf[pCOLORTEMP]);
					SetContrast();
				}
				MenuPtr.Page = 0;
				MenuPtr.Level = 0;
				MenuPtr.Op = oUPDATEALL;
				KeyLock = 1;
				
				return;
				break;
			case vkVALUP:		// MenuPtr.Menu = 3, means brightness or volume control menu
				
				return;
				break;
			case vkVADOWN:
				
				return;
				break;
			case vkPOWER:
			case Menu_Power_key:
			KeyLock = 1;
			if(!PowerStatus)
				{
				FastPowerOn();
				Pwroff_menu_flag =0;
				KeyBuffer=0;
				}
			else
				{
				PowerOffProcess();
				Pwroff_menu_flag =1;
				KeyBuffer=0;
				}
			return;
			break;
			
	
		};
		
	}
//osd opened
//else //osd open
	//{
	switch(KeyBuffer)
		{
		case vkMENU:

			if(MenuPtr.Menu == MenuType_General) 
			{
				if(MenuPtr.Level == 0)	//first time entering page, mapping cursor place in 3 special pages
				{
					MenuPtr.Level= 1;
					MenuPtr.Op = oENTER;
					MenuPtr.Ptr = 0;
				}
				else if(MenuPtr.Level == 1)
				{
					MenuPtr.Ptr++;	
					MenuPtr.Op = oINCFUNC;
				}
				else if(MenuPtr.Level == 2)
				{
					MenuPtr.Ptr++;
					if(MenuPtr.Ptr >= 7)
						MenuPtr.Ptr = 4;
					MenuPtr.Op = oINCFUNC;
				}
			}
			KeyLock = 1;
			break;
		case vkVALUP:
			if(MenuPtr.Menu  == MenuType_General)
			{
					if(FuncPtr == pImageSetup && MenuPtr.Page == Menu_Main)
					{
							MenuPtr.Page = Menu_ImageSetup;	// enter image setup
							MenuPtr.Ptr = 0;
							MenuPtr.Level = 0;
							MenuPtr.Op = oENTER;
							//ClearTextArea();
							
					}else if(FuncPtr == pSourceSelect && MenuPtr.Page == Menu_Main)
					{
							MenuPtr.Page = Menu_SourceSelect;	// enter Source Select	
							MenuPtr.Ptr = 0; 
							MenuPtr.Level = 0;
							MenuPtr.Op = oENTER;
							//ClearTextArea();
							
					}else if(FuncPtr == pOSDSetup&& MenuPtr.Page == Menu_Main)
					{
							MenuPtr.Page = Menu_OSDSetup;	// enter OSD Setup
							MenuPtr.Ptr = 0; 
							MenuPtr.Level = 0;
							MenuPtr.Op = oENTER;
							//ClearTextArea();
							
					}else
					{
							MenuPtr.Op = oINCVAL;
					}
			}
			break;
		case vkVADOWN:
			if(MenuPtr.Menu  == MenuType_General)
			{
					if(FuncPtr == pImageSetup && MenuPtr.Page == Menu_Main)
					{
						MenuPtr.Page = Menu_ImageSetup;	// enter Image setup
						MenuPtr.Ptr = 0;
						MenuPtr.Level = 0;
						MenuPtr.Op = oENTER;
						
					}else if(FuncPtr == pSourceSelect && MenuPtr.Page == Menu_Main)
					{
						MenuPtr.Page = Menu_SourceSelect;	// enter Source Select	
						MenuPtr.Ptr = 0; 
						MenuPtr.Level = 0;
						MenuPtr.Op = oENTER;
						
					}else if(FuncPtr == pOSDSetup&& MenuPtr.Page == Menu_Main)
					{
						MenuPtr.Page = Menu_OSDSetup;	// enter OSD Setup	
						MenuPtr.Ptr = 0; 
						MenuPtr.Level = 0;
						MenuPtr.Op = oENTER;
						
					}else
					{
							MenuPtr.Op = oDECVAL;
					
					}
			}
			break;
		case vkIR_dimmer:
		case vkAUTO:		
			Osd_Off();
			break;
		case vkSOURCE:
			if(CCDSignal_IN){
				KeyLock = 1;
				return;
				}
			if(MenuPtr.Menu == 4)
			{
					MenuPtr.Level = 1;
					MenuPtr.Op = oINCVAL;
				OsdTimer = 3 * 100;
			}else{
					pre_channel=FuncBuf[pVIDEOSOURCE];
					do{
						FuncBuf[pVIDEOSOURCE]++;
										
					#if decoder_TECHWELL
						if(FuncBuf[pVIDEOSOURCE]>Svideoinput)
					#else
						if(FuncBuf[pVIDEOSOURCE]>DVIinput)
					#endif
						if (vga0_YPbPr1)
								FuncBuf[pVIDEOSOURCE] = YPbPrinput;
						else
								FuncBuf[pVIDEOSOURCE] = VGAinput;
												
					}while (!CheckAvailableChannel(FuncBuf[pVIDEOSOURCE]));
											
					if (FuncBuf[pVIDEOSOURCE] == pre_channel){
							KeyLock = 1;
							return;
					}
				
					ChannelSelect();
			}
			KeyLock = 1;
			break;
		case vkPOWER:
		case Menu_Power_key:
			KeyLock = 1;
			if(!PowerStatus)
				{
				FastPowerOn();
				Pwroff_menu_flag =0;
				KeyBuffer=0;
				}
			else
				{
				PowerOffProcess();
				Pwroff_menu_flag =1;
				KeyBuffer=0;
				}			
			break;


 		};
}
#else
void KeyProcess(void)
{
	//define each menu's max item	
	Byte code MaxItemsTab[]={	
							MaxItem_MainMenu,
							MaxItem_ImageSelect,
							MaxItem_SourceSelect,
							MaxItem_OSDSetup,
							MaxItem_ChannelSelect};  						
	//Byte temp,tempFuncPtr,tempPtr,pre_channel;

	if (KeyBuffer != Menu_Power_key)
		ScanKeyPad();

	if(OsdLock && KeyBuffer != (vkMENU|vkLEFT|vkVALUP))		//osd lock
		KeyBuffer = 0;
	
	if(OutOfRange != 0 && KeyBuffer != vkPOWER && KeyBuffer != vkSOURCE)
		KeyBuffer = 0;

	MaxItem = MaxItemsTab[MenuPtr.Page];	//load each page max item value


	/*
	if(KeyBuffer != PrevKey)
	{
		printf("menu=%d \n",(unsigned short)MenuPtr.Menu);
		printf("Ptr=%d \n",(unsigned short)MenuPtr.Ptr);
		printf("Page=%d \n",(unsigned short)MenuPtr.Page);
		printf("Level=%d \n",(unsigned short)MenuPtr.Level);
		printf("Op=%d \n",(unsigned short)MenuPtr.Op);
	}
	*/

	//if no OSD =========================================
	if(MenuPtr.Menu == MenuType_None ||MenuPtr.Menu == MenuType_HotKey)	
	{
		switch(KeyBuffer)	
		{
			case vkAUTOTEST:
			case vkIR_dimmer:
			case vkAUTO:
				if(FuncBuf[pVIDEOSOURCE]==VGAinput)
				{
					ShowMessage(MSG_AUTOTUNE);
					AutoTune();
				}
				break;

			case vkMENU:	
				//OPEN MAIN MENU
				MenuPtr.Menu = MenuType_General;
				MenuPtr.Ptr = 0;
				if(NoSyncFlag)
				{
					MenuPtr.Page = Menu_ChannelSelect;
				}else
					MenuPtr.Page = Menu_Main;
				MenuPtr.Level = Level_MainItem;
				MenuPtr.Op = oUPDATEALL;
				KeyLock = 1; 				
				break;

			case vkUPANDDOWN:
				// if key = up+down
				InputMode = InputMode_1;
				break;
			case vkVALUP:		
			case vkVADOWN:
				
					if(FuncBuf[pHotKey] == HotKey_Contrast)
					{
						//OPEN Hot key MENU
						MenuPtr.Menu = MenuType_HotKey;
						MenuPtr.Ptr = 0;				
						MenuPtr.Level = Level_MainItem;
						MenuPtr.Page = HotKeyMenu_Contrast;

						if(KeyBuffer == vkVALUP)
							MenuPtr.Op = oINCVAL;
						else
							MenuPtr.Op = oDECVAL;
					
					}else if(FuncBuf[pHotKey] == HotKey_ChannelSelect)
					{
						//OPEN Hot key MENU
						MenuPtr.Menu = MenuType_HotKey;									
						MenuPtr.Level = Level_MainItem;
						MenuPtr.Page = HotKeyMenu_ChannelSelect;
						KeyLock = 1;

						do
						{
							if(KeyBuffer == vkVALUP)
							{
								MenuPtr.Op = oINCVAL;
								if(++FuncBuf[pVIDEOSOURCE]>Svideoinput)
									FuncBuf[pVIDEOSOURCE] =VGAinput;
							}else{
								
								MenuPtr.Op = oDECVAL;		
								if(--FuncBuf[pVIDEOSOURCE] > Svideoinput)
									FuncBuf[pVIDEOSOURCE] =Svideoinput;
							}			
						}while (!CheckAvailableChannel(FuncBuf[pVIDEOSOURCE]));

						MenuPtr.Ptr = FuncBuf[pVIDEOSOURCE];
					}														
				break;
				
			case vkPOWER:
			case Menu_Power_key:
				KeyLock = 1;
				if(!PowerStatus)
				{
					FastPowerOn();
					Pwroff_menu_flag =0;
					KeyBuffer=0;
				}
				else
				{
					PowerOffProcess();
					Pwroff_menu_flag =1;
					KeyBuffer=0;
				}
			break;
		};
		
	}else{
		// osd is opened
		switch(KeyBuffer)	
		{
			case vkAUTOTEST:
			case vkIR_dimmer:
			case vkAUTO:
				// goto level down
				MenuPtr.Op = oENTER;	
				break;

			case vkMENU:	
				//if(NoSyncFlag && MenuPtr.Page == Menu_ChannelSelect)
				//{
				//	OSDChannelSelectMode=FALSE;
				//	Osd_Off();		
				//	ShowMessage(MSG_NOSYNC);
				//}else{
					// going to one level up
					MenuPtr.Op = oRETURN;	
					
					if(!CheckAvailableChannel(FuncBuf[pVIDEOSOURCE]))
					{
						GoToNextAvailableChannel(); 	
						ChannelSelect();
					}
				//}
				break;
				
			case vkVALUP:		
			case vkVADOWN:
				if(MenuPtr.Level==Level_MainItem)
				{	
				//Level = MainItem ===========================
					if(KeyBuffer == vkVALUP)
					{
						// goto next row	
						MenuPtr.Op = oINCFUNC;						
						if(++MenuPtr.Ptr >= MaxItem)
							MenuPtr.Ptr = 0;				
					}else{
						// goto prev row	
						MenuPtr.Op = oDECFUNC;						
						if(--MenuPtr.Ptr >= MaxItem)
							MenuPtr.Ptr = MaxItem-1;
					}					
				}else if(MenuPtr.Level==Level_SubItem){
				//Level = SubItem ============================

					if(KeyBuffer == vkVALUP)
					{
						// goto 	
						MenuPtr.Op = oINCVAL;						
					}else{
						// goto prev row	
						MenuPtr.Op = oDECVAL;						
					}
				}
				
				break;
				
			case vkPOWER:
			case Menu_Power_key:
				KeyLock = 1;
				if(!PowerStatus)
				{
					FastPowerOn();
					Pwroff_menu_flag =0;
					KeyBuffer=0;
				}
				else
				{
					PowerOffProcess();
					Pwroff_menu_flag =1;
					KeyBuffer=0;
				}
			break;
		};
	}
}

#endif
/*
void EEPROM_WProtect_Enable(void)
{
unsigned char port;
	port = PTC_REG;
	port |= RDPC_REG;
	port |= PC2;
	PTC_REG = port;
}
void EEPROM_WProtect_Disable(void)
{
unsigned char port;
	port = PTC_REG;
	port |= RDPC_REG;
	port &= ~PC2;
	PTC_REG = port;
}
*/
void LED_RedOn(void)
{
unsigned char port;
	port = PTA_REG;
	port |= RDPA_REG;
	port &= ~PA0;
	PTA_REG = port;
}

void LED_RedOff(void)
{
unsigned char port;
	port = PTA_REG;
	port |= RDPA_REG;
	port |= PA0;
	PTA_REG = port;
}

void BackLightOn(void)
{
	unsigned char port;
		port = PTC_REG;
		port |= RDPC_REG;
#if BacklightActive	 == 1	
		port |= PC0;
#else
		port &= ~PC0;							
#endif							// turn on BL_on1
		port |= PC7;				// turn on BL_on2 kevin 07242008
		PTC_REG = port;
		BackLightTimerEn = 1;
		WriteWordIIC563(PWMA_Lo_Add,PWMON); // turn on BL_on2
}
	
void BackLightOff(void)
{
		unsigned char port/*,temp*/;
		
		port = PTC_REG; 		
		port |= RDPC_REG;
#if BacklightActive	 == 1
		port &= ~PC0;	
#else
		port |= PC0;			
#endif
		port &= ~PC7;// turn off BL_on2 kevin 07242008
		PTC_REG = port; 				  // turn off BL_on1
		BackLightTimerEn = 0;
//test0716		WriteWordIIC563(PWMA_Lo_Add,PWMOff); // turn off BL_on2
}

bit DetectBacklight()
{
	if(PTC_REG & PC0)
		return 1;
	else
		return 0;
}

//#if PanelPowerActive == High
/***************************************************************************************
* Function name : PanelPowerOn(void)
* writer		: kevin lin
* Date			: 05/22/2007
* Description	: panel power on
****************************************************************************************/
void PanelPowerOn(void)//ben980614
{
	unsigned char port,temp;
		port = PTC_REG;
		port |= RDPC_REG;
		port |= PC3;
		PTC_REG = port;
		Sleep(PowerUpPanelTiming);
		//kevin 0708 temp = ReadIIC563(0x16e);
		//kevin 0708 WriteIIC563(0x16e,temp|0x81);	// output signal enable
		temp = ReadIIC563(0x150);
		WriteIIC563(0x150,temp|0x01);	// output control and signal enable
		Sleep(PowerDownInvTiming);
}

/***************************************************************************************
* Function name : PanelPowerOff(void)
* writer		: kevin lin
* Date			: 05/22/2007
* Description	: panel power off
****************************************************************************************/
void PanelPowerOff(void)//ben980614
{
	unsigned char port,temp;
	//kevin 0708 temp = ReadIIC563(0x16e);
	//kevin 0708 WriteIIC563(0x16e,temp&0x7e);	// output signal disable
	temp = ReadIIC563(0x150);
	WriteIIC563(0x150,temp&0xfe);	// output control and signal disable
	Sleep(PowerDownPanelTiming);
		port = PTC_REG;
		port |= RDPC_REG;
		port &= ~PC3;
		PTC_REG = port;
		Sleep(300);
}
//#else
/***************************************************************************************
* Function name : PanelPowerOff(void)
* writer		: kevin lin
* Date			: 05/22/2007
* Description	: panel power off
****************************************************************************************/
/*void PanelPowerOff(void)//ben980614
{
	unsigned char port,temp;
	temp = ReadIIC563(0x16e);
	WriteIIC563(0x16e,temp&0x7e);	// output signal desable
	Sleep(PowerDownPanelTiming);
	port = PTC_REG;
	port |= RDPC_REG;
	port |= PC3;
	PTC_REG = port;
}*/
/***************************************************************************************
* Function name : PanelPowerOn(void)
* writer		: kevin lin
* Date			: 05/22/2007
* Description	: panel power on
****************************************************************************************/
/*void PanelPowerOn(void)//ben980614
{
unsigned char port,temp;
	port = PTC_REG;
	port |= RDPC_REG;
	port &= ~PC3;
	PTC_REG = port;
	Sleep(PowerUpPanelTiming);
	temp = ReadIIC563(0x16e);
	WriteIIC563(0x16e,temp&0x7e);	// output signal desable
	Sleep(PowerDownInvTiming);
}
#endif
*/
#if PANEL == CMO_M170ES05
void DC2DCPowerOn(void)
{
	unsigned char port;
	
	port = PTE_REG;
	port |= RDPE_REG;
	port &= ~PE0;
	PTE_REG = port;
}

void DC2DCPowerOff(void)
{
	unsigned char port;
	port = PTE_REG;
	port |= RDPE_REG;
	port |= PE0;
	PTE_REG = port;
}

void XAO_On(void)
{
	//Byte Port;
	//Port = PTC_REG;		
	//Port |= RDPC_REG;
	//Port |= PC3;
	//PTC_REG = Port;
}

void XAO_Off(void)
{
	//Byte Port;
	//Port = PTC_REG;		
	//Port |= RDPC_REG;
	//Port &= ~PC3;
	//PTC_REG = Port;
}
#endif
/*
void ResetOff(void)
{
unsigned char port;
	port = PTC_REG;
	port |= RDPC_REG;
	port |= PC4;
	PTC_REG = port;
}
*/

void ResetOn(void)
{
	unsigned char port;
	port = PTC_REG;
	port |= RDPC_REG;
	port &= ~PC4;
	PTC_REG = port;
	
	
	Sleep(12);
	port |= RDPC_REG;
	port |= PC4;
	PTC_REG = port;
	Sleep(50);
	WriteIIC563(0x101, 0x20);  
	Sleep(12);
	WriteIIC563(0x101, 0x0f);
	Sleep(12);
}
/*
void FastMuteOn()
{
	Byte Port;
	WaitVblank();
	Port = PTD_REG;		
	Port |= RDPD_REG;
	Port |= PD4;
	PTD_REG = Port;
}

void FastMuteOff()
{
	Byte Port;
	WaitVblank();
	Port = PTD_REG;		
	Port |= RDPD_REG;
	Port &= ~PD4;
	PTD_REG = Port;
}
*/
/*
void SAA7114Off()
{
	Byte Port;
	Port = PTD_REG;		
	Port |= RDPD_REG;
	Port &= ~PD1;
	PTD_REG = Port;
}

void SAA7114On()
{
	Byte Port;
	Port = PTD_REG;		
	Port |= RDPD_REG;
	Port |= PD1;
	PTD_REG = Port;
}
*/
void AUDIO_MUTE(void)
{

	Byte Port;
	Port = PTA_REG;		
	Port |= RDPA_REG;
	Port |= PA3;
	PTA_REG = Port;

}

void AUDIO_On(void)
{

	Byte Port;
	Port = PTA_REG;		
	Port |= RDPA_REG;
	Port &= ~PA3;
	PTA_REG = Port;

}
/*
bit TW9919Status()
{

	if(PTC_REG & PC1)
		return 0;
	else
		return 1;
}
*/

bit DetectDVI()
{
/*
	if(PTD_REG & PD3)
		return 0;
	else
		return 1;
*/
		return 1;
}

/*
bit DetectDsub()
{
	if(PTD_REG & PD5)
		return 0;
	else
		return 1;
}
*/

bit DetectIRQ()
{
	if(PTB_REG & PB2)
		return 0;
	else
		return 1;
}
/*
bit DetectPower()
{
	if(PTB_REG & PB3)
		return 1;
	else
		return 0;
}
*/

bit VsyncValide()
{
	Word VsyncWidth;
	LocalTimer = 2;	//20ms
	while(!(PTD_REG & BIT_6))	//Vsync Lo
		{
		if(LocalTimer == 0)
			return 0;
		}
	LocalTimer = 10;	//100ms
	while(PTD_REG & BIT_6)	//Vsync Hi
		{
		if(LocalTimer == 0)
			return 0;
		}
	VsyncWidth = 0;
	while(!(PTD_REG & BIT_6))	//Vsync Hi 1:7us
		{
		VsyncWidth ++;
		if(VsyncWidth > 50)		//over 350us
			return 0;
		}
	VsyncWidth = 0;
	while(PTD_REG & BIT_6)		//Vsync Hi 1:7us
		{
		VsyncWidth ++;
		if(VsyncWidth > 714)	//over 5ms
			return 1;
		}
	return 0;
}

void WaitVblank()
{
	LocalTimer = 3;	//30ms
	while(!(PTD_REG & BIT_6))	//Vsync Lo
		{
		if(LocalTimer == 0)
			return;
		}
	LocalTimer = 5;	//50ms
	while(PTD_REG & BIT_6)	//Vsync Hi
		{
		if(LocalTimer == 0)
			return;
		}
}
void IntGroup1(void) interrupt 2 using 1
{
	//Byte Port;
	if(INT_SRC & INT_EXT)
	{
		INTEXT_EN = 0x00;		// Disable interrupt...
		ET0 = 0;
		#ifdef Remote
			ReadIRCode();
		#endif
		if(INTEXT_FLG & INTE0)
			{
			if(FastMuteEnable)
				{
				//Port = PTC_REG;		//backlight off
				//Port |= RDPC_REG;
				//Port &= ~PC0;
				//PTC_REG = Port;
				//Port = PTD_REG;		//fastmute on
				//Port |= RDPD_REG;
				//Port &= ~PD4;
				//PTD_REG = Port;
				}
			Abort = 1;	//abort autotune
			//if(ChangeMode<10)
			//	ChangeMode++;
			INTEXT_FLG = INTE0;
			}
		IR_Timer = 0;	
		TR0 = 1;
		ET0 = 1;
		INTEXT_FLG = (INTE0+INTE1);
		INTEXT_EN = (INTE0+INTE1);
	}
}

void SetInverter(void)
{
	Byte value;
//	if(FuncBuf[pCOLORTEMP] < 4)
#if PANEL == CMO_M170E4_L01 || PANEL == LP_LM170E01
		value = FuncBuf[pBACKLIGHT];
#else
		value = 0x7f - FuncBuf[pBACKLIGHT];
#endif
//	else
//		value = FuncMax - FuncBuf[pBACKLIGHT2];
	
#if PANEL == Samsung_LTM150XH_L04 || PANEL == CMO_M190A1_L01
	PWM9_REG = 0xC0;
#else
	PWM9_REG = value * 2;
#endif

	#if PRINT_MESSAGE
		printf("FuncBuf[pBACKLIGHT] = %x\r\n",(unsigned short)value);
	#endif
}

#define AUDIO_PWM_MAX 208
void SetAudioVolume(void)
{
unsigned char value;
	//value = (((FuncBuf[pVOLUME] * AUDIO_PWM_MAX) + 50) / 100);
	//value = FuncBuf[pVOLUME] + 104;
	value = FuncBuf[pVOLUME] + 140;
	//value = FuncBuf[pVOLUME];
	PWM6_REG = value;
	#if PRINT_MESSAGE
		printf("FuncBuf[pVOLUME] = %x\r\n",(unsigned short)value);
	#endif
}
/*
void SetVCM_Vol(void)
{
unsigned char value;
	value = FuncBuf[pVCMVOL];
	PWM7_REG = value;
	#if PRINT_MESSAGE
		printf("FuncBuf[pVCMVOL] = %x\r\n",(unsigned short)value);
	#endif
}
*/
void SetAudioMute(void)
{
	if(AudioMute)
		AUDIO_MUTE();
	else
		AUDIO_On();
	#if PRINT_MESSAGE
		printf("AudioMute = %x\r\n",(unsigned short)AudioMute);
	#endif
}

void PowerOffProcess()
{
	LED_RedOff();
	Osd_Off();
	BackLightOff();
	PowerStatus = 0;	//power off
	PowerDown = 1;
	Write24C16(ep_Sync_Source,FuncBuf[pVIDEOSOURCE]);
	Write24C16(ep_Status,StatusFlag);
	TW9919_PowerDown();
	ScalerPowerDown();
	
	WriteIIC563(0x0f0,0x00);//disable panel interface signals
}


void CheckAnyKey()
{
	if(Abort)
		return;
	if(GetKey() == vkPOWER)
		Abort = 1;
}
/*
void AutoTest(void)
{
	xdata unsigned long tempw;
		
	if(TestPoint>2)
		TestPoint = 0;
	
	switch(TestPoint){
	case 0:
		//ForceSource = 1;
		if(FuncBuf[pVIDEOSOURCE] != 0){
			FuncBuf[pVIDEOSOURCE] = 0;
			//Write24C16(ep_Sync_Source,FuncBuf[pVIDEOSOURCE]);
			Osd_Off();
			FastMuteEnable = 0;
			BackLightOff();
			SetSource();
			PowerTimer=POWER_TIME;	//2004/2/27 test
			MessageShow = 0;	//show msg if Video sync off
			TestPoint = 0xff;
		}
		else{
			//Sleep(1000);
			//ShowMessage(MSG_AUTOCOLOR);
			//AutoColor();
			tempw = FuncBuf[pCLOCK];
			//--------Check ADCPLL and HPLL
			GetFuncRange(pCLOCK);
			for(FuncBuf[pCLOCK]=FuncMin; FuncBuf[pCLOCK]<FuncMax; FuncBuf[pCLOCK]++){
				SetADC_PLL();
			}
			FuncBuf[pCLOCK] = tempw;
			SetADC_PLL();
			ShowMessage(MSG_AUTOTUNE);
			AutoTune();
			ShowSource();
			OsdEnable = 0;
		}
		break;
	case 1:
		if(FuncBuf[pVIDEOSOURCE] != 1){
			FuncBuf[pVIDEOSOURCE] = 1;
			//Write24C16(ep_Sync_Source,FuncBuf[pVIDEOSOURCE]);
			Osd_Off();
			FastMuteEnable = 0;
			BackLightOff();
			SetSource();
			//Sleep(1000);
			PowerTimer=POWER_TIME;	//2004/2/27 test
			MessageShow = 0;	//show msg if Video sync off
			ShowSource();
		}
		else
			ShowSource();
		
		OsdEnable = 0;
		//FLG_AutoTest = 1;		 
		break;
		
	case 2:
		
		Osd_Off();
		WriteIIC563(0x1E5,0x47);
		OsdInit();
		if(ReadIIC563(0x1FC)&0x07) {
			ShowMain(1);
			Osd_On(enWIN4);	//win4 background /win3 select icon 
			WriteWordIIC563(0x0cc,SOURCE_COLOR);
			WriteSeqIndexPort(ROW9+6,tySRAM_CODE_CC_CD,"   Bist Error   ",14);
		}
		else {
		
			ShowMain(1);
			Osd_On(enWIN4);	//win4 background /win3 select icon 
			WriteWordIIC563(0x0cc,SOURCE_COLOR);
			if(FuncBuf[pVIDEOSOURCE] ==0)
				WriteSeqIndexPort(ROW9+6,tySRAM_CODE_CC_CD,"   DVI Error    ",14);
			else
				WriteSeqIndexPort(ROW9+6,tySRAM_CODE_CC_CD,"    PASS!!!     ",14);
		}
		
		Sleep(1000); // Timer * 500 ms
		Sleep(FuncBuf[pOSDTIMER]*50); // Timer * 500 ms
		//ForceSource = 0;
			
		break;
	}
	TestPoint++;
}
*/
/***************************************************************************************
* Function name : GetFBA1665D0Flag(void)
* writer		: kevin lin
* Date			: 05/31/2007
* Description	: return FBA1665D0 Flag status
****************************************************************************************/
/*void AdjustDimmer()
{
	dimmerValue++;
	if(dimmerValue == 3)
			dimmerValue = 0;
	switch(dimmerValue)
	{
		case 0x00:
			FuncBuf[pBRIGHTNESS] = 30;
		break;
		case 0x01:
			FuncBuf[pBRIGHTNESS] = 50;
			break;
		case 0x02:
			FuncBuf[pBRIGHTNESS] = 70;
			break;
    }
	//if(MenuPtr.Menu == 0)
	//	ShowOSDDimmer();
	if(FuncBuf[pVIDEOSOURCE] <= DVIinput)
	{
		SetBrightness();
		Write24C16(ep_Brightness,FuncBuf[pBRIGHTNESS]);
	}else{
		SetVideoBrightness();
		Write24C16(ep_Video_Brightness,FuncBuf[pBRIGHTNESS]);
	}
}
*/
void GetFBA1665D0Flag(void)
{
	Byte Port;
	FBA1665D0Type = 0;
	Port = PTA_REG;		
	Port = ~Port;
	Port &= 0xe0;
	// pin 7 : audio flag
	// pin 6 : light sensor flag
	// pin 5 : inverter flag
	FBA1665D0Type = Port;
	#if PRINT_MESSAGE	
		printf("FBA1665D0 flag = 0x%x\n",(unsigned short)FBA1665D0Type);			
	#endif
}
void FastPowerOn()
{
	PowerStatus = 1;	//power off
	PowerDown= 0;	//power off
	LED_RedOn();
	Osd_Off();
	ScalerPowerUp();
	
	InitScaler();//benson980622
	SetDPLL((unsigned long)FreeRunH_Freq*PanelMinHTotal);  	// for freerun dotclock kevin 20090701
	ChannelSelect();
	PowerTimer = POWER_TIME;
	channechange=1;//0709
	VideoMute = 0;
	ForceToBack= 0;
	Pwroff_menu_flag =0;
//	CCDSignal_IN=0;
	PanelPowerOn();//benson

	Write24C16(ep_Status,StatusFlag);//benson980614
}
#ifdef RS232Key
void PrintOSDOnScreen(unsigned char mode)
{
	unsigned short i;
	//FuncBuf[pLANGUAGE]= 0;
	//OsdInit();
	ClearOSD();
	WriteWordIIC563(0x081,0);	// osd H start
	WriteIIC563(0x083,0x3f);	// osd width
	WriteWordIIC563(0x084,0);	// osd v start	
	WriteIIC563(0x086,3);		// osd Height
	WriteIIC563(0x0a7,0x3f);	// osd win4 width
	WriteIIC563(0x0a9,3);		// osd win4 Height
	Osd_On(enWIN4);
	switch (mode)
	{
		case 0:
			for(i=0;i<255;i++)
				WriteWordIndexPort(i,tySRAM_CODE_CC_CD,i);
		break;
		case 1:
			WriteSeqIndexPort(0x90,tySRAM_1BIT,OneBitFontPY,7*27);
			for(i=0;i<255;i++)
				WriteWordIndexPort(i,tySRAM_CODE_CC_CD,i);
		break;
		case 2:
			WriteSeqIndexPort(0xd0,tySRAM_1BIT,OneBitFontCH2,48*27);
			WriteSeqIndexPort(0x90,tySRAM_1BIT,OneBitFontCH3,6*27);
			for(i=0;i<255;i++)
				WriteWordIndexPort(i,tySRAM_CODE_CC_CD,i);
		break;
		case 3:
			WriteSeqIndexPort(0xd0,tySRAM_1BIT,OneBitFontJP,45*27);
			WriteSeqIndexPort(0x90,tySRAM_1BIT,OneBitFontJP2,7*27);
			for(i=0;i<255;i++)
				WriteWordIndexPort(i,tySRAM_CODE_CC_CD,i);
		break;
		default:
			WriteSeqIndexPort(0,tySRAM_1BIT,OneBitFont,156*27);
			WriteSeqIndexPort(0xa0,tySRAM_1BIT,OneBitFont2,48*27);
			
		break;
	}
}
#endif
#ifdef RS232Key
void SearchCommand(unsigned char i)
{
	
	if(strncmp(cbuf,"print osd",9) == 0 && i >= 9)
		ICommand = 0x11; 
	if(strncmp(cbuf,"initscaler",10) == 0 && i >= 10)
		ICommand = 0x12; 
	if(strncmp(cbuf,"exit",i) == 0 && i == 4)
		ICommand = 0xf0; //exit command mode
	
	//printf(cbuf);
	//printf("ICommand=0x%x\n",(unsigned short)ICommand);
}
// The command "@a0" is a entry point to enter interpreter mode 
// The command "@q" is a exit command for escape the interpreter mode
void interpretermode()
{
	unsigned char temp,i;
	
	ForceToBackground(0,0,0);
	printf("\n******************************************************\n");
	printf("**              Command mode \n");
	printf("** \n");
	printf("******************************************************\n");
	printf("commnad\:");
	OsdInit();
	PrintOSDOnScreen(5);
	i=0;
	while(1)
	{
		
		temp = getchar();
		printf("%c",(unsigned short)temp);
		
		if(i>=64 || temp == 0x0d || temp == 0x0a)
			{
				cbuf[i]=' ';
				printf("commnad\:");
				SearchCommand(i);
				ClearOSD();
				
				switch(ICommand)
				{
					case 0x11:
						temp = cbuf[i-1];
						if(temp >= 0x30 && temp <=0x39)
							PrintOSDOnScreen(temp - 48);
						else
							PrintOSDOnScreen(0);
					break;
					case 0x12:
						OsdInit();
						InitScaler();
						return;
					break;
					case 0xf0:
						printf("\n Exit Command mode \n");
						WarmUp();
						return;
					break;
					default:
						
					break;
				}
				for(;i>0;i--)
					cbuf[i]=0; // clean the contents of cbuf[]
				ICommand=0;
			}else{
				WriteWordIndexPort(i,tySRAM_CODE_CC_CD,temp);
				cbuf[i]	= temp;
				i++;
			}
	}
}

unsigned char GetRS232KeyIn(unsigned char tbyte)
{
	unsigned char tempbyte;
	if(RI)
		{
			RI = 0;
			tempbyte = SBUF;
			//SBUF = 0x06;
			switch(tempbyte)
			{
				case  vkRSPOWER:
					tempbyte = vkPOWER;
					break;
 				//case vkRSBRIGHTNESS:
				//	tempbyte = 0;
				//	break;
				//case vkRSCONTRAST:
				//	tempbyte = 0;
				//	break;
				case vkRSVOLUMUP:	
					tempbyte = vkVALUP;
					break;
				case vkRSVOLUMDOWN:	
					tempbyte = vkVADOWN;
					break;
				case vkRSMENU:	
					tempbyte = vkMENU;
					break;
				case vkRSSOURCE:
					tempbyte = vkSOURCE;
					break;
				case vkRSAUTO:
					tempbyte = vkIR_dimmer;
					break;
				case vkTest:// taggle 3d/2d bit
					tempbyte = ReadIIC(TW9919,0x72);
					if(tempbyte & 0x80)
						tempbyte &=0x7F;
					else
						tempbyte |= 0x80;
					WriteIIC(TW9919,0x72,tempbyte);
					tempbyte = 0;
					break;
				case 0x31:// taggle adaptive mode/fixed mode
					tempbyte = ReadIIC(TW9919,0x72);
					if(tempbyte == 0xd0)
						tempbyte =0x90;
					else
						tempbyte = 0xd0;
					WriteIIC(TW9919,0x72,tempbyte);
					tempbyte = 0;
					break;
				case cwINTERPRETER:
					if(getchar() == 'a')
						if(getchar() == '0')
							interpretermode();
				default:
					tempbyte = 0;
					break;
			}
			return tempbyte;	
		}
	return tbyte;
}
#endif
bit CheckAvailableChannel(char channel)
{
	switch(channel)
	{
		case VGAinput:	
			if(dsub)
			 	return TRUE;
			break;
	 	case YPbPrinput: 
			if(YPbPr)
				return TRUE;
			break;
		case DVIinput:	
			if(dvid)
			 	return TRUE;
			break;
		case AV1input:	
			if(cvbs1)
			 	return TRUE;
			break;
		case AV2input:	
			if(cvbs2)
			 	return TRUE;
			break;
		case Svideoinput:	
			if(svideo)
			 	return TRUE;
			break;
	}
	return FALSE;
}
void GoToNextAvailableChannel()
{
	Byte Temp,OldSource;
	Osd_Off();
	OldSource = FuncBuf[pVIDEOSOURCE];

	for(Temp=0;Temp<5;Temp++)
	{
		switch(FuncBuf[pVIDEOSOURCE])
		{
			case VGAinput:
				if(dsub)
					{
					Temp = 5;
					SyncMode = 0;
					}
			break;
			case YPbPrinput:
				if(YPbPr)
					{
					Temp = 5;
					SyncMode = 2;
					}
			break;
			case DVIinput:
				if(dvid)
					{
					Temp = 5;
					SyncMode = 3;
					}
			break;
			
			case AV1input:
				if(cvbs1)
					{
					Temp = 5;
					SyncMode = 0;
					}
			break;
			case AV2input:
				if(cvbs2)
					{
					Temp = 5;
					SyncMode = 0;
					}
			break;
			case Svideoinput:
				if(svideo)
					{
					Temp = 5;
					SyncMode = 0;
					}
				break;
		};
		if(Temp<5)
			FuncBuf[pVIDEOSOURCE]++;
	
		if(FuncBuf[pVIDEOSOURCE]>Svideoinput)
			FuncBuf[pVIDEOSOURCE] = VGAinput;
	}
}


void ChannelSelect(void )
{
	unsigned char port,temp;

	PowerTimer = POWER_TIME;

	if(sensorFlag)
	{
		if(FuncBuf[pVIDEOSOURCE]<=DVIinput)
			FuncBuf[pBRIGHTNESS]=Read24C16(ep_PCADC);
		else
			FuncBuf[pBRIGHTNESS]=Read24C16(ep_VideoADC);			
	}
	
	if(PowerStatus)
	{	
		ChangeMode = 1;
		Osd_Off();
		ForceToBackground(0,0,255);
		BackLightOff();
		
		temp = ReadIIC563(0x150);
		WriteIIC563(0x150,temp&0xfe);	// output control and signal disable
		Sleep(PowerDownPanelTiming);
			port = PTC_REG;
			port |= RDPC_REG;
			port &= ~PC3;
			PTC_REG = port;
			Sleep(100);

			switch(FuncBuf[pVIDEOSOURCE])
			{
			case VGAinput:
					SyncMode = 0;
					SourceSelect();
					ChangeMode = 1;
					VideoMute = 0;
			break;
			
			case YPbPrinput:
					SyncMode = 2;
					//kevin 0713 SetInterface();
					SourceSelect();
					ChangeMode = 1;
					VideoMute = 0;
			break;
			case DVIinput:
					SyncMode = 3;
					SourceSelect();
					WriteIIC563(0x01d, 0x01); 
					ChangeMode = 1;
					VideoMute = 0;
					Osd_re_build=1;
			break;
			case AV1input:
					//==================	
					//PanelPowerOff();
					//Sleep(500);
					//==================				
//					PanelPowerOff();
					SyncMode = 0;
					VideoFlag = 0;
					VideoMute= 0;
					channel_select_video=1; // kevin 0709
					SourceSelect();
			break;
			case AV2input:
					SyncMode = 0;
					VideoFlag = 0;
					VideoMute= 0;
					channel_select_video=1; // kevin 0709
					SourceSelect();
			break;
			case Svideoinput:
					SyncMode = 0;
					VideoFlag = 0;
					VideoMute= 0;
					channel_select_video=1; 
					SourceSelect();
			break;
			default:
			break;
			}
			PanelPowerOn();
	if(!CCDSignal_IN )
		Write24C16(ep_Sync_Source,FuncBuf[pVIDEOSOURCE]);

	//printf("channel=%d \n",(unsigned short)FuncBuf[pVIDEOSOURCE]);
	}	
}
void TurnOnPWM(void)
{
	WriteWordIIC563(PWMA_Lo_Add,PWMOff); 	// turn off BL_on2
	WriteWordIIC563(PWMB_Lo_Add,PWMOff); 	// turn off BL_PWM
	WriteIIC563(0x070,0x30);			 	// GPIO controal
	WriteIIC563(0x07e,0x88);			 	// PWM controal1
#if pwmInverter	
	// for PWM control backlight 
	WriteIIC563(0x07f,0x08);				// PWM controal2 235 Hz
#else
	// for level control backlight
	WriteIIC563(0x07f,0x02);				// PWM controal2 60kHz 
#endif	
}
/*
void dumpEEPROM()
{
	unsigned char temp;
	Word i,Addr;
	for(i=0;i< ModeNum;i++)
	{
		Addr = ep_Mode_Data + i * 4;
		temp = Read24C16(Addr++);	
		printf("\n%d %x",i,(unsigned short)temp);
		temp = Read24C16(Addr++);
		printf("%x",(unsigned short)temp);
		temp = Read24C16(Addr++);
		printf(",%x",(unsigned short)temp);
		temp = Read24C16(Addr);
		printf("%x",(unsigned short)temp);
	}
	printf("\n");
}
*/
