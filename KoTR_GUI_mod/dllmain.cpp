#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "CPatch.h"
#include "input.h"

#include "dllmain.h"

using namespace std;

//sub_5FD3F0 - некоторая звуковая ф-ия

float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename){
	float to_return;
	char* returnedString[256];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 256, filename.c_str());
	to_return = atof(returnedString);
	return to_return;
}

string GetPrivateProfileStr(string selection, string varname, string default_val, string filename){
	char* returnedString[256];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 256, filename.c_str());
	string result = returnedString;
	return result;
}

/*void Car_V__SetVelocity(DWORD* _this, CVector *a2) {
	typedef void (__thiscall * Car_V__SetVelocity)(DWORD* _this, CVector *a2);
	return Car_V__SetVelocity(0x4F1E60)(_this, a2);
}*/

//void TaskPager::Draw(gTaskPager);
//void __thiscall sub_569A40((_DWORD **)dword_6F3458);

void TaskPager__Draw(DWORD* _this) {
	typedef void (__thiscall * TaskPager__Draw)(DWORD* _this);
	return TaskPager__Draw(0x569A40)(_this);
}

//указатели
//DWORD* vehicle = (DWORD*)*(DWORD*)0x6F6930;
//DWORD* vehicleCar_V = (DWORD *)(vehicle[5400]);

//указатели на воздуховод
//DWORD* playerID = *(DWORD**)((char *)vehicle + 0x10 + 0x508C); //работает
//cout << "playerID  =" << playerID << endl;

//DWORD* ptr1 = *(DWORD**)((char *)playerID + 0x204);
//cout << "ptr1=" << ptr1 << endl;

//DWORD* ptr2 = *(DWORD**)((char *)ptr1 + 0x10);
//cout << "ptr2=" << ptr2 << endl;

//DWORD* ptr3 = (DWORD*)((char*)ptr2 + 0x120);
//cout << "ptr3=" << ptr3 << endl;

//int upg_suspension = *(int*)((char*)vehicle + 0x52FC);
//int upg_vent_brake = *(int*)((char*)vehicleCar_V + 0x3F58 + 0x28);
//

//bool IsGamePaused(){
//	return !(*(int *)(*(int *)0x696244 + 16)); //3 - игра не на паузе, 0 - игра на паузе
//}


int GetGameFlags(){
	return (*(int *)(*(int *)0x696CC0 + 16)); //3 - игра не на паузе, 0 - игра на паузе
}

//structures
//TaskPager
struct TaskPager {
  char task[32]; //skip
  double m_dShowTimeStart;
  DWORD *m_pTheGame;
  char m_pMessagesQuery[4][128];
  char m_sNewMessage[128];
  char m_pMessages[4][128];
  int m_dwShowingCounter;
  int m_dwMessagesCount;
  int m_dwMessageQueryId;
  int m_dwMessagesShowedCount;
  int field_4BC;
  int field_4C0;
  int field_4C4;
  int field_4C8;
  int field_4CC;
  int field_4D0;
  int field_4D4;
  int field_4D8;
  float field_4DC;
  int field_4E0;
  int field_4E4;
  DWORD *tablo_txr;
  DWORD *pager_a_msk;
  DWORD *pager_p_msk;
  int field_4F4;
  int tick_;
  int field_4FC;
  DWORD *pagerSound;
  DWORD *emptyTankSound;
  DWORD *m_pMatrixFont;
  DWORD *m_pIconFont;
};
//ScreenElement
struct ScreenElement {
  char node[12]; //0xC? //skip
  DWORD *pGraphicsData;
  int bFlags;
  int field_14;
  double drawTime;
  double unkTime;
  double setupTime;
  DWORD *cursor2;
  DWORD *cursor;
};
//CWinApp
struct __declspec(align(4)) CWinApp {
  char _base[32]; //skip
  int m_hInstance;
  int m_lpCmdLine;
  int m_nCmdShow;
  int m_pszAppName;
  int m_pszAppID;
  int m_pszRegistryKey;
  int m_pDocManager;
  int m_bHelpMode;
  int m_pszExeName;
  int m_pszHelpFilePath;
  int m_pszProfileName;
  int m_hDevMode;
  int m_hDevNames;
  int m_dwPromptContext;
  int m_hLangResourceDLL;
  int m_nWaitCursorCount;
  int m_hcurWaitCursorRestore;
  int m_pRecentFileList;
  HINSTANCE m_pHinstance;
  HINSTANCE m_pPrevHinstance;
  char *m_sCmdLine;
  int m_dwShowCmd;
  void *m_nSafetyPoolSize;
  int m_lpfnDaoTerm;
  int m_dwPolicies;
  int m_dwRestartManagerSupportFlags;
  char *exeName;
  char *hlpFilePath;
  char *iniFilePath;
  int field_94;
  int field_98;
  int field_9C;
  int field_A0;
  int field_A4;
  int field_A8;
  int field_AC;
  int field_B0;
  int field_B4;
  int field_B8;
  int field_BC;
  int someElements2DCount;
  int field_C4;
  DWORD *someElements2D[10];
  ScreenElement *m_pSprites;
  ScreenElement *m_pTasks;
  ScreenElement *m_pGameStruct;
  ScreenElement *pCurrentScreenElement;
  int width;
  int height;
  int depth;
  DWORD _flags;
  DWORD *m_pMenuFont;
  char *m_sGamePath;
  LPCSTR m_sConfigFileName;
  int width__;
  int height__;
  int field_124;
  int field_128;
  int field_12C;
  int mousePosX;
  int mousePosY;
  int field_138;
  int m_dUseGamepad;
  int field_140;
  DWORD *field_144;
  int field_148;
  int bGamePause;
  int field_150;
  int m_bNeedToDestroyWnd;
  int m_bNeedToInitDirectSound;
  int field_15C;
  int field_160;
  int field_164;
  double field_168;
  double scale_time;
  double timeDelta;
  int m_pCWnd;
  HWND window;
  HINSTANCE field_188;
  int field_18C;
  int field_190;
  int field_194;
  int currentPlayerMP;
  int field_19C;
  int m_bNetIsOn;
  int field_1A4;
  int field_1A8;
  int field_1AC;
  int field_1B0;
  int field_1B4;
  DWORD playersMpCount;
  //int (__cdecl *TryToAddPlayerMp)(const char *);
  //int field_1C0;
};


