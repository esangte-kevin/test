#ifndef _OSD_TAB_H
#define _OSD_TAB_H

#ifdef CUSTOM_OSD
extern code unsigned char OSDTitle_MainMenu_font_index[];
extern code unsigned char OSDTitle_ImageSetup_font_index[];
extern code unsigned char OSDTitle_SourceSelect_font_index[];
extern code unsigned char OSDTitle_OSDSetup_font_index[];
extern code unsigned char Title_OneBitFont[];
extern code unsigned short logo_Palette2Bit[];
extern code unsigned char logo_TwoBitFont[];
extern code unsigned short logo_Palette4Bit[];
extern code unsigned char logo_FourBitFont[];
extern code unsigned short test_Palette4Bit[];
extern code unsigned char test_FourBitFont[];
extern code unsigned char palette_index[];
extern code unsigned char font_index[];
extern code unsigned char bar_TwoBitIcon[];
extern code unsigned short bar_Palette4Bit[];
extern code unsigned char bar_font_index[];
extern code unsigned char bar_palette_index[];

#endif

extern code unsigned short Palette1Bit[];
extern code unsigned short Palette2Bit[];
extern code unsigned short Palette4Bit[];
extern code unsigned char OneBitFont[];
extern code unsigned char OneBitIcon[];
extern code unsigned char TwoBitIcon[];
extern code unsigned char OneBitFont2[];
extern code unsigned char OneBitFontPY[];
extern code unsigned char OneBitFontCH2[];
extern code unsigned char OneBitFontCH3[];
extern code unsigned char OneBitFontJP[];
extern code unsigned char OneBitFontJP2[];
extern code unsigned char TwoBitFont[];
extern code unsigned char TwoBitFont2[];
extern code unsigned char FourBitFont[];
extern code unsigned char *TitalTab[][9];
extern code unsigned char *DescTab[][9];
extern code ICONDEF MenuItem[7];
//extern code ICONDEF DsubMenuItem[17];
//extern code ICONDEF DviMenuItem[17];
//extern code ICONDEF VideoMenuItem[17];
extern code Byte RGBmenuItem[];
extern code Byte FactmenuItem[];
extern code unsigned char SPACE[];
//extern code unsigned char LanguageDummy[];
extern code unsigned char *FuncTab[6];

extern Byte code PageMainMenu[7];
extern Byte code PageImageSetup[9];
extern Byte code PageSourceSelect[5];
extern Byte code PageOSDSetup[6];
extern Byte code PageChannelSelect[5];
extern Byte code PageHotKeyChannelSelect[6];


//extern Byte code Page0Item[12];
//extern Byte code Page0ItemD[9];
//extern Byte code Page0ItemY[9];
//extern Byte code Page0ItemV[11];
//extern Byte code SetupPageItem[10];
//extern Byte code OptionPageItem[6];
//extern Byte code Page3Item[1];
//extern Byte code Page4Item[6];
//extern Byte code Page5Item[5];
//extern Byte code Page6Item[6];
extern code unsigned char *Tab_Color[];
extern code unsigned char *Tab_RGB[];
extern code FACTDEF FactItem[26];
extern code unsigned char *UserTab[];
extern code unsigned char *KeyStrTab[];
extern code unsigned char *TurboStrTab[];


#endif
