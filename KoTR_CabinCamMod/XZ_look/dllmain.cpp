#include <iostream>
#include "CPatch.h"
#include "dllmain.h"

using namespace std;

int xres, yres;
RECT winRect;

double GetGameTickCount(){
	typedef double (* GetGameTickCount)();
	return GetGameTickCount(0x4E2845)();
}

void getWinRes(){
	HWND winHwnd = *(HWND*)0x6968F4;
	GetWindowRect(winHwnd, &winRect);

	//cout << "rect.bottom=" << winRect.bottom << " top=" << winRect.top << " left=" << winRect.left << " right=" << winRect.right << endl;

	xres = winRect.right - winRect.left;
	yres = winRect.bottom - winRect.top;
}

void __cdecl SetMousePos(LONG a1, LONG a2){
	typedef void (__cdecl * sub_5ACA80)(LONG a1, LONG a2);
	return sub_5ACA80(0x5ACA80)(a1, a2);
}

void __cdecl SetMousePos_hook(LONG a1, LONG a2){
	getWinRes();
	SetMousePos(xres / 2, yres / 2);
	return;
}

void __fastcall SetAppMousePos(int* app, int x, int y){
	typedef void (__thiscall * SetAppMousePos)(int* app, int x, int y);
	return SetAppMousePos(0x5E3040)(app, x, y);
}

void __fastcall SetAppMousePos_hook(int* app, int edx, int x, int y){
	getWinRes();
	SetAppMousePos(app, xres / 2, yres / 2);
	return;
}

float __fastcall CFilter__AddVal(int *CFilter, float addval, char *a3){
	typedef float (__thiscall * CFilter__AddVal)(int* CFilter, float addval, char *a3);
	return CFilter__AddVal(0x4EAD6E)(CFilter, addval, a3);
}

double __fastcall CFilter__Calculate(int *CFilter){
	typedef double (__thiscall * CFilter__Calculate)(int* CFilter);
	return CFilter__Calculate(0x4EB038)(CFilter);
}

//CMatrix *__cdecl SetMatrixRotX(CMatrix *a1, float a2)
int* __cdecl SetMatrixRotX(int *mtx, float angle){
	typedef int* (__cdecl * SetMatrixRotX)(int *mtx, float angle);
	return SetMatrixRotX(0x5AC040)(mtx, angle);
}
int* __cdecl SetMatrixRotY(int *mtx, float angle){
	typedef int* (__cdecl * SetMatrixRotY)(int *mtx, float angle);
	return SetMatrixRotY(0x5ABE60)(mtx, angle);
}
int* __cdecl SetMatrixRotZ(int *mtx, float angle){
	typedef int* (__cdecl * SetMatrixRotZ)(int *mtx, float angle);
	return SetMatrixRotZ(0x5ABF50)(mtx, angle);
}

//CMatrix *__cdecl MatrixMultiply(CMatrix *out, CMatrix *a2, CMatrix *a3)
int* __cdecl MatrixMultiply(int *mtxOut, int *mtx1, int *mtx2){
	typedef int* (__cdecl * SetMatrixRotX)(int *mtxOut, int *mtx1, int *mtx2);
	return SetMatrixRotX(0x5AB9C0)(mtxOut, mtx1, mtx2);
}

float cabCameraRotX;

int* __cdecl SetCabMatrixRotX_hook(int *mtx, float angle){
	//return SetMatrixRotX(mtx, angle);

	//SetMatrixRotZ(mtx, *(float*)0x6D1BC4);

	char mtx1[48];
	char mtx2[48];
	memcpy(mtx2, mtx, 48);

	SetMatrixRotY(mtx, *(float*)0x6D1BC4);
	memcpy(mtx1, mtx, 48);

	SetMatrixRotX((int*)mtx2, angle);

	return MatrixMultiply(mtx, (int*)mtx1, (int*)mtx2);
}