//functions
//(005EA110)
//void __cdecl sub_5EA110(char *a1, int a2, signed int a3, _DWORD *a4, char *a5, int a6, int a7)
//void __cdecl DrawTextOnSurface(GraphicsData *surface, int x, int y, CFont *font, char *text, unsigned int color, int isRealColor)
void __cdecl DrawText2D(DWORD *surface, int x, int y, DWORD *font, char *text, unsigned int color, int isRealColor){
	typedef void (__cdecl * DrawText2D)(DWORD *surface, int x, int y, DWORD *font, char *text, unsigned int color, int isRealColor);
	return DrawText2D(0x5EA110)(surface, x, y, font, text, color, isRealColor);
}

//5EA150 void __cdecl DrawText_(GraphicsData *surface, int x, int y, int propLen, CFont *font, char *text, unsigned int color, int realColor)
void __cdecl DrawText2D_len(DWORD *surface, int x, int y, int propLen, DWORD *font, char *text, unsigned int color, int isRealColor){
	typedef void (__cdecl * DrawText2D_len)(DWORD *surface, int x, int y, int propLen, DWORD *font, char *text, unsigned int color, int isRealColor);
	return DrawText2D_len(0x5EA150)(surface, x, y, propLen, font, text, color, isRealColor);
}
//(005B8910) void __cdecl DrawMask(GraphicsData *a1, int x, int y, Mask *mask, int a5)
void __cdecl DrawMask(DWORD *surface, int x, int y, DWORD *mask, int use_transparency){
	typedef void (__cdecl * DrawMask)(DWORD *surface, int x, int y, DWORD *mask, int use_transparency);
	return DrawMask(0x5B8910)(surface, x, y, mask, use_transparency);
}
//(005B8650)
//void __cdecl DrawTextureFullSized(GraphicsData *a1, int x, int y, Texture *a4)
void __cdecl DrawTextureFullSized(DWORD *graphicsData, int x, int y, DWORD *texture){
	typedef void (__cdecl * DrawTextureFullSized)(DWORD *graphicsData, int x, int y, DWORD *texture);
	return DrawTextureFullSized(0x5B8650)(graphicsData, x, y, texture);
}
//(005D2DC0)
//char *__cdecl GetString(signed int a1)
char *__cdecl GetLocalizedString(signed int str_index){
	typedef char * (__cdecl * GetLocalizedString)(signed int str_index);
	return GetLocalizedString(0x5D2DC0)(str_index);
}

//(005EA080) --------------------------------------------------------
//signed int __thiscall CFont::GetProportionalTextLen(CFont *this, const char *text)
int __cdecl GetProportionalTextLen(DWORD *_this, const char *text){
	typedef int (__thiscall * GetProportionalTextLen)(DWORD *_this, const char *text);
	return GetProportionalTextLen(0x5EA080)(_this, text);
}

