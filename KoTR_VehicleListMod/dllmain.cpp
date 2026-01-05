#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <vector>

#include <iostream>
#include <fstream>
#include <string>

#include <cstdint>

#include "detours.h"
#include "CPatch.h"

#include "dllmain.h"

using namespace std;

const int vehCount    = 27; //27 default 
const int carsStartID = 23; //23 default
const int carsIDCount = 16; //16 default

//mask files prefixes - they are located in common/esc16.res
const char *vehicle_masks_list[vehCount] =
{
	"Daf",
	"Freightliner",
	"International",
	"Kamaz",
	"Kenworth",
	"Mack",
	"Mercedes",
	"Peterbilt",
	"Renault",
	"Scania",
	"Storm",
	"Volvo",
	"Zil",
	"DafR",
	"Sobol",
	"KamazR",
	"MercedesR",
	"RenaultR",
	"ScaniaR",
	"StormR",
	"VolvoR",
	"ZilR",
	"BmwM5",
	"Patrol",
	"Gazelle",
	"Toyota",
	"Cayman",
};

//vehicle ID with color defined by index (BUS_1, BMW_M5_4)
//ID_list length should be = vehCount

const char *vehicle_ID_list[vehCount] =
{
	"DAF_TRACTOR",
	"FREIGHTLINER_TRACTOR",
	"INTERNATIONAL_TRACTOR",
	"KAMAZ_TRACTOR",
	"KENWORTH_TRACTOR",
	"MACK_TRACTOR",
	"MERCEDES_TRACTOR",
	"PETERBILT_TRACTOR",
	"RENAULT_TRACTOR",
	"SCANIA_TRACTOR",
	"URAGAN_TRACTOR",
	"VOLVO_TRACTOR",
	"ZIL_TRACTOR",
	"DAF_LORRY",
	"GAZEL_LORRY",
	"KAMAZ_LORRY",
	"MERCEDES_LORRY",
	"RENAULT_LORRY",
	"SCANIA_LORRY",
	"STORM_LORRY",
	"VOLVO_LORRY",
	"ZIL_LORRY",
	"BMW_M5_",
	"BMW_M5_",
	"BMW_M5_",
	"BMW_M5_",
	"BMW_M5_"
};

//cars ID with color defined by word
//there is special id for every variant
//in previous array there signle record for all colors,
//but in this array single record = single color

const char *cars_ID_list[carsIDCount] =
{
	"JEEP_NISSAN_GREEN",
	"JEEP_NISSAN_BLACK",
	"JEEP_NISSAN_WHITE",
	"JEEP_NISSAN_VIOLET",
	"GAZEL_BLUE_VAN",
	"GAZEL_GREEN_VAN",
	"GAZEL_GRAY_VAN",
	"GAZEL_RED_VAN",
	"JEEP_TOYOTA_GREEN",
	"JEEP_TOYOTA_BLACK",
	"JEEP_TOYOTA_PINK",
	"JEEP_TOYOTA_BROWN",
	"CAYMAN1",
	"CAYMAN2",
	"CAYMAN3",
	"CAYMAN4"
};

void DisplayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//cout << "VehicleListMod v0.1 (25.01.2024) started." << endl;
}

char* off_675C10          = (char*)0x675C10;
char* off_675BA4          = (char*)0x675BA4;
char* selectedF5vehicleID = (char*)0x6CE8D4;

int selected_vehicle_id;

char *__cdecl getVehicleIDforOrder_orig(signed int veh_id, int color_id){
	//cout << "sub_5231A0(" << veh_id << ", " << color_id << ")\n";

	if (veh_id >= carsStartID)
		sprintf(selectedF5vehicleID, "%s", cars_ID_list[(veh_id - carsStartID) * 4 + color_id]);
	else
		sprintf(selectedF5vehicleID, "%s%d", vehicle_ID_list[veh_id], color_id + 1);

	//cout << selectedF5vehicleID << endl;

	selected_vehicle_id = veh_id * 4 + color_id;

	return selectedF5vehicleID;
}


