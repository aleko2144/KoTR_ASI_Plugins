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
bool stereo_enabled;

////////////////////////////
DWORD* reverseSignalSoundInt;
DWORD* reverseSignalSoundExt;

int reverse_signal_time = 800;
int reverse_signal_time_passed;
int reverse_signal_time_start;

int air_brakes_time = 20000;
int air_brakes_time_passed;
int air_brakes_time_start;
////////////////////////////

//обходная технология
//float engine_volume = 1.0;

void DisplayScreenMessage(char* text){
	typedef int(*textcall2)(void* a1);
	int ret = textcall2(0x484050)(text); //выводит текст на панель
}

//через раз работает
/*int GetGameKeyState(int key) {
	typedef int (__thiscall * GetGameKeyState)(DWORD* _this, int key);
	return GetGameKeyState(0x530010)((DWORD*)0x6D1DD8, key);
}*/

int GetActionState_(int *a1, int a2){
	int result;
	int to_return;

	result = (int)a1[a2 + 66];

	int result_LOBYTE = LOBYTE(result);

	if (result){
		result_LOBYTE = *(BYTE *)result;
		if ((BYTE)result_LOBYTE){
			return(unsigned __int8)result_LOBYTE;
		}
	}

	result = a1[a2 + 113];
	if ( result && (result_LOBYTE = *(BYTE *)result, (BYTE)result) )
		result = (unsigned __int8)result;
	else
		result = 0;

	return result;
}

