#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "dllmain.h"
#include "detours.h"

using namespace std;

bool gameKeyStates[44];

int GetGameKeyState(int key) {
	typedef int (__thiscall * GetGameKeyState)(DWORD* _this, int key);
	return GetGameKeyState(0x530010)((DWORD*)0x6D1DD8, key);
}

bool IsGameKeyJustPressed(int key){
	if (GetGameKeyState(key)){
		if (!gameKeyStates[key]){
			gameKeyStates[key] = true;
			return true;
		}
	}
	else {
		gameKeyStates[key] = false;
	}
	return false;
}

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

DWORD* SearchRESSound(char *a1){
	typedef DWORD*(*SearchRESSound)(char *a1);
	DWORD* ret = SearchRESSound(0x5EF380)(a1);
	return ret;
}

void PlayWAV(DWORD* sound, float volume){
	typedef int(*PlayResSound)(DWORD* sound, float volume);
	int ret = PlayResSound(0x52F6B0)(sound, volume);
}

void PlayWAVLocated(DWORD* sound, float a2, float a3, DWORD *position){
	typedef int(*PlaySoundLocated)(DWORD* sound, float a2, float a3, DWORD *position);
	int ret = PlaySoundLocated(0x52F800)(sound, a2, a3, position); //52F920
}

int getUpgradeEngineInfo(DWORD* vehicle){
	DWORD* vehicleCar_V = (DWORD *)(vehicle[5400]);
	return *(int*)((char*)vehicleCar_V + 0x3EBC + 0x28);
}

class CVehicle
{
public:
	DWORD* m_offset;        //указатель на машину игрока
	DWORD* m_prev_offset;   //предыдущий указатель на машину
	//DWORD* m_task;        //параметры автомобиля
	//DWORD* m_Car_V;       //данные машины игрока из tech
	string m_car_prefix;    //префикс автомобиля
	string m_cab_prefix;    //префикс салона
	float m_rpm;
	int m_currentGear;
	int m_vehicleID;        //ID из vehicle.tech
	int m_handbrakeState;
	//DWORD* sound_pos;       //точка воспроизведения звука (не совпадает с позицией автомобиля игрока)
	int cameraMode;
	///////////////////////////
	string engine_prefix;
	//string engine_prefix_upgr;
	float m_accel_rpm;
	///////////////////////////
	bool is_engine_upgraded;
	///////////////////////////

	DWORD* accel_snd;

	DWORD* idle_snd;
	DWORD* high_free_snd;
	DWORD* high_full_snd;

	DWORD* start_snd;
	DWORD* start_fail_snd;
	DWORD* stop_snd;

	DWORD* int_idle_snd;
	DWORD* int_high_free_snd;
	DWORD* int_high_full_snd;