double processCabCameraRotationAxisZ(){
	//Viewer instance
	int* gViewer = (int*)*(int*)0x6D2098;
	//not null if any camera rotation key is pressed
	int  Viewer_dw574 = *(int *)((char*)gViewer + 0x574);

	//CabChan instance
	int* gCabChan = (int*)*(int*)0x696CD0;
	float CabChan_flt13F0 = *(float *)((char*)gCabChan + 0x13F0);
	float CabChan_flt13F4 = *(float *)((char*)gCabChan + 0x13F4);
	float CabChan_flt13F8 = *(float *)((char*)gCabChan + 0x13F8);
	float CabChan_flt1400 = *(float *)((char*)gCabChan + 0x1400);
	float CabChan_flt13FC = *(float *)((char*)gCabChan + 0x13FC);

	//interior camera max rotation angles (specified for each car in tech file)
	float* DriverNeckAngles = (float *)((char*)gCabChan + 0x1404);

	//TheGame instance
	int* gTheGame = (int*)*(int*)0x696CC0;
	double TheGame_dblB90 = *(double *)((char*)gTheGame + 0xB90); //m_dwDeltaTimePassed

	//other variables
	//it still exists in game from 4.0-5.8 and even works !!!
	float mouseSmoothPosX = *(float*)0x6D1BB8;
	float mouseSmoothPosY = *(float*)0x6D1BC4;
	unsigned __int8 v11;
	unsigned __int8 v12;
	//
	float* cabCameraRotZ = (float*)0x696D30;

	double result;

	//function code
	if (!gViewer)
		return 0.0;

	double v17 = GetGameTickCount();
	double v20 = (v17 - CabChan_flt13F0) * 1.6666666;
	double v3 = 1.0;
	
	float v18, v13;
	float v23 = TheGame_dblB90 * 3.141592653589793 / 2.0;
	//float v23 = TheGame_dblB90 * 3.141592653589793;

	//deg * degToRadMutipler = radians//
	float degToRadMutipler = *(float*)0x672D88;

	if ( v20 < 1.0 )
		v3 = (sin((v20 - 0.5) * 3.141592653589793) + 1.0) * 0.5;

	if ( *(int*)0x696D34 && fabs(mouseSmoothPosX) < 0.01 )
		*(int*)0x696D34 = 0;
	
	if ( Viewer_dw574 >= 0 )
	{
		if ( Viewer_dw574 <= 0 )
		{
			double v7 = CabChan_flt13F4 * 1.6666666;
			if ( v7 > 1.0 )
				v7 = 1.0;
			double v8 = CabChan_flt13F8 * 1.6666666;
			if ( v8 > 1.0 )
				v8 = 1.0;
			float v21 = (v17 - CabChan_flt1400) * 3.3333333 + 1.0 - v7;
			float v19 = (v17 - CabChan_flt13FC) * 3.3333333 + 1.0 - v8;
			if ( v19 <= 1.0 || v21 <= 1.0 )
			{
				float v9;
				if ( v21 >= 1.0 ){
					v9 = (sin((v19 - 0.5) * 3.141592653589793) + 1.0)
						 * 0.5
						 * (DriverNeckAngles[3] - DriverNeckAngles[0])
						 + DriverNeckAngles[0];
				} else {
					v9 = (sin((v21 - 0.5) * 3.141592653589793) + 1.0)
						 * 0.5
						 * (DriverNeckAngles[3] - DriverNeckAngles[6])
						 + DriverNeckAngles[6];
				}
				v18 = degToRadMutipler * v9;
			}
			else
			{
				v18 = degToRadMutipler * DriverNeckAngles[3];
			}
			//code from v 5.5 - camera rotation by mouse
			//v18 = v18 * degToRadMutipler;
			if ( !*(int*)0x696D34 )
			{
				if ( v11 | v12 )
					v18 = v18 - degToRadMutipler * DriverNeckAngles[6] * mouseSmoothPosX;
				else
					v18 = v18 + degToRadMutipler * DriverNeckAngles[0] * mouseSmoothPosX;
			}
		}
		else
		{
			v18 = ((DriverNeckAngles[6] - DriverNeckAngles[3]) * v3 + DriverNeckAngles[3]) * degToRadMutipler;
			float v6 = *cabCameraRotZ - v23 * 0.5;
			if ( v18 > v6 ){
				v18 = v6;
			}
		}
	} else {
		v18 = ((DriverNeckAngles[0] - DriverNeckAngles[3]) * v3 + DriverNeckAngles[3]) * degToRadMutipler;
		float v5 = v23 * 0.5 + *cabCameraRotZ;
		if ( v18 < v5 ){
			v18 = v5;
		}
	}

	// camera rotation linked to steering wheel rotation
	// this function introduced into game since 6.x versions

	//if truck.ini -> [CAR] -> turnHead = on
	if (*(int*)0x696C80) {
		int* attachedVehicle = (int*)*(int *)((char*)gViewer + 0x268);

		if (attachedVehicle){
			int* Car_V = (int*)*(int *)((char*)attachedVehicle + 0x5460);
		
			if (Car_V){
				float Car_V_steer = *(float *)((char*)Car_V + 0x3368);
				float Car_V_speed = *(float *)((char*)Car_V + 0x3330);

				float addval;

				if ( -Car_V_steer <= 0.1308997 ) {
					if ( -Car_V_steer >= -0.1308997 )
						addval = 0.0;
					else
						addval = 0.1308997 - Car_V_steer;
				} else {
					addval = -Car_V_steer - 0.1308997;
				}

				
				int* gCabChan_filter12DC = (int*)((char*)gCabChan + 0x12DC);
				
				CFilter__AddVal(gCabChan_filter12DC, addval, "makeWz");
				v13 = CFilter__Calculate(gCabChan_filter12DC) * Car_V_speed * 0.03;
				if ( v13 <= 0.94247782 ) {
					if ( v13 < -0.94247782 )
						v13 = -0.94247782;
				} else {
					v13 = 0.94247782;
				}
				v18 = v13 + v18;
				
			}
		}
	}

	double v14 = degToRadMutipler * DriverNeckAngles[0];
	if ( v18 > v14 ){
		v18 = v14;
	}
	double v15 = degToRadMutipler * DriverNeckAngles[6];
	if ( v18 < v15 ){
		v18 = v15;
	}
	double v16 = v18 - *cabCameraRotZ;
	float  v24 = v23 + v23;
	if ( v16 <= v24 )
	{
		if ( v16 < -v24 ){
			v16 = -v24;
		}
		result = v16 + *cabCameraRotZ;
		*cabCameraRotZ = result;
	}
	else
	{
		result = v24 + *cabCameraRotZ;
		*cabCameraRotZ = result;
	}
	return result;
	//4DEB3D  call    SetMatrixRotX - set interior camera rotation
}

