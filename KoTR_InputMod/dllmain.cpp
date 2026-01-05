#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "CPatch.h"

#include "dllmain.h"

using namespace std;

int key_modifier; //gas, brake, lights
int key_mod_steer;

bool always_full_gas;

float mouse_steer_coeff;

int mouse_key_accel;
int mouse_key_brakes;

int key_menu_F1;
int key_menu_F2;
int key_menu_F3;
int key_menu_F4;
int key_menu_F5;
int key_screenshot; //F6
int key_fast_save;  //F8
int key_fast_load;  //F9
int key_pause;      //Pause/Break

void DisplayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//cout << "Mod debug window started" << endl;
}

float GetPrivateProfileFloat(char* selection, char* varname, char* default_val, char* filename){
	char buf[256];
	GetPrivateProfileStringA(selection, varname, default_val, buf, 256, filename);
	return atof(buf);
}

bool is_accel_key_pressed(){
	return IsKeyPressed(mouse_key_accel);
}

bool is_brakes_key_pressed(){
	return IsKeyPressed(mouse_key_brakes);
}

bool is_modifier_pressed(){
	bool result = IsKeyPressed(key_modifier);

	if ((is_accel_key_pressed() || is_brakes_key_pressed()) && always_full_gas){
		return !result;
	}

	return result;
}

bool is_steer_modifier_pressed(){
	return IsKeyPressed(key_mod_steer);
}

bool return_null(){
	return 0;
}

int GetGameFlags(){
	return (*(int *)(*(int *)0x696CC0 + 16)); //3 - game is not paused, 0 - game is paused
}

//float __userpurge Car_V::sub_5008E0@<eax>(unsigned __int8 *this@<ecx>)

int _fastcall Car_V__sub_5008E0(int* Car_V){
	typedef int (__thiscall * Car_V__sub_5008E0)(int* Car_V);
	return Car_V__sub_5008E0(0x5008E0)(Car_V);
}

int mouse_x;
int mouse_y;

int _fastcall Car_V__sub_5008E0_hook(int* Car_V){
	int* playerVehicle = (int*)*(int*)0x6F6930;
	int* playerCarV = (int*)*(int*)((char*)playerVehicle + 0x5460);

	if (Car_V != playerCarV)
		return Car_V__sub_5008E0(Car_V);
	
	//switch Car_V->unk2->field_6 to 1
	*(__int16*)((char*)Car_V + 0x2706) = 1;
	*(int*)((char*)Car_V + 0x29E8) = 0;

	//*(int*)0x696894 = 1;

	int win_xres = *(int*)0x69688C;
	int win_yres = *(int*)0x696890;

	int win_x_center = win_xres / 2;
	int win_y_center = win_yres / 2;

	int *win_mouse_x = (int*)0x6968A0;
	int *win_mouse_y = (int*)0x6968A4;

	mouse_x = *win_mouse_x;
	mouse_y = *win_mouse_y;


	float mouse_pos = mouse_steer_coeff * (float)(win_x_center - mouse_x) / win_x_center;

	float steer_sens = *(float*)((char*)Car_V + 0x2724);
	float mouse_linear = ((1.0 - steer_sens) * (mouse_pos * mouse_pos)) + steer_sens;
	float mouse_rot = mouse_pos * mouse_linear;


	if (mouse_rot > 1.0)
		mouse_rot = 1.0;

	if (mouse_rot < -1.0)
		mouse_rot = -1.0;

	//1 - left, -1 - right;
	float* ruleSteering = (float*)((char*)Car_V + 0x29E0);
	*ruleSteering = mouse_rot;
	

	//cout << "r=" << *ruleSteering << endl;
	//cout << "m=" << mouse_rot << endl;

	//set hooks
	CPatch::SetShort(0x500AAF, 10720);
	CPatch::SetChar (0x53B71D,    16);

	int result = Car_V__sub_5008E0(Car_V);

	//remove hooks
	CPatch::SetShort(0x500AAF, 10012);
	CPatch::SetChar (0x53B71D,    28);

	return result;
}

void __cdecl sub_5696A0(){
  *(DWORD *)((char*)*(int*)0x6F3458 + 0x4FC) = 0;
  *(int*)0x696894 = 11;
}

