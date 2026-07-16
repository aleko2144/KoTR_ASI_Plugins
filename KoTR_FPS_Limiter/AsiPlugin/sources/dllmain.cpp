#include <windows.h>
#include <iostream>

#include "Utils\CPatch.h"
#include "Utils\ExeUtils.h"

#include "Hooks\FPS_Limiter.h"

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
			LPCSTR configName = ".\\KoTR_FPS_Limiter.ini";

			if (GetPrivateProfileIntA("common", "DisplayConsole", 0, configName)) {
				displayConsole();
				//cout << "HT [Modern] Patch v0.1" << endl;
			}

			gameVersion = GetGameVersion();

			if (!gameVersion) {
				MessageBoxA(0, "Detected game version is not supported! Failed to attach to game. Please report the error to the plugin author.", "FPS Limiter Message", MB_OK);
				return TRUE;
			}
			/*
			else {
				PrintGameVersion(gameVersion);
			}
			*/
			
			if (GetPrivateProfileIntA("COMMON", "LimiterEnabled", 0, configName)) {
				if (FPSLimiter::injectHooks(configName, gameVersion)) {
					cout << "[PATCH]: FPS Limiter injected" << endl;
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