#ifndef _UserAdj_H_
#define  _UserAdj_H_

//extern code unsigned char ColorTab[];
void setAutoPowerOnSignal(void);
void LoadModeIndependentSettings(void);
void LoadModeDependentSettings(void);
void SaveModeDependentSettings(void);
void Recall(bit);
void AdjustProcess();
void MiscProcess(bit);
void SaveData(void);
void GetFuncRange(unsigned char);
void SetFuncMaxMin();
void LoadColor(unsigned short);
void SetSource();
void SaveBackLightTime(void);
void LoadPresetModeSettings(unsigned char);
#endif
