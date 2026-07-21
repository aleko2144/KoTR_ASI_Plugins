#include <windows.h>
#include <iostream>

#include "Utils\CPatch.h"
#include "Utils\ExeUtils.h"

#include "Hooks\IntroVideo.h"

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
			LPCSTR configName = ".\\KoTR_IntroMod.ini";

			if (GetPrivateProfileIntA("MOD", "DisplayConsole", 0, configName)) {
				displayConsole();
			}

			gameVersion = GetGameVersion();

			if (!gameVersion || gameVersion < 80) {
				return TRUE;
			}
			
			if (GetPrivateProfileIntA("MOD", "Enabled", 0, configName)) {
				if (IntroVideo::injectHooks()) {
					cout << "[IntroMod]: hooks injected" << endl;
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