#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "dllmain.h"

#include "CPatch.h"

using namespace std;

float GetPrivateProfileFloat(const char* selection, const char* varname, const char* default_val, const char* filename){
	char returnedString[256];
	GetPrivateProfileStringA(selection, varname, default_val, returnedString, 256, filename);
	return atof(returnedString);
}

float __fastcall CFilter__AddVal(int *CFilter, float addval, char *a3){
	typedef float (__thiscall * CFilter__AddVal)(int* CFilter, float addval, char *a3);
	return CFilter__AddVal(0x4EAD6E)(CFilter, addval, a3);
}

//double __thiscall CFilter::Calculate_(CFilter *this)
double __fastcall CFilter__Calculate(int *CFilter){
	typedef double (__thiscall * CFilter__Calculate)(int* CFilter);
	return CFilter__Calculate(0x4EB038)(CFilter);
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

//signed int __cdecl sub_578F70(_DWORD *a1, int a2);
signed int __cdecl sub_578F70(int *a1, int a2){
	typedef signed int (__cdecl * sub_578F70)(int *a1, int a2);
	return sub_578F70(0x578F70)(a1, a2);
}

typedef struct JoyData { //bytes
    int  axes[6];        //00 - 24
	int  sliders[2];     //24 - 32
	int  POVs[4];        //32 - 48
	BYTE buttons[32];    //48 - 80
} JoyData;

JoyData JoyInfo;

///// ini params /////
bool printJoyInfo;

int clutchAxisID = 1;
bool invertClutchAxis;

float clutchAxisValue;
float clutch_interp_v;

int joy_gear_R;
int joy_gear_N;
int joy_gear_1;
int joy_gear_2;
int joy_gear_3;
int joy_gear_4;
int joy_gear_5;
int joy_gear_6;
int joy_gear_range;

bool use_gear_range;
bool autoNeutralSet;
bool use_gearbox;

int clutchAxisMin;
int clutchAxisMax;
///// joy info ////

bool joyKeyStates[32];

bool IsJoyKeyPressed(int key){
	return JoyInfo.buttons[key];
}

bool IsJoyKeyJustPressed(int key){
	if (IsJoyKeyPressed(key)){
		if (!joyKeyStates[key]){
			joyKeyStates[key] = true;
			return true;
		}
	} else {
		joyKeyStates[key] = false;
	}
	return false;
}

char joyInfoText[256];
int* menuFont;

///// 2D UI /////
void __cdecl DrawText2D(int *surface, int x, int y, int *font, char *text, unsigned int color, int isRealColor){
	typedef void (__cdecl * DrawText2D)(int *surface, int x, int y, int *font, char *text, unsigned int color, int isRealColor);
	return DrawText2D(0x5EA110)(surface, x, y, font, text, color, isRealColor);
}

int *__cdecl LoadFont(char *a1){
	typedef int* (__cdecl * LoadFont)(char* a1);
	return LoadFont(0x5E8C00)(a1);
}

void drawJoyInfo(){
	//CWinApp->screenElement->surface
	int* surface  = (int*)*(int*)((char*)*(int*)0x69686C + 0xC);
	if (!menuFont)
		menuFont = LoadFont("menu.fon");

	//colors: 0 - black, 4 - white, 5 - red (from LOAD.RES->menu_.plm)
	DrawText2D(surface, 21, 21, menuFont, joyInfoText, 0, 0); //text shadow
	DrawText2D(surface, 20, 20, menuFont, joyInfoText, 4, 0); //text
}
/////

signed int __cdecl UpdateDInputDeviceState(int *a1, int a2){
	signed int result = sub_578F70(a1, a2);

	if (!result){
		memcpy(&JoyInfo, a1, 80);

		if (printJoyInfo){
			int currentDeviceId = *(int*)0x6F4BB4;

			__int16 *word_6F3570 = (__int16*)0x6F3570;
			__int16 *word_6F3572 = (__int16*)0x6F3572;
			__int16 *word_6F3574 = (__int16*)0x6F3574;
			__int16 *word_6F3576 = (__int16*)0x6F3576;

			int axes_cnt    = word_6F3570[0x11C * currentDeviceId];
			int buttons_cnt = word_6F3572[0x11C * currentDeviceId];
			int POVs_cnt    = word_6F3574[0x11C * currentDeviceId];
			int sliders_cnt = word_6F3576[0x11C * currentDeviceId];

			sprintf(joyInfoText, "Device ID: %d", currentDeviceId);
			sprintf(joyInfoText, "%s\nAxes/Buttons/POVs/Sliders: %d/%d/%d/%d", joyInfoText, axes_cnt, buttons_cnt, POVs_cnt, sliders_cnt);
			sprintf(joyInfoText, "%s\nAxes: 1.=%d 2.= %d 3.=%d 4.= %d 5.= %d 6.= %d", joyInfoText, JoyInfo.axes[0], JoyInfo.axes[1], JoyInfo.axes[2], JoyInfo.axes[3], JoyInfo.axes[4], JoyInfo.axes[5], JoyInfo.axes[6]);
		}
		
		clutchAxisValue = (abs(clutchAxisMin) + float(a1[clutchAxisID])) / (abs(clutchAxisMin) + clutchAxisMax);

		if (invertClutchAxis){
			clutchAxisValue = 1.0 - clutchAxisValue;
		}

		if (printJoyInfo){
			sprintf(joyInfoText, "%s\nclutchAxisValue = %f", joyInfoText, clutchAxisValue);
			drawJoyInfo();
		}
	}

	return result;
}

//void __thiscall Car_V::sub_4FF2A0(Car_V *this, float *a2)
void __fastcall Car_V__sub_4FF2A0(int *Car_V, float *a2){
	typedef void (__thiscall * Car_V__sub_4FF2A0)(int* Car_V, float *a2);
	return Car_V__sub_4FF2A0(0x4FF2A0)(Car_V, a2);
}

float interp(float min, float max, float power){
    return min + power * (max - min);
}

float prev_clutch = 1.0;

void __fastcall sub_4FF2A0_hook(int *Car_V, int EDX, float *gearboxRPS_ptr){
	int* playerVehicle = (int*)*(int*)0x6F6930;
	int* playerCarV = (int*)*(int*)((char*)playerVehicle + 0x5460);

	//1 - АКПП, 0 - МКПП
	bool autogear = *(int*)((char*)Car_V + 0x1C14);

	if (Car_V != playerCarV || autogear)
		return Car_V__sub_4FF2A0(Car_V, gearboxRPS_ptr);

	int* filterEngineTach = (int*)((char*)Car_V + 0x1B50);

	int wheelsCount = *(int*)((char*)Car_V + 0xDD4);

	double v2 = 0.0;
	int v4 = 0;
	int v6 = 0;
	int v7 = 0;
	int *v8;
	int *v9;
	int v18 = 0;

	if (wheelsCount > 0)
	{
		v8 = (int*)((char*)Car_V + 0x19D4);
		v9 = (int*)((char*)Car_V + 0x19A4);
		do
		{
			if ( v8[120])
			{
				++v7;
				if ( *(WORD *)v9 )
				{
					++v4;
				}
				if ( *v8 )
				{
					v2 = v2 + *((float *)v8 + 0x327);
					++v6;
				}
			}
			v9 = (int *)((char *)v9 + 2);
			v8 += 4;
			--wheelsCount;
		}
		while ( wheelsCount );
		v18 = v6;
	}

	float gearboxRPS = *gearboxRPS_ptr;
	float RPS_result = *gearboxRPS_ptr;

	//Car_V fields
	float tach_with_max_power = *(float*)((char*)Car_V + 0x1C88); //rpm when torque is maximum (tach_with_max_power from vehicle.tech)
	
	float* engineRPS = (float*)((char*)Car_V + 0x1C84); //current rps (revolutions per second), rpm = rps * 60.0
	float* clutch    = (float*)((char*)Car_V + 0x1CB8); //clutch (0.0 ... 1.0, not used)

	float throttle     = *(float*)((char*)Car_V + 0x29D8);
	int   currentGear  =   *(int*)((char*)Car_V + 0x1C1C);

	int    playerTechID            = playerVehicle[2605];
	float* maxrotmomentum_original = (float*)((0x697888 + (2416 * playerTechID) + 0xC0));
	float* maxrotmomentum_current  = (float*)(((char*)Car_V) + 0x3EBC + 0xC);

	float RPS_neutral = throttle * tach_with_max_power * 3.0;
	float RPS_gearbox = gearboxRPS;

	//для отладки
	//if (GetAsyncKeyState(VK_NUMPAD5))
	//	clutchAxisValue = 0.0;
	//else
	//	clutchAxisValue = 1.0;

	*clutch = clutchAxisValue;

	//имитация инерции вращения двигателя при нажатии на педаль сцепления
	if (clutchAxisValue < prev_clutch){
		*clutch = interp(prev_clutch, clutchAxisValue, clutch_interp_v);
	}
	prev_clutch = *clutch;

	//отключение переключения передач если не выжато сцепление
	if (clutchAxisValue > 0.5){
		*(int*)((char*)Car_V + 0x1C24) = 0;
	} else if (use_gearbox){
		//текущая передача
		int *gear_current = (int*)((char*)Car_V + 0x1C1C);
		//передача, которую следует включить - можно поменять значение,
		//а всё остальное сделает игра сама
		int *gear_target  = (int*)((char*)Car_V + 0x2746);

		if (autoNeutralSet){
			//если не зажата кнопка никакой передачи, то включение нейтрали
			if (!IsJoyKeyPressed(joy_gear_1) && !IsJoyKeyPressed(joy_gear_2) && !IsJoyKeyPressed(joy_gear_3) && !IsJoyKeyPressed(joy_gear_4) && !IsJoyKeyPressed(joy_gear_5) && !IsJoyKeyPressed(joy_gear_6) && !IsJoyKeyPressed(joy_gear_R)){
				*gear_target = 21;
			}
		}

		if (IsJoyKeyJustPressed(joy_gear_R))
			*gear_target = 20;

		if (IsJoyKeyJustPressed(joy_gear_N))
			*gear_target = 21;

		//диапазон переключения передач
		int gear_range_add = 0;
		if (use_gear_range)
			gear_range_add = 6;

		if (IsJoyKeyJustPressed(joy_gear_1))
			*gear_target = 22 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_2))
			*gear_target = 23 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_3))
			*gear_target = 24 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_4))
			*gear_target = 25 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_5))
			*gear_target = 26 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_6))
			*gear_target = 27 + gear_range_add;

		if (IsJoyKeyJustPressed(joy_gear_range)){
			use_gear_range = !use_gear_range;
			if (use_gear_range)
				cout << "Gears range: 6 - 12" << endl;
			else
				cout << "Gears range: 1 - 6" << endl;
		}
	}

	//если нейтраль или передача выше первой при оборотах выше низких (?)
	if ( currentGear == 1 || v4 == v7 ){
		RPS_result = RPS_neutral;
	} else {
		*maxrotmomentum_current = *maxrotmomentum_original * *clutch;
		if (*maxrotmomentum_current < 0.01){
			*maxrotmomentum_current = 0.01;
		}

		RPS_result = interp(RPS_neutral, RPS_gearbox, *clutch);
	}

	//т.к. сцепление не учитывается, то на грузовик действует сопротивление передачи(?), которое катит его назад
	//тормоз должен это гасить
	
	float speed_ms = *(float*)((char*)Car_V + 0xDC4);
	float *brakes  =  (float*)((char*)Car_V + 0x29DC);

	if (currentGear != 1 && speed_ms < 1.0 && *clutch < 0.3){
		*brakes = 1.0 - throttle;
	}
	

	double engRPS_min = interp(tach_with_max_power * 0.2, tach_with_max_power * 0.05, *clutch);
	if ( RPS_result < engRPS_min )
		RPS_result = engRPS_min;
	

	CFilter__AddVal(filterEngineTach, RPS_result, "correct_tach:filterEngineTach");
	*engineRPS = CFilter__Calculate(filterEngineTach);
}