	void Clear()
	{
		m_offset = nullptr;
		m_prev_offset = nullptr;
		//m_task = nullptr;
		//m_Car_V = nullptr;
		m_car_prefix = "";
		m_cab_prefix = "";
		m_rpm = 0;
		m_currentGear = 0;
		m_vehicleID = 0;
		m_handbrakeState = 0;
		/////
	}
	CVehicle()
	{
		Clear();
	}
	void Reset(){
		//cout << "reset\n";

		//m_offset = (DWORD*)(*(DWORD*)0x6F6930);
		m_prev_offset = m_offset;

		//m_task = (m_offset + 0x10);
		//m_Car_V = (DWORD *)(m_offset[5400]);
		m_vehicleID = *(int *)(*(int *)0x6D2098 + 104);
		m_car_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2342));
		m_cab_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2322));
		m_rpm = 0;
		m_currentGear = 0;
		m_handbrakeState = 0;

		//cout << "m_car_prefix=" << m_car_prefix.c_str() << endl;
		//cout << "m_cab_prefix=" << m_cab_prefix.c_str() << endl;

		/////////////////
		is_engine_upgraded = getUpgradeEngineInfo(m_offset);
		/////////////////

		if (is_engine_upgraded)
			engine_prefix = GetPrivateProfileStr(m_car_prefix, "engine_prefix_upgraded", "", ".\\KoTR_SoundMod.ini");
		else
			engine_prefix = GetPrivateProfileStr(m_car_prefix, "engine_prefix_default", "", ".\\KoTR_SoundMod.ini");

		if (is_engine_upgraded && !engine_prefix.length()){
			engine_prefix = GetPrivateProfileStr(m_car_prefix, "engine_prefix_default", "", ".\\KoTR_SoundMod.ini");
		}


		//engine_prefix_upgr = GetPrivateProfileStr(m_car_prefix, "engine_prefix_upgraded", "", ".\\KoTR_SoundMod.ini");

		//cout << "engine_prefix=" << engine_prefix.c_str() << endl;
		//cout << "engine_prefix_upgr=" << engine_prefix_upgr.c_str() << endl;

		//if (!engine_prefix_upgr.length()){
		//	engine_prefix_upgr = engine_prefix;
		//}

		m_accel_rpm = (GetPrivateProfileFloat(m_car_prefix, "accel_rpm", "0", ".\\KoTR_SoundMod.ini") / 100);

		char snd_name[255];
		sprintf(snd_name, "%s_accel", engine_prefix.c_str());
		accel_snd = SearchRESSound(snd_name);

		//cout << snd_name << endl;

		//салонные звуки
		sprintf(snd_name, "%s_int_idle", engine_prefix.c_str());
		int_idle_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;
		
		sprintf(snd_name, "%s_int_full", engine_prefix.c_str());
		int_high_full_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		sprintf(snd_name, "%s_int_full_free", engine_prefix.c_str());
		int_high_free_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		//внешние звуки
		sprintf(snd_name, "%s_idle", engine_prefix.c_str());
		idle_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;
		
		sprintf(snd_name, "%s_full", engine_prefix.c_str());
		high_full_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		sprintf(snd_name, "%s_full_free", engine_prefix.c_str());
		high_free_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		//start, stop
		sprintf(snd_name, "%s_start", engine_prefix.c_str());
		start_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;
		
		sprintf(snd_name, "%s_start_fail", engine_prefix.c_str());
		start_fail_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		sprintf(snd_name, "%s_stop_eng", engine_prefix.c_str());
		stop_snd = SearchRESSound(snd_name);
		//cout << snd_name << endl;

		//если нет звуков high_free
		if (!high_free_snd)
			high_free_snd = high_full_snd;
		if (!int_high_free_snd)
			int_high_free_snd = int_high_full_snd;

		//если нет салонных звуков
		if (!int_idle_snd)
			int_idle_snd = idle_snd;

		if (!int_high_full_snd)
			int_high_full_snd = high_full_snd;

		if (!int_high_free_snd)
			int_high_free_snd = high_free_snd;


		if (start_snd)
			*(DWORD*)0x6D1D14 = (DWORD)start_snd;
		else 
			*(DWORD*)0x6D1D14 = (DWORD)SearchRESSound("start0Sound");

		if (start_fail_snd)
			*(DWORD*)0x6D1D68 = (DWORD)start_fail_snd;
		else 
			*(DWORD*)0x6D1D68 = (DWORD)SearchRESSound("startglochSound");

		if (stop_snd)
			*(DWORD*)0x6D1D18 = (DWORD)stop_snd;
		else 
			*(DWORD*)0x6D1D18 = (DWORD)SearchRESSound("stop_engSound");

		//DWORD* stop_eng_sound = (DWORD*)0x6D1D18;
		//DWORD* startgloch_sound = (DWORD*)0x6D1D68;

		/*cout << "accel_snd=" << accel_snd << endl;

		cout << "idle_snd=" << idle_snd << endl;
		cout << "high_free_snd=" << high_free_snd << endl;
		cout << "high_full_snd=" << high_full_snd << endl;

		cout << "start_snd=" << start_snd << endl;
		cout << "start_fail_snd=" << start_fail_snd << endl;
		cout << "stop_snd=" << stop_snd << endl;*/
	}
	float GetDistanceBetweenViewerAndVehicle(){
		//float VehiclePos_x = *(DOUBLE *)0x68BAD0;
		//float VehiclePos_y = *(DOUBLE *)0x68BAD8;
		//float VehiclePos_z = *(DOUBLE *)0x68BAE0;
		//float ViewerPos_x = *(float *)0x6F69AC;
		//float ViewerPos_y = *(float *)0x6F69B0;
		//float ViewerPos_z = *(float *)0x6F69B4;
		return sqrt(pow((*(DOUBLE *)0x68BAD0 - *(float *)0x6F69AC), 2) + pow((*(DOUBLE *)0x68BAD8 - *(float *)0x6F69B0), 2));
	}
	float GetExternalSoundVolume(float max){
		float result = 5.6 / GetDistanceBetweenViewerAndVehicle();
		if (result > max){
			return max;
		}
		return result;
	}

	void Process()
	{
		m_offset = (DWORD*)(*(DWORD*)0x6F6930);
		cameraMode = *(int *)(*(int *)0x6D2098 + 1400);

		//if no vehicle pointer or vehicle changed or engine upgrade installed
		if (!m_offset || (m_offset != m_prev_offset) || is_engine_upgraded != getUpgradeEngineInfo(m_offset)){
			Reset();
		}

		// || is_engine_upgraded != getUpgradeEngineInfo(m_offset)
		//if (is_engine_upgraded != getUpgradeEngineInfo(m_offset)){
		//	cout << "engine upgrade state changed\n";
		//	Reset();
		//}
		if (!m_offset){
			return;
		}

		m_rpm = *(float *)((char*)m_offset + 0x51E8) * 0.60000002;

		if (IsGameKeyJustPressed(0)){
			if (m_rpm && m_rpm <= m_accel_rpm){
				if (cameraMode){
					PlayWAV(accel_snd, GetExternalSoundVolume(0.25));
				} else {
					PlayWAV(accel_snd, 0.45);
				}
			}
		}

		//m_rpm = *(float *)(m_offset + 20968) * 0.60000002;
		//m_currentGear = *(DWORD *)(m_task + 20980);
		//m_handbrakeState = *(DWORD *)(m_task + 21004);
	}

	DWORD* GetIdleSound(){
		if (!cameraMode)
			return int_idle_snd;

		return idle_snd;
	}

	DWORD* GetFullSound(){
		//если газ зажат, то high_full, иначе high_free
		if (!cameraMode){
			if (GetGameKeyState(0))
				return int_high_full_snd;
			return int_high_free_snd;

		} else {
			if (GetGameKeyState(0))
				return high_full_snd;
			return high_free_snd;
		}
	}
};


