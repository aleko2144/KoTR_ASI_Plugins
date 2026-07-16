#include <iostream>

#include "CPatch.h"
#include "CVector.h"

#include "dllmain.h"

using namespace std;

float vec1_coeff, vec2_coeff, speed_min, vec1_roadtrain, vec2_roadtrain;

float GetPrivateProfileFloat(char* selection, char* variableName, char* defaultValue, LPCSTR filename) {
	char buf[256];
	GetPrivateProfileStringA(selection, variableName, defaultValue, buf, sizeof(buf), filename);
	return atof(buf);
}

float clampf(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

float interp(float a, float b, float t) {
	return a + t * (b - a);
}

CVector* __fastcall Car_V__sub_4F3980(int* Car_V, CVector* a2, CVector* a3) {
	typedef CVector* (__thiscall* sub_4F3980)(int* Car_V, CVector* a2, CVector* a3);
	return sub_4F3980(0x4F3980)(Car_V, a2, a3);
}

CVector* __fastcall Car_V__sub_4F3980_hook(int* Car_V, int EDX, CVector* arg0, CVector* arg4) {
	CVector* result = Car_V__sub_4F3980(Car_V, arg0, arg4);

	float speed_ms = *(float*)((char*)Car_V + 0xDC0);
	float interp_coeff = 1.0;

	if (speed_ms > 0.0){
		float sp_coeff = (speed_ms / speed_min);
		interp_coeff = clampf(sp_coeff, 0.0, 1.0);
		//cout << "sp_coeff=" << sp_coeff << endl;
	}

	//cout << "interp_coeff=" << interp_coeff << endl;


	float a2_coeff = 1.0; //traction/steering/brakes vector?
	float a3_coeff = 1.0; //roll resistance vector?

	//check if vehicle type is semitrailer
	int techID = *(int*)((char*)Car_V + 0x8CC);
	int tractorsStartID = *(int*)0x6CE358;
	//int semitrailersStartID = *(int*)0x6CE35C;

	//cout << "techID=" << dec << techID << endl;
	//techId >= g_dwTechSemiTrailersStartId
	//cout << (techID >= semitrailersStartID) << endl;

	//if vehicle is tractor/semitrailer/trailer
	if (techID >= tractorsStartID) {
		a2_coeff = interp(1.0, vec1_roadtrain, interp_coeff);
		a3_coeff = interp(1.0, vec2_roadtrain, interp_coeff);
	} else {
		a2_coeff = interp(1.0, vec1_coeff, interp_coeff);
		a3_coeff = interp(1.0, vec2_coeff, interp_coeff);
	}


	//cout << a3_coeff << endl;

	arg0->x *= a2_coeff; arg0->y *= a2_coeff; arg0->z *= a2_coeff;
	arg4->x *= a3_coeff; arg4->y *= a3_coeff; arg4->z *= a3_coeff;

	return result;
}

void DisplayConsole(){
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
			LPCSTR configName = ".\\KoTR_ClassicPhysics.ini";

			if (!GetPrivateProfileIntA("MOD", "Enabled", 0, configName))
				return TRUE;

			if (GetPrivateProfileIntA("MOD", "DisplayConsole", 0, configName))
				DisplayConsole();

			cout << "ClassicPhysics v0.01 (16.07.2026) started." << endl;

			vec1_coeff = GetPrivateProfileFloat("MOD", "vec1_coeff", "1.0", configName);
			vec2_coeff = GetPrivateProfileFloat("MOD", "vec2_coeff", "1.0", configName);
			speed_min  = GetPrivateProfileFloat("MOD", "speed_min",  "5.5", configName);

			vec1_roadtrain = GetPrivateProfileFloat("MOD", "vec1_roadtrain", "1.0", configName);
			vec2_roadtrain = GetPrivateProfileFloat("MOD", "vec2_roadtrain", "1.0", configName);

			CPatch::RedirectCall(0x4F5D3C, &Car_V__sub_4F3980_hook);
			CPatch::RedirectCall(0x4F8672, &Car_V__sub_4F3980_hook);
			
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