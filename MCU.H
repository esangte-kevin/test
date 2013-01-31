#ifndef _MCU_H_
#define  _MCU_H_

extern code unsigned char KeyTab[];

#define CPU_CLK 14000000*2//unit Mhz

void InitMCU(void);
void Timer0(void);
void Sleep(unsigned short);
void WaitSetup(unsigned short time);
void KeyProcess(void);

//void EEPROM_WProtect_Enable(void); //ADY 02Aug06
//void EEPROM_WProtect_Disable(void); //ADY 02Aug06

void LED_GrnOn(void);
void LED_GrnOff(void);
void LED_RedOn(void);
void LED_RedOff(void);
void PanelAndSignalOn(void);
void PanelAndSignalOff(void);
void BackLightOn(void);
void BackLightOff(void);
bit DetectBacklight();
//void FastMuteOn();
//void FastMuteOff();
void PanelPowerOff(void);
void PanelPowerOn(void);
void ResetOff(void);
void ResetOn(void);
bit TW9919Status();
bit DetectDVI();
bit DetectDsub();
bit DetectIRQ();
//bit DetectPower();
bit VsyncValide();
void WaitVblank();
void CheckDDC2Bi(void);
void ScanKeyPad(void);
unsigned char GetKey();
void PowerOffProcess();
void AUDIO_MUTE(void);
void AUDIO_On(void);
void SetAudioVolume(void);
void SetVCM_Vol(void);
void SetAudioMute(void);
void CheckAnyKey();
void DC2DCPowerOn(void);
void DC2DCPowerOff(void);
void XAO_On(void);
void XAO_Off(void);
void AutoTest(void);
void GetFBA1665D0Flag(void);
void ChannelSelect(void);
void FastPowerOn();
void AdjustDimmer();
unsigned char GetRS232KeyIn(unsigned char tbyte);
void interpretermode();
bit  CheckAvailableChannel(char chanel);
void GoToNextAvailableChannel();
void GetAllSyncStatus();
bit CheckAllSync();
void PutStringToScreen(unsigned char *buf,unsigned char length);
void dumpEEPROM();
void VGAYPbPrSwitch(unsigned char swbit);
void TurnOnPWM(void);

#endif
