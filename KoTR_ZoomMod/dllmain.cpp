#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include <fstream>
#include "CPatch.h"

#include "dllmain.h"


using namespace std;

//KeyZoomIn = 0x18,
//KeyZoomOut = 0x19,
//KeyViewLeft = 0x1A,
//KeyViewRight = 0x1B

double xpyramid_min = 1.0; //1.2 default
double xpyramid_max = 4.0; //2.0 default


void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	if (*(DWORD*)0x6D2098){
		int cameraMode   = *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
		double* xpyramid = (double*)0x67E180;

		int* xres = (int*)0x696870;
		int* yres = (int*)0x696874;

		float res_aspect = (float)*xres / *yres;
		float fov_aspect = *xpyramid / res_aspect;

		//если вид снаружи
		if (cameraMode)
			return;

		//DWORD* playerVehicle = (DWORD*)(*(DWORD*)0x6F6930);
		//DWORD* playerCar_V = (DWORD *)(playerVehicle[5400]);
		//CVector *velocity = (CVector*)((char*)playerCar_V + 0x40)

		float* fov_x = (float *)(*(DWORD *)0x696CCC + 0x58);
		float* fov_y = (float *)(*(DWORD *)0x696CCC + 0x54);

		*fov_x = *xpyramid;
		*fov_y = fov_aspect;

		//cout << (*(float*)0x696CCC + 0x58) << endl;

		//cout << *fov_x << endl;
		//cout << *fov_y << endl;

		//if (GetGameKeyState(0x18)){ //KeyZoomIn
		//	Vehicle.m_camera_out_z[0] += 0.05 * cam_addval;
		//}

		//if (GetGameKeyState(0x19)){ //KeyZoomOut
		//	Vehicle.m_camera_out_z[0] -= 0.05 * cam_addval;
		//}
	}
}
	
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//DisplayConsole();
			//LoadIni();

			//if (mod_enabled)
			CPatch::SetPointer(0x597BDA, &xpyramid_min);
			CPatch::SetPointer(0x597C5C, &xpyramid_max);

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

	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}