bool GetGameKeyState(int a2){
	if (GetActionState_((int *)0x6D1DD8, a2) == 1){
		return true;
	}
	else {
		return false;
	}
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

bool IsGamePaused(){
	return !(*(int *)(*(int *)0x696244 + 16)); //3 - игра не на паузе, 0 - игра на паузе
}

DWORD* SearchRESSound(char *a1){
	typedef DWORD*(*SearchRESSound)(char *a1);
	DWORD* ret = SearchRESSound(0x5EF380)(a1);
	return ret;
}

void PlayWAV(DWORD* sound, float volume){
	if (!sound)
		return;
	typedef int(*PlayResSound)(DWORD* sound, float volume);
	int ret = PlayResSound(0x52F6B0)(sound, volume);
}

/*void PlayWAVLocated(DWORD* sound, float a2, float a3, DWORD *position){
	typedef int(*PlaySoundLocated)(DWORD* sound, float a2, float a3, DWORD *position);
	int ret = PlaySoundLocated(0x52F800)(sound, a2, a3, position); //52F920
}*/

DWORD* GetNewSound(DWORD* old_snd, DWORD* new_snd){
	if (new_snd)
		return new_snd;

	return old_snd;
}

//функции, нужные для перехвата
void sub_4E5251(DWORD* a1, float a2, float a3, float *a4){
	typedef int(*sub_4E5251)(DWORD* a1, float a2, float a3, float *a4);
	int ret = sub_4E5251(0x4E5251)(a1, a2, a3, a4);
}

void sub_5FDAB0(DWORD *a1, float a2, float a3){
	typedef int(*sub_5FDAB0)(DWORD* a1, float a2, float a3);
	int ret = sub_5FDAB0(0x5FDAB0)(a1, a2, a3);
}

/*int *idleSound_std;
int *fullSound_std;
int *jidleSound_std;
int *jfullSound_std;
int *start0Sound_std;
int *stop_engSound_std;
int *startglochSound_std;
int *gearSound_std;
int *pssSound_std;
int *pss1Sound_std;
int *pss2Sound_std;

int *g_idleSound = (int*)0x6D1D20;
int *g_fullSound = (int*)0x6D1D24;
int *g_jidleSound = (int*)0x6D1D28;
int *g_jfullSound = (int*)0x6D1D2C;
int *g_start0_sound = (int*)0x6D1D14;
int *g_stop_eng_sound = (int*)0x6D1D18;
int *g_startgloch_sound = (int*)0x6D1D68;*/

//struct ViewerSounds:
DWORD* idleSound = (DWORD*)0x6D1D20;
DWORD* jidleSound = (DWORD*)0x6D1D28;
DWORD* fullSound = (DWORD*)0x6D1D24;
DWORD* jfullSound = (DWORD*)0x6D1D2C;

DWORD* g_start0Sound = (DWORD*)0x6D1D14;
DWORD* g_stop_engSound = (DWORD*)0x6D1D18;
DWORD* g_startglochSound = (DWORD*)0x6D1D68;

DWORD *gearSound_std;
DWORD *pssSound_std;
//DWORD *pss1Sound_std;
DWORD *pss1Sound_int;
DWORD *pss1Sound_ext;
DWORD *pss2Sound_std;

DWORD* start0Sound_std;
DWORD* stop_engSound_std;
DWORD* startglochSound_std;

DWORD *g_gearSound = (DWORD*)0x6D1D30;
DWORD *g_pssSound  = (DWORD*)0x6D1D5C; //звук "отпускания" тормоза
DWORD *g_pss1Sound = (DWORD*)0x6D1D6C; //звук прокола шин
DWORD *g_pss2Sound = (DWORD*)0x6D1D60; //звук "нажатия" на тормоз

class CVehicle
{
public:
	DWORD* m_offset;        //указатель на машину игрока
	DWORD* m_prev_offset;   //предыдущий указатель на машину
	DWORD* m_task;          //параметры автомобиля
	DWORD* m_Car_V;         //данные машины игрока из tech
	string m_car_prefix;    //префикс автомобиля
	string m_cab_prefix;    //префикс салона
	float m_rpm;
	int m_currentGear;
	int m_vehicleID;        //ID из vehicle.tech
	int m_handbrakeState;
	bool handbrake_prev_state;
	int m_trailerState;
	bool trailer_prev_state;
	//DWORD* sound_pos;       //точка воспроизведения звука (не совпадает с позицией автомобиля игрока)
	int cameraMode;
	///////////////////////////
	string engine_prefix;
	float m_accel_rpm;
	///////////////////////////
	DWORD* accel_snd_ext;
	DWORD* accel_snd_int;
	DWORD* accel_snd_wide;

	DWORD* idle_snd_int;
	DWORD* idle_snd_ext;
	DWORD* idle_snd_wide;

	DWORD* full_snd_int;
	DWORD* full_snd_ext;
	DWORD* full_snd_wide;

	DWORD* start_snd_int;
	DWORD* start_snd_ext;
	DWORD* start_snd_wide;

	DWORD* start_fail_snd_int;
	DWORD* start_fail_snd_ext;
	DWORD* start_fail_snd_wide;

	DWORD* stop_snd_int;
	DWORD* stop_snd_ext;
	DWORD* stop_snd_wide;

	DWORD* trailerAttach_int;
	DWORD* trailerAttach_ext;
	DWORD* trailerDetach_int;
	DWORD* trailerDetach_ext;

	DWORD* handbrake_off_ext;
	DWORD* handbrake_off_int;
	DWORD* handbrake_on_ext;
	DWORD* handbrake_on_int;

	DWORD* brake_pull_snd;
	DWORD* brake_release_snd;
	DWORD* brake_release2_snd;

	DWORD* gear_a_snd;
	DWORD* gear_m_snd;

	bool use_reverse_signal;
	bool simulate_air_brakes;

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

		m_task = (DWORD *)((char*)m_offset + 0x10); //+ 0x10
		m_Car_V = (DWORD *)(m_offset[5400]);
		m_vehicleID = *(int *)(*(int *)0x6D2098 + 104);
		m_car_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2342));
		m_cab_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2322));
		m_rpm = 0;
		m_currentGear = 0;
		m_handbrakeState = *(DWORD *)((char*)m_task + 21004); //0;

		trailer_prev_state = *(DWORD *)((char*)m_Car_V + 0x2778);
		handbrake_prev_state = *(DWORD *)((char*)m_task + 21004);

		//cout << "m_car_prefix=" << m_car_prefix.c_str() << endl;
		//cout << "m_cab_prefix=" << m_cab_prefix.c_str() << endl;

		/////////////////
		engine_prefix  = GetPrivateProfileStr(m_car_prefix, "engine_prefix", m_car_prefix, ".\\KoTR_SoundMod.ini");
		m_accel_rpm = (GetPrivateProfileFloat(m_car_prefix, "accel_rpm", "0", ".\\KoTR_SoundMod.ini") / 100);
		/////////////////
		use_reverse_signal = GetPrivateProfileIntA(m_car_prefix.c_str(), "reverse_signal", 0, ".\\KoTR_SoundMod.ini");
		simulate_air_brakes = GetPrivateProfileIntA(m_car_prefix.c_str(), "simulate_air_brakes", 0, ".\\KoTR_SoundMod.ini");

		//cout << "m_car_prefix=" << m_car_prefix.c_str() << endl;
		//cout << "use_reverse_signal=" << use_reverse_signal << endl;

		char snd_name[255];

		//ускорение
		sprintf(snd_name, "%s_accel_ext", engine_prefix.c_str());
		accel_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_accel_int", engine_prefix.c_str());
		accel_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_accel_ST", engine_prefix.c_str());
		accel_snd_wide = SearchRESSound(snd_name);

		//cout << snd_name << endl;

		//звук холостых оборотов
		sprintf(snd_name, "%s_idle_ext", engine_prefix.c_str());
		idle_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_idle_int", engine_prefix.c_str());
		idle_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_idle_ST", engine_prefix.c_str());
		idle_snd_wide = SearchRESSound(snd_name);

		//звук средних оборотов
		sprintf(snd_name, "%s_full_ext", engine_prefix.c_str());
		full_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_full_int", engine_prefix.c_str());
		full_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_full_ST", engine_prefix.c_str());
		full_snd_wide = SearchRESSound(snd_name);

		//звук start_eng
		sprintf(snd_name, "%s_start_ext", engine_prefix.c_str());
		start_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_start_int", engine_prefix.c_str());
		start_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_start_ST", engine_prefix.c_str());
		start_snd_wide = SearchRESSound(snd_name);

		//звук start_fail
		sprintf(snd_name, "%s_start_fail_ext", engine_prefix.c_str());
		start_fail_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_start_fail_int", engine_prefix.c_str());
		start_fail_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_start_fail_ST", engine_prefix.c_str());
		start_fail_snd_wide = SearchRESSound(snd_name);

		//звук stop_eng
		sprintf(snd_name, "%s_stop_ext", engine_prefix.c_str());
		stop_snd_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_stop_int", engine_prefix.c_str());
		stop_snd_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_stop_ST", engine_prefix.c_str());
		stop_snd_wide = SearchRESSound(snd_name);


		//эффекты
		sprintf(snd_name, "%s_handbrake_off_ext", engine_prefix.c_str());
		handbrake_off_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_handbrake_off_int", engine_prefix.c_str());
		handbrake_off_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_handbrake_on_ext", engine_prefix.c_str());
		handbrake_on_ext = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_handbrake_on_int", engine_prefix.c_str());
		handbrake_on_int = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_brake_pull", engine_prefix.c_str());
		brake_pull_snd = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_brake_release", engine_prefix.c_str());
		brake_release_snd = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_brake_release2", engine_prefix.c_str());
		brake_release2_snd = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_gear_a", engine_prefix.c_str());
		gear_a_snd = SearchRESSound(snd_name);

		sprintf(snd_name, "%s_gear_m", engine_prefix.c_str());
		gear_m_snd = SearchRESSound(snd_name);

		trailerAttach_int = SearchRESSound("trailer_attach_int");
		trailerAttach_ext = SearchRESSound("trailer_attach_ext");
		trailerDetach_int = SearchRESSound("trailer_detach_int");
		trailerDetach_ext = SearchRESSound("trailer_detach_ext");

		gearSound_std = SearchRESSound("gearSound");
		pssSound_std  = SearchRESSound("pssSound");
		//pss1Sound_std = SearchRESSound("pss1Sound");
		pss1Sound_ext = SearchRESSound("pss1ExtSound");
		pss1Sound_int = SearchRESSound("pss1IntSound");
		pss2Sound_std = SearchRESSound("pss2Sound");

		start0Sound_std = SearchRESSound("start0Sound");
		stop_engSound_std = SearchRESSound("stop_engSound");
		startglochSound_std = SearchRESSound("startglochSound");

		reverseSignalSoundInt = SearchRESSound("reverseSignalIntSound");
		reverseSignalSoundExt = SearchRESSound("reverseSignalExtSound");

		//шо-то не то
		/*if (brake_pull_snd){
			*g_pss2Sound = (DWORD)brake_pull_snd;
		} else {
			*g_pss2Sound = (DWORD)pss2Sound_std;
		}

		if (brake_release_snd){
			*g_pssSound = (DWORD)brake_release_snd;
		} else {
			*g_pssSound = (DWORD)pssSound_std;
		}

		if (pss1Sound_std){
			*g_pss1Sound = (DWORD)pss1Sound_std;
		} else {
			*g_pss1Sound = (DWORD)pss2Sound_std;
		}*/

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

	DWORD* GetAccelSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return accel_snd_wide;
			}
			return accel_snd_int;
		}

		return accel_snd_ext;
	}

	void ProcessReverseSignalSound(){
		//cout << "ProcessReverseSignalSound\n";
		if (use_reverse_signal && !m_currentGear && m_rpm){

			//cout << "if (use_reverse_signal && !m_currentGear && m_rpm)\n";

			/*if (cameraMode){
				distance_to_vehicle = GetDistanceBetweenPoints(VehiclePos.x, ViewerPos.x, VehiclePos.y, ViewerPos.y);
				sound_volume = 0.25 * GetSoundLevel(distance_to_vehicle);
			}
			else{
				sound_volume = 0.25;
			}*/

			reverse_signal_time_passed = GetTickCount() - reverse_signal_time_start;

			//cout << "reverse_signal_time_passed=" << reverse_signal_time_passed << endl;
		
			if (abs(reverse_signal_time_passed) >= reverse_signal_time){
				if (cameraMode){
					//cout << "reverseSignalSoundExt\n";
					PlayWAV(reverseSignalSoundExt, GetExternalSoundVolume(0.15)); //0.25
				} else {
					//cout << "reverseSignalSoundInt\n";
					PlayWAV(reverseSignalSoundInt, 0.25);
				}
				reverse_signal_time_start = GetTickCount();
			}
		}
	}

	void ProcessAirBrakes2Sound(){
		if (simulate_air_brakes && m_rpm){
			air_brakes_time_passed = GetTickCount() - air_brakes_time_start;

			//cout << "air_brakes_time_passed=" << air_brakes_time_passed << endl;

			//если ручник или нажат тормоз
			if (m_handbrakeState || GetGameKeyState(3)){
				air_brakes_time_start = GetTickCount();
			}
		
			if (abs(air_brakes_time_passed) >= air_brakes_time){

				//if (brake_release2_snd)
				//	cout << "brake_release2\n";

				if (cameraMode){
					PlayWAV(brake_release2_snd, GetExternalSoundVolume(1.0)); //0.25
				} else {
					PlayWAV(brake_release2_snd, 1.0); //0.25
				}
				air_brakes_time_start = GetTickCount();
			}
		}
	}

	void Process()
	{
		//дописать громкость звуков из настроек игры!!!
		//дописать замену звуков переключения КПП

		m_offset = (DWORD*)(*(DWORD*)0x6F6930);
		cameraMode = *(int *)(*(int *)0x6D2098 + 1400);

		if (!m_offset || (m_offset != m_prev_offset)){
			Reset();
		}

		if (!m_offset){
			return;
		}

		m_rpm = *(float *)((char*)m_offset + 0x51E8) * 0.60000002;


		//m_trailerState = *(DWORD *)(m_Car_V + 0x2778); //10104 //(m_Car_V[2526]);
		//m_handbrakeState = *(DWORD *)(m_task + 21004);

		m_trailerState   = *(DWORD *)((char*)m_Car_V + 0x2778);
		m_handbrakeState = *(DWORD *)((char*)m_task + 21004); //(m_task[5251]);
		m_currentGear = *(DWORD *)((char*)m_task + 20980);

		//cout << "m_handbrakeState=" << m_handbrakeState << endl;
		//cout << "m_trailerState="   << m_trailerState   << endl;

		//KeyBreak = 0x16,
		//KeyLights = 0x17,
		//KeyAttachTrailer = 0x28,

		//gearbox
		/*if (*(int *)0x6F3470){ //если коробка передач в режиме АКПП
			*g_gearSound = (DWORD)GetNewSound(gearSound_std, gear_a_snd);
		} else {
			*g_gearSound = (DWORD)GetNewSound(gearSound_std, gear_m_snd);
		}*/

		//accel
		//ДОБАВИТЬ ГРОМКОСТЬ ИЗ Ф-ИИ IDLE/FULL
		//upd. громкость звука и так есть в PlayWAV (PlaySoundMono)
		if (IsGameKeyJustPressed(0)){

			if (m_rpm && m_rpm <= m_accel_rpm){
				if (cameraMode){
					PlayWAV(GetAccelSound(), GetExternalSoundVolume(0.25));
				} else {
					PlayWAV(GetAccelSound(), 0.45);
				}
			}
		}

		//handbrake
		if (m_handbrakeState != handbrake_prev_state){ //IsGameKeyJustPressed(0x16)
			handbrake_prev_state = m_handbrakeState;

			if (cameraMode){
				if (!m_handbrakeState){
					PlayWAV(handbrake_off_ext, GetExternalSoundVolume(0.1));
					//cout << "handbrake_off_ext\n";
				} else {
					PlayWAV(handbrake_on_ext, GetExternalSoundVolume(0.1));
					//cout << "handbrake_on_ext\n";
				}
			} else {
				if (!m_handbrakeState){
					PlayWAV(handbrake_off_int, 0.25);
					//cout << "handbrake_off_int\n";
				} else {
					PlayWAV(handbrake_on_int, 0.25);
					//cout << "handbrake_on_int\n";
				}
			}
		}

		//*(DWORD*)0x6CED64 - нет ли паузы клавишой PauseBreak
		//*(DWORD*)0x696244 - TheGamePtr
		//TheGame->bGameFlags != 3 -> paused

		//cout << "bGameFlags=" << *(DWORD *)((char*)0x696244 + 0x10) << endl;
		//cout << "bGameFlags=" << *(int *)(*(int *)0x696244 + 16) << endl;
		//*(int *)(*(int *)0x6D2098 + 1400)

		//если нет паузы
		if (!IsGamePaused()){
			ProcessReverseSignalSound();
			ProcessAirBrakes2Sound();
		}

		//trailer
		/*if (m_trailerState != trailer_prev_state){
			trailer_prev_state = m_trailerState;

			if (m_trailerState){
				if (cameraMode){
					PlayWAV(trailerAttach_ext, GetExternalSoundVolume(0.1));
					//cout << "trailerAttach_ext\n";
				} else {
					PlayWAV(trailerAttach_int, 0.25);
					//cout << "trailerAttach_int\n";
				}
				//DisplayScreenMessage("attached");
			} else {
				if (cameraMode){
					PlayWAV(trailerDetach_ext, GetExternalSoundVolume(0.1));
					//cout << "trailerDetach_ext\n";
				} else {
					PlayWAV(trailerDetach_int, 0.25);
					//cout << "trailerDetach_int\n";
				}
				//DisplayScreenMessage("detached");
			}
		}*/

		//KeyEngine
		/*if (IsGameKeyJustPressed(0x29)){
			DWORD* old_snd;

			if (cameraMode){
				if (stereo_enabled){
					old_snd = SearchRESSound("start0Sound");
					*(DWORD*)0x6D1D14 = (DWORD)GetNewSound(old_snd, start_snd_wide);

					old_snd = SearchRESSound("startglochSound");
					*(DWORD*)0x6D1D68 = (DWORD)GetNewSound(old_snd, start_fail_snd_wide);

					old_snd = SearchRESSound("stop_engSound");
					*(DWORD*)0x6D1D18 = (DWORD)GetNewSound(old_snd, stop_snd_wide);
				} else {
					old_snd = SearchRESSound("start0Sound");
					*(DWORD*)0x6D1D14 = (DWORD)GetNewSound(old_snd, start_snd_int);

					old_snd = SearchRESSound("startglochSound");
					*(DWORD*)0x6D1D68 = (DWORD)GetNewSound(old_snd, start_fail_snd_int);

					old_snd = SearchRESSound("stop_engSound");
					*(DWORD*)0x6D1D18 = (DWORD)GetNewSound(old_snd, stop_snd_int);
				}
			} else {
				old_snd = SearchRESSound("start0Sound");
				*(DWORD*)0x6D1D14 = (DWORD)GetNewSound(old_snd, start_snd_ext);

				old_snd = SearchRESSound("startglochSound");
				*(DWORD*)0x6D1D68 = (DWORD)GetNewSound(old_snd, start_fail_snd_ext);

				old_snd = SearchRESSound("stop_engSound");
				*(DWORD*)0x6D1D18 = (DWORD)GetNewSound(old_snd, stop_snd_ext);
			}
		}*/

		//m_rpm = *(float *)(m_offset + 20968) * 0.60000002;
		//m_currentGear = *(DWORD *)(m_task + 20980);
		//m_handbrakeState = *(DWORD *)(m_task + 21004);
	}

	DWORD* GetGearSound(){
		if (*(int *)0x6F3470){ //если коробка передач в режиме АКПП
			return gear_a_snd;
		} else {
			return gear_m_snd;
		}
	}

	DWORD* GetPssSound(){
		return brake_release_snd;
	}

	DWORD* GetPss1Sound(){
		if (!cameraMode){
			return pss1Sound_int;
		}
		return pss1Sound_ext;
		//return pss1Sound_std;
	}

	DWORD* GetPss2Sound(){
		return brake_pull_snd;
	}

	DWORD* GetIdleSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return idle_snd_wide;
			}
			return idle_snd_int;
		}
		return idle_snd_ext;
	}

	DWORD* GetFullSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return full_snd_wide;
			}
			return full_snd_int;
		}
		return full_snd_ext;
	}

	DWORD* GetStartSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return start_snd_wide;
			}
			return start_snd_int;
		}
		return start_snd_ext;
	}

	DWORD* GetStartFailSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return start_fail_snd_wide;
			}
			return start_fail_snd_int;
		}
		return start_fail_snd_ext;
	}

	DWORD* GetStopSound(){
		if (!cameraMode){
			if (stereo_enabled){
				return stop_snd_wide;
			}
			return stop_snd_int;
		}
		return stop_snd_ext;
	}
};

