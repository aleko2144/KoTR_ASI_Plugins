#include <windows.h>
#include <iostream>

#include "Utils\CPatch.h"
#include "Utils\ExeUtils.h"

#include "Hooks\FPS_Limiter.h"
#include "Hooks\D3D_Hooks.h"

#include "dllmain.h"

using namespace std;

int gameVersion;

void displayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			LPCSTR configName = ".\\HT_ModernPatch.ini";

			if (GetPrivateProfileIntA("common", "DisplayConsole", 0, configName)) {
				displayConsole();
				cout << "HT Modern Patch v0.1" << endl;
			}

			gameVersion = GetGameVersion();

			if (gameVersion == -1) {
				MessageBoxA(0, "Detected game version is not supported! Failed to attach to game. Please report the error to the patch author.", "Patch Message", MB_OK);
				return TRUE;
			} else {
				PrintGameVersion(gameVersion);
			}
			
			if (GetPrivateProfileIntA("patches", "FPSLimiter", 0, configName)) {
				if (FPSLimiter::injectHooks(configName, gameVersion)) {
					cout << "[PATCH]: injected FPS limiter" << endl;
				}
			}

			if (GetPrivateProfileIntA("patches", "D3DHooks", 0, configName)) {
				if (D3DHooks::injectHooks(gameVersion)) {
					cout << "[PATCH]: injected D3D hooks" << endl;
				}
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