#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "dllmain.h"
#include "CPatch.h"
#include "sigscan.h"

using namespace std;

// Configurable no-fine speed tolerance for police cars and radar posts.
//
// The game stores every speed limit as (signposted limit + 5 kph) and the ticket
// UI subtracts 5 back for display -- that hard-coded 5 kph is the stock tolerance.
// Both enforcement triggers compare the player's speed against the STORED limit:
//   police car (sub_4D1360):  fld speed_ms ; fld limit_kph ; fmul [1/3.6] ; fcompp
//   radar post (sub_56FFD0):  fld limit_kph ; fcomp speed_kph
// We steal each trigger's "load the limit" sequence and add (ToleranceKph - 5) to
// the limit before the compare, so a fine starts only above signposted+ToleranceKph.
// The fine-degree brackets (signposted +15/+35) and the ticket display are untouched.

float GetPrivateProfileFloat(const char* selection, const char* varname, const char* default_val, const char* filename){
	char returnedString[256];
	GetPrivateProfileStringA(selection, varname, default_val, returnedString, 256, filename);
	return atof(returnedString);
}

extern "C" { // stuff to build on linux
	DWORD  sft_policeResume;
	DWORD  sft_radarResume;
	double sft_deltaKph = 0.0;         // ToleranceKph - 5 (stored limits already include +5)
	double sft_kph2ms   = 1.0 / 3.6;   // the same conversion the stolen fmul did
}

// Trampolines over the two trigger compares. MSVC: naked + MS inline asm.
// GCC/MinGW (i686, no naked support) uses the identical stubs in SpeedFineMod_x86.S.
#ifdef _MSC_VER
// Police car: entered with st0 = player speed (m/s). Load the zone limit (double
// kph at [esp+0x18]), add the tolerance, convert to m/s, resume at the fcompp.
__declspec(naked) void sft_policeTrigger(){
	__asm {
		fld  qword ptr [esp + 0x18]
		fadd qword ptr [sft_deltaKph]
		fmul qword ptr [sft_kph2ms]
		jmp  sft_policeResume
	}
}

// Radar post: load the radar limit (float kph at [edi]), add the tolerance,
// compare against the player speed (double kph at [esp+0x24]), resume at fnstsw.
__declspec(naked) void sft_radarTrigger(){
	__asm {
		fld   dword ptr [edi]
		fadd  qword ptr [sft_deltaKph]
		fcomp qword ptr [esp + 0x24]
		jmp   sft_radarResume
	}
}
#else
extern "C" void sft_policeTrigger(void);   // in SpeedFineMod_x86.S
extern "C" void sft_radarTrigger(void);
#endif

void InstallSpeedFineTolerance(){
	SigScan sig;

	float tol = GetPrivateProfileFloat("SPEED_FINE", "ToleranceKph", "5.0", ".\\KoTR_SpeedFineMod.ini");
	sft_deltaKph = (double)tol - 5.0;

	// Police car trigger (in sub_4D1360):
	//   fld [ecx+0x332c] ; fld qword [esp+0x18] ; fmul [1/3.6] ; fcompp
	DWORD policeMatch = sig.FindPattern(NULL,
		"\xD9\x81\x2C\x33\x00\x00\xDD\x44\x24\x18\xDC\x0D\x00\x00\x00\x00\xDE\xD9",
		"xxxxxxxxxxxx????xx");
	if (policeMatch){
		DWORD policeHook = policeMatch + 6;        // steal fld+fmul (10 bytes), keep fcompp
		sft_policeResume = policeHook + 10;
		CPatch::Nop(policeHook, 10);
		CPatch::RedirectJump(policeHook, (void*)&sft_policeTrigger);
	}

	// Radar post trigger (in sub_56FFD0):
	//   fstp qword [esp+0x24] ; fld [edi] ; fcomp qword [esp+0x24] ; fnstsw ax
	DWORD radarMatch = sig.FindPattern(NULL,
		"\xDD\x5C\x24\x24\xD9\x07\xDC\x5C\x24\x24\xDF\xE0\xF6\xC4\x01",
		"xxxxxxxxxxxxxxx");
	if (radarMatch){
		DWORD radarHook = radarMatch + 4;          // steal fld+fcomp (6 bytes), keep fnstsw
		sft_radarResume = radarHook + 6;
		CPatch::Nop(radarHook, 6);
		CPatch::RedirectJump(radarHook, (void*)&sft_radarTrigger);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (!GetPrivateProfileIntA("mod", "Enabled", 0, ".\\KoTR_SpeedFineMod.ini"))
				return TRUE;

			InstallSpeedFineTolerance();

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