CVehicle PlayerVehicle;
void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	if (*(DWORD *)0x6D2098) {
		//cout << "aboba\n";
		PlayerVehicle.Process();
	}
}



void DisplayScreenMessage(char* text){
	typedef int(*textcall2)(void* a1);
	int ret = textcall2(0x484050)(text); //выводит текст на панель
}

//struct ViewerSounds:
DWORD* idleSound = (DWORD*)0x6D1D20;
DWORD* jidleSound = (DWORD*)0x6D1D28;
DWORD* fullSound = (DWORD*)0x6D1D24;
DWORD* jfullSound = (DWORD*)0x6D1D2C;

DWORD* start0_sound = (DWORD*)0x6D1D14;
DWORD* stop_eng_sound = (DWORD*)0x6D1D18;
DWORD* startgloch_sound = (DWORD*)0x6D1D68;

int GetActionState(int key){
	typedef int(__thiscall *GetActionState)(DWORD *a1, int a2);
	return GetActionState(0x530010)((DWORD *)0x6D1DD8, key);
}

DWORD* GetNewSound(DWORD* old_snd, DWORD* new_snd){
	if (new_snd)
		return new_snd;

	return old_snd;
}

int sub_52F700(DWORD *a1, float a2){
	typedef int(__cdecl *sub_52F700)(DWORD *a1, float a2);
	return sub_52F700(0x52F700)(a1, a2);
}

int sub_5FD580(DWORD *a1, float a2){
	typedef int(__thiscall *sub_5FD580)(DWORD *a1, float a2);
	return sub_5FD580(0x5FD580)(a1, a2);
}

//работает при виде из салона
int __cdecl sub_52F750(DWORD *sound, float volume, float pitch){
	int result; // eax

	if (sound == (DWORD*)*fullSound || sound == (DWORD*)*jfullSound){
		int current_gear = *(DWORD *)((*(int *)(*(int *)0x6D2098 + 0x268) + 0x10) + 20980);
		if (!GetActionState(0) && current_gear == 1 && pitch < 0.3){ //если газ не нажат и передача = нейтральная и обороты примерно холостые
			return 0;
		}
		sound = GetNewSound(sound, PlayerVehicle.GetFullSound());
	} else {
		sound = GetNewSound(sound, PlayerVehicle.GetIdleSound());
	}

	if (sound){
		sub_52F700(sound, volume);
		result = sub_5FD580(sound, pitch);
	}
	return result;
}

void sub_52F780(DWORD *a1, float a2, float a3, DWORD* pos){
	typedef void(__cdecl *sub_52F780)(DWORD *a1, float a2, float a3, DWORD* pos);
	return sub_52F780(0x52F780)(a1, a2, a3, pos);
}

//работает при виде снаружи
int __cdecl sub_52F7D0(DWORD *sound, float a2, float a3, float pitch, DWORD* pos){
	int result;

	//PlayerVehicle.sound_pos = pos;

	if (sound == (DWORD*)*fullSound || sound == (DWORD*)*jfullSound){
		int current_gear = *(DWORD *)((*(int *)(*(int *)0x6D2098 + 0x268) + 0x10) + 20980);
		//если газ не нажат и передача = нейтральная и обороты примерно холостые
		if (!GetActionState(0) && current_gear == 1 && pitch < 0.3){
			return 0;
		}
		sound = GetNewSound(sound, PlayerVehicle.GetFullSound());
	} else {
		sound = GetNewSound(sound, PlayerVehicle.GetIdleSound());
	}

	if (sound){
		sub_52F780(sound, a2, a3, pos);
		result = sub_5FD580(sound, pitch);
	}
	return result;
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

void AttachHooks(){
    //cout << "Try to attach hooks" << endl;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	//sub_52F750 - sound 2d with pitch
	//sub_52F7D0 - sound 3d with pitch

	DWORD addr_52F750 = 0x52F750;
	DWORD addr_52F7D0 = 0x52F7D0;

    DetourAttach(&(LPVOID&)addr_52F750, &sub_52F750);
	DetourAttach(&(LPVOID&)addr_52F7D0, &sub_52F7D0);
    DetourTransactionCommit();
    //cout << "Hooks attached!" << endl;
	//cout << "SoundMod v17.01.2024 started." << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (GetPrivateProfileIntA("Mod", "Enabled", 0, ".\\KoTR_SoundMod.ini")){
				//DisplayConsole();
				AttachHooks();
				SetTimer(0, 0, 50, (TIMERPROC)OnTimer);
			}

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