int __fastcall sub_530010(int *InputData, int keyID){
	typedef int (__thiscall * sub_530010)(int *InputData, int keyID);
	return sub_530010(0x530010)(InputData, keyID);
}

int __fastcall sub_530010_hook(int *InputData, int EDX, int keyID){
	return sub_530010(InputData, keyID);
}

int __fastcall sub_530010_F1(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_menu_F1);
}

int __fastcall sub_530010_F2(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_menu_F2);
}

int __fastcall sub_530010_F3(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_menu_F3);
}

int __fastcall sub_530010_F4(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_menu_F4);
}

int __fastcall sub_530010_F5(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_menu_F5);
}

int __fastcall sub_530010_F6(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_screenshot);
}

int __fastcall sub_530010_F8(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_fast_save);
}

int __fastcall sub_530010_F9(int *InputData, int EDX, int keyID){
	return IsKeyPressed(key_fast_load);
}

//void __thiscall CWinApp::DrawFrame(CWinApp *this)
//5E1FB0

void __fastcall sub_5E1FB0(int *CWinApp){
	typedef void (__thiscall * sub_5E1FB0)(int *CWinApp);
	return sub_5E1FB0(0x5E1FB0)(CWinApp);
}

int GetCameraMode(){ //1 - external view, 0 - interior view
	if (*(DWORD *)0x6D2098)
		return *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
	return 0;
}

void __cdecl sub_5ACA80(LONG a1, LONG a2){
	typedef void (__cdecl * sub_5ACA80)(LONG a1, LONG a2);
	return sub_5ACA80(0x5ACA80)(a1, a2);
}

void __cdecl sub_5ACA80_hook(LONG a1, LONG a2){
	return;
	//return sub_5ACA80(a1, a2);
}

int mouse_prev_x;
int mouse_prev_y;
int prev_game_state;