void Viewer__OnKeyPressed(int* viewer, __int16* controllerData){
	typedef void (__thiscall * Viewer__OnKeyPressed)(int* viewer, __int16* controllerData);
	return Viewer__OnKeyPressed(0x532900)(viewer, controllerData);
}

//void __thiscall Viewer::OnKeyPressed(Viewer *this, ControllerData *a2)
void __fastcall Viewer__OnKeyPressed_hook(int* viewer, __int16* controllerData){
	Viewer__OnKeyPressed(viewer, controllerData);

	//532900
	//((void(*)(int*, signed __int16*))0x532900)(viewer, controllerData);

	//((void(*)(int))dw_SetupOffsets[s_eCurrentD2Version])(0);
	//void (*original_sub_57C980)(short) = (void(*)(short))0x57C980;
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
			if (!GetPrivateProfileIntA("mod", "Enabled", 0, ".\\KoTR_CabinCamMod.ini"))
				return TRUE;

			DisplayConsole();

			CPatch::RedirectCall(0x536704, processCabCameraRotationAxisZ);

			CPatch::RedirectCall(0x4DEB3D, SetCabMatrixRotX_hook);

			//WinSetCursorPosition
			//Viewer::Run
			//on view right/left button
			CPatch::RedirectCall(0x5363FA, &SetMousePos_hook);
			//on camera interior enable
			CPatch::RedirectCall(0x53657A, &SetMousePos_hook);
			//on camera exterior enable
			CPatch::RedirectCall(0x536530, &SetMousePos_hook);
			//another call from Viewer func
			CPatch::RedirectCall(0x532C79, &SetMousePos_hook);

			//CWinApp::SetMousePos
			//TheGame::call0x8C -> SetCursorToCenter
			CPatch::RedirectCall(0x4E07FC, &SetAppMousePos_hook);
			//SetCursorToCenter()
			CPatch::RedirectCall(0x50BDF2, &SetAppMousePos_hook);
			//Viewer::Run
			CPatch::RedirectCall(0x5366E4, &SetAppMousePos_hook);
			//func_5E1020
			CPatch::RedirectCall(0x5E165E, &SetAppMousePos_hook);

			//CPatch::RedirectCall(0x4E0C9A, &Viewer__OnKeyPressed_hook);
			//CPatch::RedirectCall(0x4E0CD3, &Viewer__OnKeyPressed_hook);
			//CPatch::RedirectCall(0x50FF55, &Viewer__OnKeyPressed_hook);

			//CPatch::SetPointer(0x532B56, &xres);
			//CPatch::SetPointer(0x532BB3, &yres);

			cout << "CabinCamMod v0.01 (15.01.2026) started." << endl;

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