///// debug /////
DWORD* vehicleMasks[vehCount*4];

void *__cdecl GetFromMainPoolByName(const char *a1) {
	typedef void * (_cdecl * GetFromMainPoolByName)(const char *a1);
	return GetFromMainPoolByName(0x5EF380)(a1);
}

void getVehicleMasks(){
	char buf[64];

	int vehIdx = 0;
	int vehColorIdx;

	do {
		vehColorIdx = 0;
		do {
			//texture location in esc16.res
			sprintf(buf, "m16\\%s%d.msk", vehicle_masks_list[vehIdx], ++vehColorIdx);
			vehicleMasks[vehIdx * 4 + (vehColorIdx - 1)] = (DWORD*)GetFromMainPoolByName(buf);
		}
		while ( vehColorIdx < 4 );
		++vehIdx;
	}
	while ( vehIdx < vehCount );
}

// hook of function draws vehicle texture
void __cdecl DrawMask(DWORD *surface, int x, int y, DWORD *mask, int use_transparency){
	typedef void (__cdecl * DrawMask)(DWORD *surface, int x, int y, DWORD *mask, int use_transparency);
	return DrawMask(0x5B8910)(surface, x, y, mask, use_transparency);
}
void __cdecl DrawMask_hook(DWORD* graphicsData, int x, int y, DWORD* mask, int a5)
{
	mask = vehicleMasks[selected_vehicle_id];

	//if there isn't any mask, discard function call
	if (mask > (DWORD*)0x10000)
		DrawMask(graphicsData, x, y, mask, a5);

	return;
}

// hook of F5 radio constructor
int* CMenuInGameRadio_init(DWORD* _this, DWORD* a2, int a3) {
	typedef int* (__thiscall * CMenuInGameRadio_init)(DWORD* _this, DWORD* a2, int a3);
	return CMenuInGameRadio_init(0x523200)(_this, a2, a3);
}
int* __fastcall CMenuInGameRadio_hook(DWORD* CGraphics, DWORD EDX, DWORD* ScreenElement, int a3)
{
	//cout << "CMenuInGameRadio::(" << CGraphics << ", " << ScreenElement << ", " << a3 << ")\n";
	int* result = CMenuInGameRadio_init(CGraphics, ScreenElement, a3);
	getVehicleMasks();
	return result;
}

/////////////////
 
void AttachHooks(){
    //cout << "Try to attach hooks" << endl;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());


	DWORD addr_sub_5231A0 = 0x5231A0;
	DetourAttach(&(LPVOID&)addr_sub_5231A0, &getVehicleIDforOrder_orig);

	//523200 CMenuInGameRadio::CMenuInGameRadio function:
	//673. v144 = off_675B38;
	CPatch::SetPointer(0x52422A, &vehicle_masks_list);
	//691. while ( (signed int)v144 < (signed int)off_675BA4 );
	CPatch::SetPointer(0x52427E, &vehicle_ID_list);

	//F5 vehicles switch lim (27 default)
	CPatch::SetChar(0x5249E9, vehCount);
	CPatch::SetChar(0x524A0B, vehCount);

	//draw vehicle image in F5 radio
	CPatch::RedirectCall(0x52542B, &DrawMask_hook);

	//radio constructor calls
	CPatch::RedirectCall(0x50CD07, &CMenuInGameRadio_hook);
	CPatch::RedirectCall(0x50CD65, &CMenuInGameRadio_hook);
	CPatch::RedirectCall(0x50CDC3, &CMenuInGameRadio_hook);
	CPatch::RedirectCall(0x50CE21, &CMenuInGameRadio_hook);

    DetourTransactionCommit();
	cout << "VehicleListMod v0.1 (01.10.2025) started." << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisplayConsole();
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