void __fastcall sub_5E1FB0_hook(int *CWinApp, int EDX){
	int game_state = GetGameFlags();

	int *win_mouse_x = (int*)0x6968A0;
	int *win_mouse_y = (int*)0x6968A4;

	if (game_state){
		mouse_prev_x = *win_mouse_x;
		mouse_prev_y = *win_mouse_y;
	}

	sub_5E1FB0(CWinApp);

	if (game_state && prev_game_state != game_state){
		*win_mouse_x = mouse_prev_x;
		*win_mouse_y = mouse_prev_y;
		prev_game_state = game_state;
	}

	//cout << "g=" << GetGameFlags() << endl;
	//cout << "x=" << mouse_prev_x << " y=" << mouse_prev_y << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (!GetPrivateProfileIntA("MOD", "Enabled", 0, ".\\KoTR_InputMod.ini"))
				return TRUE;

			DisplayConsole();
			cout << "InputMod v0.05 (15.12.2025) started." << endl;

			key_modifier    = GetPrivateProfileIntA("MODIFIERS", "ModifierKey", 0x11, ".\\KoTR_InputMod.ini");
			key_mod_steer   = GetPrivateProfileIntA("MODIFIERS", "SteerModKey", 0x11, ".\\KoTR_InputMod.ini");

			if (GetPrivateProfileIntA("MOUSE", "UseForSteering", 0, ".\\KoTR_InputMod.ini")){
				CPatch::RedirectCall(0x4F59A4, &Car_V__sub_5008E0_hook);
				//CPatch::RedirectJump(0x5696A0, &sub_5696A0);

				//CWinApp::DrawFrame
				CPatch::SetPointer(0x64D8D4, &sub_5E1FB0_hook);
				CPatch::SetPointer(0x651A00, &sub_5E1FB0_hook);
				CPatch::SetPointer(0x652708, &sub_5E1FB0_hook);

				//WinSetCursorPosition
				//Viewer::Run
				//on view right/left button
				CPatch::RedirectCall(0x5363FA, &sub_5ACA80_hook);
				//on camera interior enable
				CPatch::RedirectCall(0x53657A, &sub_5ACA80_hook);
				//on camera exterior enable
				CPatch::RedirectCall(0x536530, &sub_5ACA80_hook);

				//Viewer::OnKeyPressed
				CPatch::RedirectCall(0x532C79, &sub_5ACA80_hook);
				//Viewer::attachVehicle
				CPatch::RedirectCall(0x532208, &sub_5ACA80_hook);


				mouse_steer_coeff  = GetPrivateProfileFloat("MOUSE", "steering_coeff", "1.0", ".\\KoTR_InputMod.ini");
				cout << "Mouse is using for steering! (coeff=" << mouse_steer_coeff << ")" << endl;
			}

			if (GetPrivateProfileIntA("MOUSE", "UseForGasBrake", 0, ".\\KoTR_InputMod.ini")){
				mouse_key_accel  = GetPrivateProfileIntA("MOUSE", "MouseKeyAccel",  0x1, ".\\KoTR_InputMod.ini");
				mouse_key_brakes = GetPrivateProfileIntA("MOUSE", "MouseKeyBrakes", 0x2, ".\\KoTR_InputMod.ini");
				always_full_gas  = GetPrivateProfileIntA("MOUSE", "AlwaysFull",       0, ".\\KoTR_InputMod.ini");

				CPatch::SetPointer(0x67879C, &is_accel_key_pressed);
				CPatch::SetPointer(0x6787A0, &is_brakes_key_pressed);

				cout << "Mouse is using for gas/brake!" << endl;
			}
			
			if (GetPrivateProfileIntA("MODIFIERS", "UseModKeys", 0, ".\\KoTR_InputMod.ini")){
				CPatch::SetPointer(0x6787A4, &is_modifier_pressed);
				CPatch::SetPointer(0x6787B0, &is_steer_modifier_pressed);

				if (always_full_gas)
					cout << "Always full gas/brake enabled!" << endl;

				cout << "Using new modifiers keybindings" << endl;
			}

			if (GetPrivateProfileIntA("KEYBINDINGS", "UseModKeys", 0, ".\\KoTR_InputMod.ini")){
				key_menu_F1    = GetPrivateProfileIntA("KEYBINDINGS", "KeyMenuHelp",    0x70, ".\\KoTR_InputMod.ini");
				key_menu_F2    = GetPrivateProfileIntA("KEYBINDINGS", "KeyMenuRating",  0x71, ".\\KoTR_InputMod.ini");
				key_menu_F3    = GetPrivateProfileIntA("KEYBINDINGS", "KeyMenuInfo",    0x72, ".\\KoTR_InputMod.ini");
				key_menu_F4    = GetPrivateProfileIntA("KEYBINDINGS", "KeyMenuCompany", 0x73, ".\\KoTR_InputMod.ini");
				key_menu_F5    = GetPrivateProfileIntA("KEYBINDINGS", "KeyRadio",       0x74, ".\\KoTR_InputMod.ini");
				key_screenshot = GetPrivateProfileIntA("KEYBINDINGS", "KeyScreenshot",  0x75, ".\\KoTR_InputMod.ini");
				key_fast_save  = GetPrivateProfileIntA("KEYBINDINGS", "KeyFastSave",    0x77, ".\\KoTR_InputMod.ini");
				key_fast_load  = GetPrivateProfileIntA("KEYBINDINGS", "KeyFastLoad",    0x78, ".\\KoTR_InputMod.ini");
				key_pause      = GetPrivateProfileIntA("KEYBINDINGS", "KeyPause",       0x13, ".\\KoTR_InputMod.ini");

				CPatch::RedirectCall(0x598045, &sub_530010_F1);
				CPatch::RedirectCall(0x597F37, &sub_530010_F2);
				CPatch::RedirectCall(0x597FE2, &sub_530010_F3);
				CPatch::RedirectCall(0x597E47, &sub_530010_F4);

				CPatch::RedirectCall(0x59813C, &sub_530010_F5);
				CPatch::SetChar(0x5763F3, key_screenshot);

				CPatch::RedirectCall(0x5980DC, &sub_530010_F8);
				CPatch::RedirectCall(0x598106, &sub_530010_F9);

				CPatch::SetChar(0x5E234E, key_pause);

				cout << "Functional keys reassigned" << endl;
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

	return TRUE;
}