//ScreenElement functions
//(004DDD60)
//int __thiscall ScreenElement__GetWidth(ScreenElement *this)
int ScreenElement__GetWidth(ScreenElement *_this){
	typedef int (__thiscall * ScreenElement__GetWidth)(ScreenElement* _this);
	return ScreenElement__GetWidth(0x4DDD60)(_this);
}
//(004DDD70)
//int __thiscall ScreenElement__GetHeight(TheGame *this)

//(004DDD80)
//int __thiscall ScreenElement__GetDepth(ScreenElement *this)
//

////(00454C10) void **__thiscall copyPointer(void *this, void *a2)
void ** copyPointer(void *_this, void *a2){
	typedef void ** (__thiscall * copyPointer)(void *_this, void *a2);
	return copyPointer(0x454C10)(_this, a2);
}
//(0043FEB0) int __cdecl GetPlayerData(PlayerId *playerid, PlayerData *playerdata)
int __cdecl GetPlayerData(DWORD *playerID, DWORD *playerData){
	typedef int(__cdecl * GetPlayerData)(DWORD *playerID, DWORD *playerData);
	return GetPlayerData(0x43FEB0)(playerID, playerData);
}

//void __fastcall ProcessVehicleLights(DWORD* _this, DWORD EDX){
//void __thiscall TaskPager::Draw(TaskPager *ecx0)

//CWinApp *&g_pWinApp = *(CWinApp **)0x721770;
//CWinApp &g_WinApp = *(CWinApp*)0x696770;
//TheGame *&g_pTheGame = *(TheGame**)0x696CC0;

/* car params
		m_speed_ms = *(float *)(*(DWORD *)(m_offset + 21600) + 13100);
		m_rpm = *(float *)(m_offset + 20968) * 0.60000002;
		m_fuelLevel = *(float *)(m_offset + 20988);
		m_damageLevel = *(float *)(m_task + 0x51F0);
		m_kilometrage = ((int)(*(float *)(m_offset + 20980)));
		m_currentGear = *(DWORD *)(m_task + 20980);
		m_lightsState = *(DWORD *)(m_offset + 20920);
		m_handbrakeState = *(DWORD *)(m_task + 21004);
		m_mass = *(float *)(m_Car_V + 0x2640);

  v68 = *(_DWORD *)(dword_6D2098 + 616);
  if ( !v68 )
    return;
    dword_6F3468 = (signed __int64)(v15 * 3.6 / v69);
    dword_6F3348 = *(_DWORD *)(v68 + 21020);
    dword_6F346C = *(_DWORD *)(v68 + 20996);
    dword_6F3470 = *(_DWORD *)(v68 + 21024);
    dword_6F3474 = (signed __int64)(*(float *)(v68 + 20968) * 0.60000002);
    dword_6F347C = *(_DWORD *)(*(_DWORD *)v12 + 660);
*/

void DisplayScreenMessage(char* text){
	typedef int(*textcall2)(void* a1);
	int ret = textcall2(0x484050)(text); //выводит текст на панель
}

void DisplayPagerMessage(char* text){
	typedef int(*textcall2)(void* a1);
	int ret = textcall2(0x56ADF0)(text); //выводит текст на панель
}

char PlayerDataTemp[0xBC];

int pager_msg_time_start;
int pager_msg_char_idx;
bool pager_msg_error = false;

//некоторые шрифты
//DWORD* whiteFont  = (DWORD*)*(DWORD*)0x6CED18; //matrix 1?
//DWORD* matrix1Fon = (DWORD*)*(DWORD*)0x6CED44;
//DWORD* LargeFont  = (DWORD*)*(DWORD*)0x6CED48;
//DWORD* matrix2Fon = (DWORD*)*(DWORD*)0x6CED4C;
//DWORD* menublkFon = (DWORD*)*(DWORD*)0x6CED50;
//DWORD* menuFont = g_WinApp.m_pMenuFont;

