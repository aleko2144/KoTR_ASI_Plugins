#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "dllmain.h"

#include "detours.h"

using namespace std;

bool outputEnabled;

bool GetPrivateProfileBool(char* section, char* variable, char* default_val, char* filename){
	char buf[64];

	GetPrivateProfileStringA(section, variable, default_val, buf, 64, filename);
	cout << variable << "=" << buf << endl;

	if (!strcmp(buf, "off") || !strcmp(buf, "false") || !strcmp(buf, "0"))
		return false;

	return true;
}

float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename){
	float to_return;
	char* returnedString[256];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 256, filename.c_str());
	to_return = atof(returnedString);
	return to_return;
}

//if v10 = 1 then helicopter in VEH_IN_MODE_NOTHING else in VEH_IN_MODE_COPTER
int* add_helicopter(DWORD* TheGame, char* roomName, char* moduleName, float posX, float posY, float posZ, float radius, int v9, int v10, int v11){
	typedef int* (*add_helicopter)(DWORD* TheGame, char* roomName, char* moduleName, float posX, float posY, float posZ, float radius, int v9, int v10, int v11);
	return add_helicopter(0x5687C0)(TheGame, roomName, moduleName, posX, posY, posZ, radius, v9, v10, v11);
}

void load_all_helicopters(DWORD *TheGame){
	int* helicopters_count  =  (int*)0x67B2B8;

	*helicopters_count = 1;
	int iter = 0;


	char section [64];
	char module  [64];
	char room    [64];
	float posX, posY, posZ, radius;

    while(true){
        sprintf(section, "COPTER_%d", iter + 1);

        GetPrivateProfileStringA(section, "module", "none", module, 64, ".\\KoTR_HelicoptersMod.ini");
		GetPrivateProfileStringA(section, "room",   "none", room,   64, ".\\KoTR_HelicoptersMod.ini");
        posX   = GetPrivateProfileFloat(section, "posX",   "0.0", ".\\KoTR_HelicoptersMod.ini");
		posY   = GetPrivateProfileFloat(section, "posY",   "0.0", ".\\KoTR_HelicoptersMod.ini");
		posZ   = GetPrivateProfileFloat(section, "posZ",   "0.0", ".\\KoTR_HelicoptersMod.ini");
		radius = GetPrivateProfileFloat(section, "radius", "0.0", ".\\KoTR_HelicoptersMod.ini");

        if (!strcmp(module, "none") || !strcmp(room, "none") || iter > 128)
            break;

		if (outputEnabled)
			cout << "COPTER_" << iter + 1 << ": pos=" << posX << ";" << posY << ";" << posZ << ", W=" << radius << "; MODULE=" << module << "; ROOM=" << room << endl;

		add_helicopter(TheGame, room, module, posX, posY, posZ, radius, iter, 0, 0);
		
		iter++;
    }
	*helicopters_count = iter + 1;
}

int* SearchResourceSND(char *a1){
	typedef int*(*GetResource)(char *a1);
	int* ret = GetResource(0x5EF380)(a1);
	return ret;
}

int*__cdecl initHelicopters(DWORD *TheGame){
	int helicopters_enabled = *(int*)0x696C7C;
	int* helicopters_count  =  (int*)0x67B2B8;
	int* result = (int*)helicopters_enabled;

	char buf[128];

	*helicopters_count = 0;

	int* shootSound   = (int*)0x6F2C64;
	int* hitgrndSound = (int*)0x6F2C68;

	if (helicopters_enabled){
		load_all_helicopters(TheGame);

		*shootSound   = (int)SearchResourceSND("shootSound");
		*hitgrndSound = (int)SearchResourceSND("hitgrndSound");

	if ( !shootSound || !hitgrndSound )
		sprintf(buf, "initHelicopters() did not find shootSound or hitgrndSound");
	} else {
		result = hitgrndSound;
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

	DWORD addr_568AA0 = 0x568AA0;

	DetourAttach(&(LPVOID&)addr_568AA0, &initHelicopters);

    DetourTransactionCommit();

    //cout << "Hooks attached!" << endl;
	cout << "HelicoptersMod v0.1 (30.08.2025) started." << endl;
}
	
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//if (!GetPrivateProfileIntA("MOD", "Enabled", 0, ".\\KoTR_HelicoptersMod.ini"))
			//if (!GetPrivateProfileBool("COMMON", "enabled", "0", sAbsolutePathBuffer))
			//	return TRUE;

			//if (GetPrivateProfileBool("MOD", "console", "0", ".\\KoTR_HelicoptersMod.ini"))
			//	DisplayConsole();

			//if (GetPrivateProfileBool("MOD", "output", "0", ".\\KoTR_HelicoptersMod.ini"))
			//	outputEnabled = true;

			DisplayConsole();
			outputEnabled = true;
			AttachHooks();

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
