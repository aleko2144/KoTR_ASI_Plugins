#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "CPatch.h"

#include "dllmain.h"

using namespace std;

//(005B8910) void __cdecl DrawMask(GraphicsData *a1, int x, int y, Mask *mask, int a5)
void __cdecl DrawMask(int *surface, int x, int y, int *mask, int use_transparency){
	typedef void (__cdecl * DrawMask)(int *surface, int x, int y, int *mask, int use_transparency);
	return DrawMask(0x5B8910)(surface, x, y, mask, use_transparency);
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

//https://docs.godotengine.org/ru/latest/tutorials/math/interpolation.html
float linear_interp(float a, float b, float t){
	return a + (b - a) * t;
}

int random_in_range(int min, int max){
    return rand() % (max - min + min);
}

int *__cdecl sub_5B8F40(int *graphicsData, signed int x, signed int y, int *mask){
	int *result; // eax
	int v5; // ecx

	result = mask;
	if (mask){
		if (mask[7] & 1) {
			result = graphicsData;
			v5 = graphicsData[graphicsData[4] + 6]; //v5 = (char *)a1->lpBackSurface[a1->m_dwCurrentSurface];
			if (v5){
				if ( y < *((unsigned __int16 *)graphicsData + 1) && x < *(unsigned __int16 *)graphicsData && y >= 0 && x >= 0 ){
					//int* surf2 = (int*)graphicsData[24];

					//int* mask2 = (int*)GetResource("txr\glass_1.msk");
					//cout << mask2 << endl;
					//DrawMask(surf2, x, y, mask2, 0);

					//cout << "X=" << x << " Y=" << y << endl;
					result = (int *)(x * *((unsigned __int16 *)graphicsData + 2) + v5 + y * *((unsigned __int16 *)graphicsData + 3));
					*((BYTE *)result + 1) |= 0xF0u; //F0 hex = 240 dec
				}
			}
		}
	}
	return result;
}

void __cdecl sub_5A56B0_hook(int* surface, int x, int y, int* mask){
	DrawMask(surface, x, y, mask, 0);

	//8.2 default rain
	//sub_5B8F40(surface, x, y, mask);

	//srand(GetTickCount());
	//int x_new = random_in_range(0, 255);
	//int y_new = random_in_range(0, 255);
	//DrawMask(surface, x_new, y_new, mask, 0);

	//for (int i = 0; i < 5; i++){
	//	srand(GetTickCount() + x + (i % 2));
	//	x_new = random_in_range(0, 255);
	//	y_new = random_in_range(0, 255);
	//	DrawMask(surface, x_new, y_new, mask, 0);
	//}
}

void __cdecl addr_5BC1E7_hook(void *a1){
  return;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//hook drops draw on glass functions
			CPatch::RedirectJump(0x5B8F40, sub_5A56B0_hook);
			CPatch::RedirectJump(0x5A56B0, sub_5A56B0_hook);

			//resmask::init - disable deleting dataoffset
			//when msk-file is imported from file
			CPatch::RedirectCall(0x5BC1E7, addr_5BC1E7_hook);
			CPatch::Nop(0x5BC1EF, 3);
			
			DisplayConsole();
			cout << "RainEffectMod v0.01 started" << endl;

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