void __fastcall DrawTopPanel(DWORD* taskPager, DWORD EDX){
	TaskPager &g_TaskPager = *(TaskPager*)taskPager;
	CWinApp &g_WinApp = *(CWinApp*)0x696770;
	ScreenElement *g_ScreenElt = g_WinApp.pCurrentScreenElement;
	DWORD *graphicsData = g_ScreenElt->pGraphicsData;

	//определение смещений
	int x = 170;
	int y = 50;
	int scr_width = ScreenElement__GetWidth(g_ScreenElt);
	int tablo_offset_x = -479;
	int text_offset_x = -10;

	if ( scr_width <= 1204 ) {
		if ( scr_width <= 800 ){
			if ( scr_width > 640 ){
				x = 250;
				y = 50;
				text_offset_x = 70;
				tablo_offset_x = -399;
			}
		} else {
			x = 682;
			y = 11;
			tablo_offset_x = -479;
		}
	} else {
		x = 1161;
		y = 11;
		tablo_offset_x = 0;
		text_offset_x = 469;
	}

	//отрисовка tablo.txr (верхняя панелька)
	DrawTextureFullSized(graphicsData, tablo_offset_x, 0, g_TaskPager.tablo_txr);

	//общие переменные
	char buf[20];
	int text_len;

	//*** параметры автомобиля игрока ***
	DWORD* vehicle = (DWORD*)*(DWORD*)0x6F6930; //*(DWORD**)((char *)0x6D2098 + 0x268); //
	
	if (!vehicle)
		return;

	DWORD* vehicleCar_V = (DWORD *)(vehicle[5400]);

	float m_speed_ms = *(float*)((char*)vehicleCar_V + 0x332C);
	float m_rpm = *(float *)((char*)vehicle + 20968) * 0.60000002; //*(float*)((char*)vehicleCar_V + 0x3C3C) * 0.60000002;
	int currentGear = *(int *)((char*)vehicle + 20980 + 0x10);
	int gearboxType = *(int *)((char*)vehicle + 21024); //*(_DWORD *)(v68 + 21024);
	bool handbrakeState = *(int *)((char*)vehicle + 21004 + 0x10);
	float fuelLevel = *(float *)((char*)vehicle + 20988);
	float damageLevel = 1.0 - *(float *)((char*)vehicle + 0x10 + 0x51F0);
	float kilometrage = ((float)(*(float *)((char*)vehicle + 20980))); //float, значение в метрах

	int techID = *(int*)((char*)vehicleCar_V + 0x8CC);
	DWORD* playerID = (DWORD*)*(DWORD *)((char*)vehicle + 20636);

	//m_currentGear = *(DWORD *)(m_task + 20980);
	//m_lightsState = *(DWORD *)(m_offset + 20920);
	//m_handbrakeState = *(DWORD *)(m_task + 21004);
	//float fuelLevel = *(float *)(vehicle_offset + 20988);
	//float carDamageLevel = *(float *)(vehicle_task + 0x51F0);
	//int engineState = *(int *)(vehicle_offset + 21012); //0-выключен, 1-запуск, 2-включён, 3-выключение

	//cout << m_speed_ms << endl;

	//DWORD* vehicle = (DWORD*)*(DWORD*)0x6F6930;
	//DWORD* vehicleCar_V = (DWORD *)(vehicle[5400]);
	//указатели на воздуховод
	//DWORD* playerID = *(DWORD**)((char *)vehicle + 0x10 + 0x508C); //работает
	//int upg_suspension = *(int*)((char*)vehicle + 0x52FC);
	//int upg_vent_brake = *(int*)((char*)vehicleCar_V + 0x3F58 + 0x28);

    //dword_6F3468 = (signed __int64)(v15 * 3.6 / v69);
    //dword_6F3348 = *(_DWORD *)(v68 + 21020);
    //dword_6F346C = *(_DWORD *)(v68 + 20996);
    //dword_6F3470 = *(_DWORD *)(v68 + 21024);
    //dword_6F3474 = (signed __int64)(*(float *)(v68 + 20968) * 0.60000002);
    //dword_6F347C = *(_DWORD *)(*(_DWORD *)v12 + 660);

	///// ***** панель времени ***** /////
	//char c_playerID[0x3E8];
	//char c_playerData[0x3E0];
	bool is_in_race;
	double race_time_left;
	double race_time_passed;
	int licenses_count;
	double license_time;
	int currentRacePosition = 0; // = *(DWORD*)0x67B72C;
	int currentRaceVehCount = 0;
	//if ( !(v10 % SLODWORD(v69)) || v9 < 10 ){
	if (g_TaskPager.field_4FC < 10)
	{
		if (playerID != (DWORD*)*(DWORD*)0x68B9B0)
		{
			GetPlayerData(playerID, (DWORD*)PlayerDataTemp);
			is_in_race = *(int*)((char*)PlayerDataTemp + 0x88);

			if (is_in_race){
				//PlayerId->PlayerData->параметры
				currentRacePosition = 1 + *(int*)((char*)playerID + 0x8 + 0x208);
				currentRaceVehCount = 1 + *(int*)((char*)playerID + 0x8 + 0x204);
				//cout << currentRacePosition << endl;
				//cout << currentRaceVehCount << endl;
			} else {
				currentRacePosition = -1;
			}
			race_time_left = *(double*)((char*)PlayerDataTemp + 0x78);
			race_time_passed = *(double*)((char*)PlayerDataTemp + 0x80);
			license_time = *(double*)((char*)PlayerDataTemp + 0x40); //через чит лицензия на 12 минут
			licenses_count = (int)((*(int*)((char*)playerID + 0x94) - *(int*)((char*)playerID + 0x90)) / 8);

			//cout << "lic_time=" << *(double*)((char*)PlayerDataTemp + 0x78) << endl; //time_for_race?
		}
	}

	if (playerID != (DWORD*)*(DWORD*)0x68B9B0){
		if (currentRacePosition >= 0){ // если сейчас идёт гонка
			DrawText2D(graphicsData, text_offset_x + 26, 23, g_TaskPager.m_pMatrixFont, GetLocalizedString(89), 0, 0);// ПОЗИЦИЯ
			sprintf(buf, "%d", currentRacePosition);
			//уменьшение размера текста?
			char v17 = buf[0];
			if (buf[0]){
				char* v18 = buf;
				do {
					*v18 = v17 + 16;
					v17 = (v18++)[1];
				} while ( v17 );
			}
			DrawText2D(graphicsData, text_offset_x + 26, 43, g_TaskPager.m_pIconFont, buf, 0, 0);// %d
			
			//из
			text_len = GetProportionalTextLen(g_TaskPager.m_pMatrixFont, GetLocalizedString(95));
			DrawText2D(graphicsData, text_offset_x - text_len / 2 + 50, 38, g_TaskPager.m_pMatrixFont, GetLocalizedString(95), 0, 0);// из

			//кол-во соперников
			sprintf(buf, "%d", currentRaceVehCount);
			char v22 = buf[0];
			if ( buf[0] ){
				char* v23 = buf;
				do {
					*v23 = v22 + 16;
					v22 = (v23++)[1];
				} while ( v22 );
			}
			DrawText2D(graphicsData, text_offset_x + 63, 43, g_TaskPager.m_pIconFont, buf, 0, 0);// %d

			//время оставшееся
			DrawText2D(graphicsData, text_offset_x + 85, 23, g_TaskPager.m_pMatrixFont, GetLocalizedString(90), 0, 0);// ОСТ. ВРЕМЯ
			int v83 = (signed int)(signed __int64)race_time_left % 60;
			int v82 = ((signed int)(signed __int64)race_time_left / 60);
			sprintf(buf, "%02d:%02d", v82, v83);
			char v25 = buf[0];
			if ( buf[0] ){
				char* v26 = buf;
				do {
					*v26 = v25 + 16;
					 v25 = (v26++)[1];
				} while ( v25 );
			}
			DrawText2D(graphicsData, text_offset_x + 85, 43, g_TaskPager.m_pIconFont, buf, 0, 0);// %02d:%02d

			//время в пути
			if (!licenses_count){
				DrawText2D(graphicsData, text_offset_x + 158, 23, g_TaskPager.m_pMatrixFont, "В ПУТИ", 0, 0);
				int v83 = (signed int)(signed __int64)race_time_passed % 60;
				int v82 = ((signed int)(signed __int64)race_time_passed / 60);
				sprintf(buf, "%02d:%02d", v82, v83);
				char v25 = buf[0];
				if ( buf[0] ){
					char* v26 = buf;
					do {
						*v26 = v25 + 16;
						 v25 = (v26++)[1];
					} while ( v25 );
				}
				DrawText2D(graphicsData, text_offset_x + 158, 43, g_TaskPager.m_pIconFont, buf, 0, 0);// %02d:%02d
			}
		}
		//лицензии на найм
		if (licenses_count > 0){
			DrawText2D(graphicsData, text_offset_x + 154, 23, g_TaskPager.m_pMatrixFont, GetLocalizedString(101), 0, 0); //ЛИЦЕНЗИИ
			sprintf(buf, "%d", licenses_count);

			DrawText2D(graphicsData, text_offset_x + 210, 23, g_TaskPager.m_pMatrixFont, buf, 0, 0);
		  
			int v67 = (signed int)(signed __int64)license_time % 60;
			int v66 = (signed int)(signed __int64)license_time / 60;
			sprintf(buf, "%02d:%02d", v66, v67);

			char v28 = buf[0];
			if (buf[0]){
				char* v29 = buf;
				do{
					*v29 = v28 + 16;
					v28 = (v29++)[1];
				} while ( v28 );
			}
			DrawText2D(graphicsData, text_offset_x + 158, 43, g_TaskPager.m_pIconFont, buf, 0, 0);
		}
	}
	

	///// ***** панель параметров автомобиля ***** /////

	///// скорость
	bool use_kmph = *(DWORD*)0x696C74; //1 - км/ч, 0 - миль/ч
	int currentSpeed; //int &currentSpeed = *(int*)0x6F3468;
	//* вывод единицы измерения скорости
	if (use_kmph){
		//м/c
		//currentSpeed = (int)(m_speed_ms);
		//DrawText2D(graphicsData, text_offset_x + 291, 40, g_TaskPager.m_pMatrixFont, "м/с", 0, 0);
		currentSpeed = (int)(m_speed_ms * 3.6);
		DrawText2D(graphicsData, text_offset_x + 273, 30, g_TaskPager.m_pMatrixFont, GetLocalizedString(93), 0, 0); //км/ч
	} else {
		currentSpeed = (int)(m_speed_ms * 2.236);
		DrawText2D(graphicsData, text_offset_x + 273, 30, g_TaskPager.m_pMatrixFont, "MpH", 0, 0);
	}
	//* вывод самого значения скорости
	sprintf(buf, "%d", currentSpeed);
			
	char v17 = buf[0];
	if (buf[0]){
		char* v18 = buf;
		do {
			*v18 = v17 + 16;
			v17 = (v18++)[1];
		} while ( v17 );
	}

	text_len = GetProportionalTextLen(g_TaskPager.m_pIconFont, buf);
	DrawText2D(graphicsData, text_offset_x - text_len + 270, 32, g_TaskPager.m_pIconFont, buf, 0, 0); //32
	//DrawText2D(graphicsData, text_offset_x - text_len + 287, 38, g_TaskPager.m_pIconFont, buf, 0, 0);

	//* вывод одометра
	// поменять местами с тахометром
	//kilometrage += 9999.9;
	//kilometrage *= 90000.0; //для теста

	int kilometrage_km = kilometrage / 1000.0;
	int kilometrage_m  = (kilometrage - kilometrage_km * 1000) / 100.0;

	//cout << kilometrage << "->" << kilometrage_km << " | " << kilometrage_m << endl;

	//sprintf(buf, "%d.%d км", 720000, 1);
	if (kilometrage_km < 10){
		sprintf(buf, "00000%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, "     %d.%d км", kilometrage_km, kilometrage_m);
		//           "72000x.1 км"
		//sprintf(buf, "720000.1 км");
	} else if (kilometrage_km >= 10 && kilometrage_km < 100){
		sprintf(buf, "0000%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, "    %d.%d км", kilometrage_km, kilometrage_m);
		//           "7200xx.1 км"
	} else if (kilometrage_km >= 100 && kilometrage_km < 1000){
		sprintf(buf, "000%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, "   %d.%d км", kilometrage_km, kilometrage_m);
		//           "720xxx.1 км"
	} else if (kilometrage_km >= 1000 && kilometrage_km < 10000){
		sprintf(buf, "00%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, "  %d.%d км", kilometrage_km, kilometrage_m);
		//           "72xxxx.1 км"
	} else if (kilometrage_km >= 10000 && kilometrage_km < 100000){
		sprintf(buf, "0%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, " %d.%d км", kilometrage_km, kilometrage_m);
		//           "7xxxxx.1 км"
	} else if (kilometrage_km >= 100000 && kilometrage_km < 999999){
		sprintf(buf, "%d.%d км", kilometrage_km, kilometrage_m);
		//sprintf(buf, " %d.%d км", kilometrage_km, kilometrage_m);
		//           "7xxxxx.1 км"
	} else {
		sprintf(buf, "> 999999 км");
	}

	//sprintf(buf, "100000.1 км");

	//text_offset_x + 291, 40
	//232, 40
	DWORD* menublkFon = (DWORD*)*(DWORD*)0x6CED50;
	DrawText2D_len(graphicsData, text_offset_x + 241, 39, 64, menublkFon, buf, 0, 0); //на месте указателя текущей передачи
	//DrawText2D(graphicsData, text_offset_x + 233, 40, menublkFon, buf, test_idx, test_idx2); //на месте указателя текущей передачи

	/////обороты двигателя
	int currentRPM = (int)m_rpm;
	sprintf(buf, "%d", currentRPM); //sprintf(buf, "%d", currentSpeed);

	//* вывод оборотов двигателя
	text_len = GetProportionalTextLen(g_TaskPager.m_pMatrixFont, buf);
	DrawText2D(graphicsData, text_offset_x - text_len + 385, 40, g_TaskPager.m_pMatrixFont, buf, 0, 0);

	//* вывод единицы оборотов двигателя
	DrawText2D(graphicsData, text_offset_x + 386, 40, g_TaskPager.m_pMatrixFont, GetLocalizedString(94), 0, 0); //x100 об/м



	///// коробка передач
	char currentGear_c[3];
	int currentGear_display;

	//currentGear += 8;

	if (currentGear <= 1){
		sprintf(currentGear_c, " %c", (currentGear != 0 ? 'N' : 'R'));
	} else if (currentGear > 1 && currentGear < 11){
		sprintf(currentGear_c, " %c", (currentGear + '/'));
	} else {
		sprintf(currentGear_c, "1%c", ((currentGear - 10) + '/'));
	}

	if (gearboxType){ //МКПП или АКПП
		sprintf(buf, "%s%s", GetLocalizedString(91), currentGear_c); //АКПП
	} else {
		sprintf(buf, "%s%s", GetLocalizedString(92), currentGear_c); //МКПП
	}
	
	//DrawText2D(graphicsData, text_offset_x + 295, 23, g_TaskPager.m_pMatrixFont, buf, 0, 0); //возле скорости
	DrawText2D(graphicsData, text_offset_x + 15 + 24 + 361, 23, g_TaskPager.m_pMatrixFont, buf, 0, 0); //возле уровня топлива
	//DrawText2D(graphicsData, text_offset_x + 15 + 361, 23, g_TaskPager.m_pMatrixFont, buf, 0, 0);

	/////ручной тормоз
	if ( handbrakeState ){ //&& HandBrakeTicking )
		//стандартное положение
		//DrawText2D(graphicsData, text_offset_x + 315, 40, g_TaskPager.m_pIconFont, "[\\", 0, 0);
		//в окне навигатора (x+608)
		DrawText2D(graphicsData, text_offset_x + 596, 40, g_TaskPager.m_pIconFont, "[\\", 0, 0);
	}

	/////панель состояния машины и уровня топлива
	//IconFont, если выводить %c цифру:
	//48-57 крупные цифры
	//59-61 навигатор
	//64-73 малые цифры
	//75-76 прицепа нет
	//77-78 прицеп есть
	//80-90 шкала топлива/состояния машины
	//91-92 ручник
	//95 - car, 79 - truck, 63 - fuel
	//96-107? навигатор? нестандартный какой-то, возможно, для сети
	//191 - красный символ заправки
	//sprintf(buf, "%c%c", 95, car_damage_level + 80); //панель состояния легковушки
	//sprintf(buf, "%c%c", 79, car_damage_level + 80); //панель состояния грузовика

	int damageLevel_display = ceil(10.0 * damageLevel);
	int fuelLevel_display = ceil(10.0 * fuelLevel);

	//при 81 и 80 пустая шкала, поэтому если значение близко к 0, но не равно 0,
	//то отрисовка хотя бы одной палки шкалы
	if (damageLevel > 0.0 && damageLevel < 0.2){
		damageLevel_display = 2;
	}
	if (fuelLevel > 0.0 && fuelLevel < 0.2){
		fuelLevel_display = 2;
	}

	//cout << "damageLevel=" << damageLevel << endl;
	//cout << "damage_disp=" << damageLevel_display << endl << endl;

	//cout << "fuelLevel=" << fuelLevel << endl;
	//cout << "fuel_disp=" << fuelLevel_display << endl << endl << endl;

	if ((techID >= *(DWORD*)0x6CE350) && (techID < *(DWORD*)0x6CE354)){
		sprintf(buf, "%c%c", 95, damageLevel_display + 80); //панель состояния легковушки
	} else {
		sprintf(buf, "%c%c", 79, damageLevel_display + 80); //панель состояния грузовика
	}

	DrawText2D(graphicsData, text_offset_x + 553, 40, g_TaskPager.m_pIconFont, buf, 0, 0); //уровень повреждений
	
	sprintf(buf, "%c%c", 63, fuelLevel_display + 80);
	DrawText2D(graphicsData, text_offset_x + 512, 40, g_TaskPager.m_pIconFont, buf, 0, 0); //уровень топлива

	/////панель прицепа
	int trailerState = g_TaskPager.field_4F4;
	if (trailerState){
		if (trailerState != 1){
			DrawText2D(graphicsData, text_offset_x + 458, 40, g_TaskPager.m_pIconFont, "KL", 0, 0);
			//v80 = "KL";
		} else {
			DrawText2D(graphicsData, text_offset_x + 458, 40, g_TaskPager.m_pIconFont, "MN", 0, 0);
			//v80 = "MN";
		}
	} else {
		DrawText2D(graphicsData, text_offset_x + 458, 40, g_TaskPager.m_pIconFont, "KL", 0, 0);
		//v80 = "KL";
	}


	///// присвоение значений стандартным DWORD /////
	*(int*)0x6F3468 = currentSpeed;
	*(int*)0x6F3474 = currentRPM;
	*(int*)0x6F346C = currentGear;
	*(int*)0x6F3470 = gearboxType;

	*(int*)0x67B72C = currentRacePosition;
	*(int*)0x6F3464 = currentRaceVehCount;
	*(int*)0x6F3478 = licenses_count;

	*(double*)0x6F3480 = race_time_left;
	*(double*)0x6F3488 = license_time;
	
	//dword_6F347C - шо-то с навигатором?

	//if (IsKeyJustPressed(71)){ //G key
	//	DisplayPagerMessage("ГООООООООООООООООоооооооооооооооооОООоооооооооооооооОООЛ!");
	//}


	/////пейджер, надо переделать

	int showCounter = g_TaskPager.m_dwShowingCounter;

	//if (showCounter)
	//	DrawMask(graphicsData, x, y, g_TaskPager.pager_p_msk, -1);
	
	//заводской костыль? без него такое в warn.log
	//warning : ddrw mask fail There is an undefined error condition. (unknown) 0
	DrawText2D(graphicsData, 20, 20, g_TaskPager.m_pMatrixFont, " ", 0, 0);
	if (showCounter == 1){
		if (g_TaskPager.field_4C0){
		  DrawMask(graphicsData, x, y, g_TaskPager.pager_p_msk, -1);//
		  DrawText2D_len(graphicsData, x + 34, y + 23, 247, g_TaskPager.m_pMatrixFont, g_TaskPager.m_sNewMessage, 0, 0);
		}
	} else {
		DrawMask(graphicsData, x, y, g_TaskPager.pager_p_msk, -1); //
		if ((showCounter > 1) && (showCounter < 20) && (showCounter / 2 & 1)){
			DrawMask(graphicsData, x + 5, y + 6, g_TaskPager.pager_a_msk, -1);
			//обнуление таймера вывода
			pager_msg_time_start = 0;
			pager_msg_char_idx = 0;
			pager_msg_error = false;
		}
		//плавный вывод текста (msg)
		if (showCounter > 1 ){
			//обнуление таймера вывода
			//if (!pager_msg_time_start){
			//	pager_msg_time_start = showCounter; 
			//	pager_msg_char_idx = 0;
			//	pager_msg_error = false;
			//}

			int pager_msg_time_passed = showCounter - pager_msg_time_start;

			char pager_msg_text[38];
			memset(pager_msg_text, 0, 38);

			int msg_len = 0;
			while (true){
				if (!g_TaskPager.m_sNewMessage[pager_msg_char_idx + msg_len]){
					break;
				}
				msg_len++;
			}

			//заполнение выводимой строки текстом сообщения
			for (int j = 0; j < 38; j++){
				if (g_TaskPager.m_sNewMessage[pager_msg_char_idx + j]){
					pager_msg_text[j] = g_TaskPager.m_sNewMessage[pager_msg_char_idx + j];
				} else {
					break;
				}
			}

			DrawText2D(graphicsData, x + 34, y + 23, g_TaskPager.m_pMatrixFont, pager_msg_text, 0, 0);

			//ожидание перед перемещением текста
			if (!pager_msg_error){
				if (abs(pager_msg_time_passed) > 25){
					pager_msg_error = true;
				} else {
					return;
				}
			}
		
			char temp_char = g_TaskPager.m_sNewMessage[pager_msg_char_idx];

			if (abs(pager_msg_time_passed) > 1){
				if (temp_char && (msg_len > 38)){ //ограничитель
					pager_msg_char_idx++;
				}
				pager_msg_time_start = showCounter;
			}

			//увеличение значения таймера, чтобы не ожидать долго, пока сообщение пропадёт
			if ( showCounter < 1000 ){
				g_TaskPager.m_dwShowingCounter = 1000;
			}
		}
	}
}

void DisplayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//cout << "Mod debug window started" << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//if (consoleEnabled){
			//DisplayConsole();
			//}

			CPatch::RedirectJump(0x569A40, &DrawTopPanel);

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}