double ruleMaxRotation;
void ReadSettings(){

	printJoyInfo = GetPrivateProfileIntA("MOD", "printJoyInfo", 0, ".\\KoTR_ClutchMod.ini");

	clutchAxisID     = GetPrivateProfileIntA ("CLUTCH", "axisID",     0, ".\\KoTR_ClutchMod.ini") - 1;
	invertClutchAxis = GetPrivateProfileIntA ("CLUTCH", "invert",     0, ".\\KoTR_ClutchMod.ini");
	clutchAxisMin    = GetPrivateProfileIntA ("CLUTCH", "min",        0, ".\\KoTR_ClutchMod.ini");
	clutchAxisMax    = GetPrivateProfileIntA ("CLUTCH", "max",     1000, ".\\KoTR_ClutchMod.ini");
	clutch_interp_v  = GetPrivateProfileFloat("CLUTCH", "interp", "0.3", ".\\KoTR_ClutchMod.ini");

	ruleMaxRotation = GetPrivateProfileIntA("STEERING", "cabSteerMaxDeg", 90, ".\\KoTR_ClutchMod.ini") * 3.14 / 180.0;

	use_gearbox = GetPrivateProfileIntA("GEARBOX", "enabled", 0, ".\\KoTR_ClutchMod.ini"); 

	if (!use_gearbox)
		return;

	joy_gear_R     = GetPrivateProfileIntA("GEARBOX", "joy_gear_R", 0,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_N     = GetPrivateProfileIntA("GEARBOX", "joy_gear_N", 1,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_1     = GetPrivateProfileIntA("GEARBOX", "joy_gear_1", 2,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_2     = GetPrivateProfileIntA("GEARBOX", "joy_gear_2", 3,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_3     = GetPrivateProfileIntA("GEARBOX", "joy_gear_3", 4,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_4     = GetPrivateProfileIntA("GEARBOX", "joy_gear_4", 5,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_5     = GetPrivateProfileIntA("GEARBOX", "joy_gear_5", 6,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_6     = GetPrivateProfileIntA("GEARBOX", "joy_gear_6", 7,     ".\\KoTR_ClutchMod.ini") - 1;
	joy_gear_range = GetPrivateProfileIntA("GEARBOX", "joy_gear_range", 8, ".\\KoTR_ClutchMod.ini") - 1;

	autoNeutralSet = GetPrivateProfileIntA("GEARBOX", "autoNeutralSet", 0, ".\\KoTR_ClutchMod.ini"); 

	int max_button_idx = 32; //потому что в JoyData только 32 кнопки

	if (joy_gear_R > max_button_idx)
		cout << "joy_gear_R have too high button index, max 32 supported\n";
	if (joy_gear_N > max_button_idx)
		cout << "joy_gear_N have too high button index, max 32 supported\n";
	if (joy_gear_1 > max_button_idx)
		cout << "joy_gear_1 have too high button index, max 32 supported\n";
	if (joy_gear_2 > max_button_idx)
		cout << "joy_gear_2 have too high button index, max 32 supported\n";
	if (joy_gear_3 > max_button_idx)
		cout << "joy_gear_3 have too high button index, max 32 supported\n";
	if (joy_gear_4 > max_button_idx)
		cout << "joy_gear_4 have too high button index, max 32 supported\n";
	if (joy_gear_5 > max_button_idx)
		cout << "joy_gear_5 have too high button index, max 32 supported\n";
	if (joy_gear_6 > max_button_idx)
		cout << "joy_gear_6 have too high button index, max 32 supported\n";
	if (joy_gear_range > max_button_idx)
		cout << "joy_gear_range have too high button index, max 32 supported\n";
}

void PatchBytes(){
	//замена адресов переменных для сравнения внутри условий if и т.п.
	//dbl_64B840 0.2 -> dbl_64B678 0.0 
	CPatch::SetInt(0x4FFE97, 6600312);

	//dbl_64B660 2.0 -> dbl_64B678 0.0 
	CPatch::SetInt(0x4FFEB9, 6600312);

	//dbl_64B660 2.0 -> dbl_64B678 0.0 
	CPatch::SetInt(0x4FFEF3, 6600312);

	//отключение перегазовки при переключении передач на МКПП
	CPatch::SetChar(0x5004BC, 52); //оригинал 48

	//joystick
	CPatch::RedirectCall(0x4E0F4E, &UpdateDInputDeviceState);

	//сцепление (Car_V::make_inner() -> Car_V::correct_tach())
	CPatch::RedirectCall(0x4FFC09, &sub_4FF2A0_hook);

	//макс. угол поворота руля в салоне (радианы)
	CPatch::SetPointer(0x4DEDE3, &ruleMaxRotation);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (!GetPrivateProfileIntA("MOD", "Enabled", 0, ".\\KoTR_ClutchMod.ini"))
				return TRUE;

			DisplayConsole();
			cout << "ClutchMod v0.3 (26.12.2025) started." << endl;

			ReadSettings();

			PatchBytes();

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

	return TRUE;
}
