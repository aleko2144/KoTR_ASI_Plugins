#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "dllmain.h"

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

void ProcessMod(){
	DWORD *playerVehicle = (DWORD*)(*(DWORD*)0x6F6930);

	if (!playerVehicle)
		return;

	DWORD *SirenState = (DWORD *)((char*)playerVehicle + 0x51BC);

	if (IsGameKeyJustPressed(21)){
		*SirenState = !*SirenState;
	}
}


void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	//*(DWORD*)0x6D2098 - Viewer
	//*(DWORD*)0x6F6930 - playerVehicle

	if (*(DWORD*)0x6D2098) {
		ProcessMod();
	} else {
	}
}
	
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			SetTimer(0, 0, 50, (TIMERPROC)OnTimer);
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
