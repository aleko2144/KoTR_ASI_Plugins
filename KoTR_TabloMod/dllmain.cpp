#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <fstream>

#include "dllmain.h"

#include "detours.h"
#include "input.h"

using namespace std;

bool display_tablo = true;
int* tabloTopOffset = (int*)0x672D7C;

DWORD func_addr_569A40 = 0x569A40; //TaskPager::Draw
DWORD func_addr_56AA00 = 0x56AA00; //TaskPager::Process
DWORD func_addr_575680 = 0x575680; //DrawMapAndEtc

////(00569A40)
//void __thiscall TaskPager::Draw(TaskPager *ecx0)
void sub_569A40(DWORD* _this, DWORD EDX){
	return;
}

////(0056AA00)
//void __thiscall TaskPager::Process(TaskPager *this)
void sub_56AA00(DWORD* _this, DWORD EDX){
	return;
}

////(00575680)
//void __cdecl DrawMapAndEtc()
void sub_575680( DWORD EDX){
	return;
}

int switchTabloKey; //G
bool mod_enabled;

void LoadIni(){
	mod_enabled = GetPrivateProfileIntA("MOD", "Enabled", 0, ".\\KoTR_TabloMod.ini");
	switchTabloKey = GetPrivateProfileIntA("KEYBINDINGS", "SwitchTabloKey", 71, ".\\KoTR_TabloMod.ini");
}

void attachHook(){
	//std::cout << "Try to attach hooks" << std::endl;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(LPVOID&)func_addr_569A40, &sub_569A40);
	//DetourAttach(&(LPVOID&)func_addr_56AA00, &sub_56AA00);
	//DetourAttach(&(LPVOID&)func_addr_575680, &sub_575680);

	DetourTransactionCommit();
	//std::cout << "Hooks attached!" << std::endl;
}

void detachHook(){
	//std::cout << "Try to detach hooks" << std::endl;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourDetach(&(LPVOID&)func_addr_569A40, &sub_569A40);
	//DetourDetach(&(LPVOID&)func_addr_56AA00, &sub_56AA00);
	//DetourDetach(&(LPVOID&)func_addr_575680, &sub_575680);

	DetourTransactionCommit();
	//std::cout << "Hooks detached!" << std::endl;
}

void hideTablo(){
	if (*tabloTopOffset == 0)
		return;

	attachHook();
	*tabloTopOffset = 0;
}

void showTablo(){
	if (*tabloTopOffset == 50)
		return;

	detachHook();
	*tabloTopOffset = 50;
}

void setTabloMode(bool mode){
	if (display_tablo){
		showTablo();
	} else {
		hideTablo();
	}
}

void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	if (*(DWORD*)0x6D2098){
		if (IsKeyJustPressed(switchTabloKey)){
			display_tablo = !display_tablo;
		}

		setTabloMode(display_tablo);
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
			LoadIni();

			if (mod_enabled)
				SetTimer(0, 0, 10, (TIMERPROC)OnTimer);

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