CVehicle PlayerVehicle;

//звуки, воспроизводимые данными функциями:
//gViewerSounds.pss
//gViewerSounds.pss1
//gViewerSounds.pss2
//
//gViewerSounds.kriak
//gViewerSounds.startgloch
//gViewerSounds.start0
//gViewerSounds.gear
//
//громкость accel можно брать из ф-ии воспроизведения idle/full

void captureVehSounds(){
	//тут менять все стандартные звуки на новые
	DWORD* new_snd;
		
	//gear
	new_snd = PlayerVehicle.GetGearSound();

	if (new_snd){
		*g_gearSound = (DWORD)new_snd;
	} else {
		*g_gearSound = (DWORD)gearSound_std;
	}

	//pss - звук отпускания тормозов
	new_snd = PlayerVehicle.GetPssSound();

	if (new_snd){
		*g_pssSound = (DWORD)new_snd;
	} else {
		*g_pssSound = (DWORD)pssSound_std;
	}

	//pss1 - звук прокола шин
	new_snd = PlayerVehicle.GetPss1Sound();

	if (new_snd){
		*g_pss1Sound = (DWORD)new_snd;
	} else {
		*g_pss1Sound = (DWORD)pss2Sound_std;
	}

	//if (new_snd){
	//	*g_pss1Sound = (DWORD)new_snd;
	//} else if (pss1Sound_std){
	//	*g_pss1Sound = (DWORD)pss1Sound_std;
	//} else {
	//	*g_pss1Sound = (DWORD)pss2Sound_std;
	//}

	//pss2 - звук нажатия тормозов
	new_snd = PlayerVehicle.GetPss2Sound();

	if (new_snd){
		*g_pss2Sound = (DWORD)new_snd;
	} else {
		*g_pss2Sound = (DWORD)pss2Sound_std;
	}


	//start
	new_snd = PlayerVehicle.GetStartSound();

	if (new_snd){
		*g_start0Sound = (DWORD)new_snd;
	} else {
		*g_start0Sound = (DWORD)start0Sound_std;
	}

	//startgloch
	new_snd = PlayerVehicle.GetStartFailSound();

	if (new_snd){
		*g_startglochSound = (DWORD)new_snd;
	} else {
		*g_startglochSound = (DWORD)startglochSound_std;
	}

	//stop_eng
	new_snd = PlayerVehicle.GetStopSound();

	if (new_snd){
		*g_stop_engSound = (DWORD)new_snd;
	} else {
		*g_stop_engSound = (DWORD)stop_engSound_std;
	}
}

void OnTimer(HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime)
{
	if (*(DWORD *)0x6D2098) {
		//cout << "aboba\n";
		PlayerVehicle.Process();
		//перехватики
		captureVehSounds();

		//cout << "eng_volume=" << engine_volume << endl;

		/*if ( *(DWORD*)0x6CEC98 ){ //g_MenuInGameSound
			double v2;
			v2 = (double)*(signed __int16 *)(*(DWORD*)0x6CEC98 + 388) * 0.01;
			cout << "all_volume=" << v2 << endl;

			v2 = (double)*(signed __int16 *)(*(DWORD*)0x6CEC98 + 162) * 0.01;
			//*((signed __int16 *)v3 + 162)
			cout << "eng_volume=" << v2 << endl;
		}*/


		//int __thiscall sub_5171D0(ScreenElement **this, int a2)
		//int __thiscall sub_5171D0(_WORD *this, int a2)
		//v3 = this;
		//sub_603B40(aEffects, aEngvol, (signed __int16)v3[162], v2);
	}
}

int GetActionState(int key){
	typedef int(__thiscall *GetActionState)(DWORD *a1, int a2);
	return GetActionState(0x530010)((DWORD *)0x6D1DD8, key);
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
	//обходная технология
	//engine_volume = pitch;
	//
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

//перехваты функций воспроизведения звука
int __cdecl PlaySoundMono(DWORD* a1a, float a2)
{
	DWORD* a1 = a1a;

	double v2; // st7
	int result; // eax
	float a2a; // [esp+8h] [ebp+8h]

	if ( a1 ){
		//cout << "PlaySound -> g_gearSound\n";
		//cout << a1 << endl;
		//cout << g_gearSound << endl;
		//cout << *a1 << endl;
		//cout << *g_gearSound << endl;
		//cout << PlayerVehicle.GetGearSound() << endl;

		//10CD4F20

		if ( *(DWORD*)0x6CEC98 ) //g_MenuInGameSound
			//это громкость двигателя
			v2 = (double)*(signed __int16 *)(*(DWORD*)0x6CEC98 + 388) * 0.01;
		else
			v2 = 1.0;
		a2a = a2 * v2;
		sub_5FDAB0(a1, a2a, a2a);
		//result = CSound::Play((ResSound *)a1, a2a, a2a);
		//result = sub_5FDAB0((_DWORD *)a1, v4, v4);
	}
	return 1; //result
}

void __cdecl PlaySoundAtPos(DWORD *a1, float a2, float a3, float *a4)
{
  double v5; // st6
  float v6; // ST08_4

  if ( a1 )
  {
    if ( *(DWORD*)0x6CEC98 ) //g_MenuInGameSound
	//это громкость двигателя
      v5 = (double)*(signed __int16 *)(*(DWORD*)0x6CEC98 + 388) * 0.01;
    else
      v5 = 1.0;
    v6 = v5 * a3;
    sub_4E5251(a1, a2, v6, a4);
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

void AttachHooks(){
    //cout << "Try to attach hooks" << endl;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	//sub_52F750 - sound 2d with pitch
	//sub_52F7D0 - sound 3d with pitch

	DWORD addr_52F750 = 0x52F750;
	DWORD addr_52F7D0 = 0x52F7D0;
	DWORD addr_52F800 = 0x52F800;
	DWORD addr_52F6B0 = 0x52F6B0;

    DetourAttach(&(LPVOID&)addr_52F750, &sub_52F750);
	DetourAttach(&(LPVOID&)addr_52F7D0, &sub_52F7D0);

	//new
	//DetourAttach(&(LPVOID&)addr_52F800, &PlaySoundAtPos);
	//DetourAttach(&(LPVOID&)addr_52F6B0, &PlaySoundMono);

    DetourTransactionCommit();
    //cout << "Hooks attached!" << endl;
	cout << "SoundMod v1.1 (31.08.2025) started." << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (GetPrivateProfileIntA("Mod", "Enabled", 0, ".\\KoTR_SoundMod.ini")){
				DisplayConsole();
				AttachHooks();
				SetTimer(0, 0, 50, (TIMERPROC)OnTimer);

				stereo_enabled = (bool)GetPrivateProfileIntA("Mod", "stereo_enabled", 0, ".\\KoTR_SoundMod.ini");
				reverse_signal_time = GetPrivateProfileIntA("Mod", "reverse_signal_time", 800, ".\\KoTR_SoundMod.ini");
				air_brakes_time = GetPrivateProfileIntA("Mod", "air_brakes_time", 20000, ".\\KoTR_SoundMod.ini");
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
