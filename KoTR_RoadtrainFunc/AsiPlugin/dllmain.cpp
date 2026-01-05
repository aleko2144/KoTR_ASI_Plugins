#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "CPatch.h"
#include "input.h"

#include "CVector.h"
#include "CMatrix.h"
#include "Car_V.h"
#include "Vehicle.h"

#include "dllmain.h"

using namespace std;

char debugText[512];

void __cdecl DrawText2D(int *surface, int x, int y, int *font, char *text, unsigned int color, int isRealColor){
	typedef void (__cdecl * DrawText2D)(int *surface, int x, int y, int *font, char *text, unsigned int color, int isRealColor);
	return DrawText2D(0x5EA110)(surface, x, y, font, text, color, isRealColor);
}

void __cdecl sub_575680(){
	typedef void (__cdecl * sub_575680)();
	return sub_575680(0x575680)();
}

int* whiteFont  = (int*)0x6CED18; //
int* matrix1Fon = (int*)0x6CED44; //white
int* matrix2Fon = (int*)0x6CED4C; //red
int* LargeFont  = (int*)0x6CED48; //
int* menublkFon = (int*)0x6CED50; //black
int* menuFont   = (int*)0x696880; //blue

void drawDebugInfo(int* surface){
	if (!surface)
		return;

	int* font = (int*)*matrix1Fon;

	int win_xres = *(int*)0x69688C;
	int win_yres = *(int*)0x696890;

	DrawText2D(surface, win_xres / 2, win_yres - 128, font, debugText, 0, 0); //text
}

int __cdecl sub_4D2BB0(int *surface){
	typedef int(*sub_4D2BB0)(int *surface);
	return sub_4D2BB0(0x4D2BB0)(surface);
}

int _cdecl sub_4D2BB0_hook(int *surface){
	drawDebugInfo(surface);

	return sub_4D2BB0(surface);
}

bool __fastcall sub_558EB0(int* vehicle){
	typedef bool (__fastcall * sub_558EB0)(int *vehicle);
	return sub_558EB0(0x558EB0)(vehicle);
}

CVector CVector_MultiplyOnMatrix(CVector a1, CMatrix mtx){
	CVector result; 

	result.x = mtx.right.x * a1.x + mtx.right.y * a1.y + mtx.right.z * a1.z;
	result.y = mtx.top.x * a1.x + mtx.top.y * a1.y + mtx.top.z * a1.z;
	result.z = mtx.at.x * a1.x + mtx.at.y * a1.y + mtx.at.z * a1.z;

	return result;	
}

CVector * CVector_MulMatrix_(CVector *out, CVector *a2, CMatrix *a3){
	CVector *v3; // eax
	double v4; // st7
	double v5; // st6
	double v6; // st5
	CVector *result; // eax
	float v8; // [esp+8h] [ebp+8h]

	v3 = a2;
	v4 = a3->at.y * a2->z + a3->top.y * a2->y + a3->right.y * a2->x;
	v8 = a3->at.z * a2->z + a3->top.z * a2->y + a3->right.z * a2->x;
	v5 = a3->at.x * v3->z + a3->top.x * v3->y;
	v6 = v3->x * a3->right.x;
	result = out;
	out->z = v8;
	out->x = v5 + v6;
	out->y = v4;
	return result;
}

CVector CVector_MulMatrix_(CVector *a2, CMatrix *a3){
	CVector *v3; // eax
	double v4; // st7
	double v5; // st6
	double v6; // st5
	CVector result; // eax
	float v8; // [esp+8h] [ebp+8h]

	v3 = a2;
	v4 = a3->at.y * a2->z + a3->top.y * a2->y + a3->right.y * a2->x;
	v8 = a3->at.z * a2->z + a3->top.z * a2->y + a3->right.z * a2->x;
	v5 = a3->at.x * v3->z + a3->top.x * v3->y;
	v6 = v3->x * a3->right.x;

	result.z = v8;
	result.x = v5 + v6;
	result.y = v4;
	return result;
}

//BOOL __thiscall processRoadtrainMovement(Vehicle *this)
/*
bool __fastcall func_558EB0(int* vehicle, int EDX){
	bool result = sub_558EB0(vehicle);

	sprintf(debugText, "debug info");

	return result;
}
*/

CVector tractor_velocity;
CVector tractor_angular_velocity;

CVector trailer_velocity;
CVector trailer_angular_velocity;

bool __fastcall func_558EB0_v2(int* tractor, int EDX){
	int* coupler_state = (int *)((char*)tractor + 21892);
	sprintf(debugText, "coupler_state = %d", *coupler_state);

	return sub_558EB0(tractor);
}

bool __fastcall func_558EB0(int* tractor, int EDX){
	bool result;

	/////

	//sprintf(debugText, "debug info");

	//default vectors
	CVector axisXvector = *(CVector*)0x702660;
	CVector axisYvector = *(CVector*)0x702640;
	CVector axisZvector = *(CVector*)0x702650;
	CVector zeroVector  = *(CVector*)0x702670;

	CMatrix IdentityMatrix = *(CMatrix*)0x702610;

	int* strailer = (int *)tractor[5470];

	int strailerTechID = strailer[2605];
	int tractorTechID  = tractor[2605];

	int* tractorCar_V  = (int *)(tractor[5400]);
	int* strailerCar_V = (int *)(strailer[5400]);

	int* tractorCarVunk2  = (int*)((char*)tractorCar_V + 0x2700);
	int* strailerCarVunk2 = (int*)((char*)strailerCar_V + 0x2700);

	CMatrix playerCar_mtx;
	CMatrix trailerCar_mtx;
		
	//через Car_V
	//CVector* tractorInertia   = (CVector*)(((char*)tractorCar_V) + 0x1F48);
	//float*   tractorMass      = (float*)(((char*)tractorCar_V) + 0x2640);
	//CVector* strailerInertia   = (CVector*)(((char*)strailerCar_V) + 0x1F48);
	//float*   strailerMass      = (float*)(((char*)strailerCar_V) + 0x2640);

	//через Vehicle
	CVector* tractorInertia  = (CVector*)(((char*)tractor) + 0xCE8);
	float*   tractorMass  = (float*)(((char*)tractor) + 0xCD4);

	CVector* strailerInertia  = (CVector*)(((char*)strailer) + 0xCE8);
	float*   strailerMass = (float*)(((char*)strailer) + 0xCD4);

	CVector tractor_iW0, strailer_iW0, tractor_wV0, strailer_wV0;

	//параметры сцепки из vehicle.tech
	CVector tchTractorSaddle     = *((CVector*)((0x697888 + (2416 *  tractorTechID) + 0x7D0))); //lowSaddle
	CVector tchSTrailerSaddle    = *((CVector*)((0x697888 + (2416 * strailerTechID) + 0x7DC))); //topSaddle
	float tchTractorShiftSaddle  = *((float*)((0x697888 + (2416 * tractorTechID) + 0x800)));    //shiftSaddle
	float tchSTrailerShiftSaddle = *((float*)((0x697888 + (2416 * strailerTechID) + 0x800)));

	//шо? каво?
	CMatrix* tractor_m2C4  = (CMatrix*)(((char*)tractor) + 0x2C4);
	CMatrix* strailer_m2C4 = (CMatrix*)(((char*)strailer) + 0x2C4);

	CMatrix v277;
	CVector v278, v279, v286;

	//в оригинальном коде нигде им значения не присваиваются
	unsigned __int8 v59;
	unsigned __int8 v60;
	unsigned __int8 v80;
	unsigned __int8 v81;
	unsigned __int8 v94;
	unsigned __int8 v95;
	unsigned __int8 v116;
	unsigned __int8 v117;
	/////

	float couplerAttachDist = 0.5; //0.5 default

	memcpy(&v277, tractor_m2C4, sizeof(v277));

	v278.x = (v277.at.x * tchTractorSaddle.z + v277.top.x * tchTractorSaddle.y) + (v277.right.x * tchTractorSaddle.x) + v277.pos.x;

	v278.y = v277.top.y * tchTractorSaddle.y // lowSaddle.y
	 	 + v277.right.y * tchTractorSaddle.x // lowSaddle.x
	 	 + v277.at.y    * tchTractorSaddle.z // lowSaddle.z
		 + v277.pos.y;

	v278.z = v277.right.z * tchTractorSaddle.y;
	 	     + v277.at.z  * tchTractorSaddle.x;
	 	     + v277.top.z * tchTractorSaddle.z;
	 	     + v277.pos.z;
	/////
	memcpy(&v277, strailer_m2C4, sizeof(v277));
	
	v286.x = tchTractorShiftSaddle * axisYvector.x;
	v286.y = tchTractorShiftSaddle * axisYvector.y;
	
	float v271 = v286.x + tchSTrailerSaddle.x;
	float v272 = v286.y + tchSTrailerSaddle.y;
	double v10 = tchTractorShiftSaddle * axisYvector.z + tchSTrailerSaddle.z;

	v279.x = v277.right.x * v271 + v10 * v277.at.x + v272 * v277.top.x + v277.pos.x;
	v279.y = v271 * v277.right.y + v10 * v277.at.y + v272 * v277.top.y + v277.pos.y;
	v279.z = v271 * v277.right.z + v10 * v277.at.z + v272 * v277.top.z + v277.pos.z;
	/////
	//зачем, а главное для чего, если результат всё равно не используется? что за лол...
	//CVector::GetNormalized(&v426, &v278);
	//CVector::GetNormalized(&v425, &v279);

	///// debug info /////
	//memcpy(&playerCar_mtx, (const void *)(((char*)tractorCar_V) + 0x10), 0x30u);
	//memcpy(&trailerCar_mtx, (const void *)(((char*)strailerCar_V) + 0x10), 0x30u);

	//sprintf(debugText, "TRACTOR POS = %.3f %.3f %.3f\nv279 = %.3f %.3f %.3f", playerCar_mtx.pos.x, playerCar_mtx.pos.y, playerCar_mtx.pos.z, v279.x, v279.y, v279.z);
	
	sprintf(debugText, "");
	//////////////////////


	int tractorVar  = *(int *)((char*)tractorCar_V + 0x3344) == 0;
	int strailerVar = *(int *)((char*)strailerCar_V + 0x3344) == 0;

	//-1 = attaching now
	// 0 = attached
	// 2 = detaching now

	//int* coupler_state = (int *)((char*)tractor + 21892);
	int* coupler_state = (int *)tractor + 5473;
	//v1[5473] ; *((_DWORD *)v1 + 5473)

	CVector saddle_mass_a2 = axisZvector * -1.0;
	float saddleLoadMass = Car_V::getSemiTrailerSaddleLoad(strailerCar_V, &saddle_mass_a2);
	//cout << "saddle_mass_a2=" << saddle_mass_a2.x << " " << saddle_mass_a2.y << " " << saddle_mass_a2.z << endl;

	if (!tractorVar || !strailerVar || (result = 0, *coupler_state > 0)){
		result = 1; //v261 is result
	}

	int v302 = Car_V::sub_4EBB70(tractorCarVunk2,  &v278, &zeroVector, &zeroVector);
	int v280 = Car_V::sub_4EBB70(strailerCarVunk2, &v279, &zeroVector, &zeroVector);

	int v341 = Car_V::sub_4EBC00(tractorCarVunk2,  &zeroVector, &zeroVector);
	int v345 = Car_V::sub_4EBC00(strailerCarVunk2, &zeroVector, &zeroVector);

	while (tractorVar || strailerVar){
		//memcpy(&playerCar_mtx, (const void *)(((char*)tractorCar_V) + 0x29F0), 0x30u);
		//memcpy(&trailerCar_mtx, (const void *)(((char*)strailerCar_V) + 0x29F0), 0x30u);

		CMatrix v290, v277, v396, v397, tmp_mtx;

		memcpy(&v290, Car_V::Multiply_unk1_col_Mtx_To_Field20(tractorCar_V, &tmp_mtx), sizeof(tmp_mtx));
		memcpy(&v277, Car_V::Multiply_unk1_col_Mtx_To_Field20(strailerCar_V, &tmp_mtx), sizeof(tmp_mtx));
		Car_V::Multiply_unk1_Field50_To_col_iMtx(tractorCar_V, &v396);
		Car_V::Multiply_unk1_Field50_To_col_iMtx(strailerCar_V, &v397);

		float v30 = v290.right.x * v278.x + v290.at.x * v278.z + v290.top.x * v278.y;
		float v283 = v30;
		float v31 = v278.x * v290.right.y + v290.at.y * v278.z + v290.top.y * v278.y;
		float v32 = v31;
		float v284 = v31;
		float v33 = v278.x * v290.right.z + v290.at.z * v278.z + v290.top.z * v278.y;
		float v34 = v33;
		float v285 = v33;
		float v35 = v277.at.x * v279.z + v277.top.x * v279.y + v277.right.x * v279.x;
		float v36 = v35;
		float v287 = v35;
		float v37 = v277.at.y * v279.z + v277.top.y * v279.y + v279.x * v277.right.y;
		float v338 = v37;
		float v288 = v37;
		float v38 = v277.at.z * v279.z + v277.top.z * v279.y + v279.x * v277.right.z;
		
		float v340 = v38;
		float v289 = v38;

		float v303 = v30 + v290.pos.x;
		float v304 = v32 + v290.pos.y;
		float v305 = v34 + v290.pos.z;
		float v309 = v36 + v277.pos.x;
		float v310 = v338 + v277.pos.y;
		float v311 = v340 + v277.pos.z;

		CVector v270;
		v270.x = v303 - v309;
		v270.y = v304 - v310;
		v270.z = v305 - v311;

		Car_V::get_wV0(tractorCar_V,  &tractor_wV0);
		Car_V::get_wV0(strailerCar_V, &strailer_wV0);

		CVector* v40 = Car_V::get_iW0(tractorCar_V, &tractor_iW0);
		float v306 = v290.at.x * v40->z + v290.top.x * v40->y + v290.right.x * v40->x;
		float v307 = v290.at.y * v40->z + v290.top.y * v40->y + v290.right.y * v40->x;
		float v308 = v290.at.z * v40->z + v290.top.z * v40->y + v290.right.z * v40->x;

		CVector* v42 = Car_V::get_iW0(strailerCar_V, &strailer_iW0);
		float v43 = v277.right.x * v42->x + v277.at.x * v42->z + v277.top.x * v42->y;
		float v44 = v277.right.y * v42->x + v277.at.y * v42->z + v277.top.y * v42->y;
		float v45 = v277.right.z * v42->x + v277.at.z * v42->z + v277.top.z * v42->y;

		float v370 = v283 * v308 - v306 * v285;
		float v371 = v306 * v284 - v283 * v307;
		float v342 = tractor_wV0.x + v307 * v285 - v308 * v284;
		float v343 = v370 + tractor_wV0.y;
		float v344 = v371 + tractor_wV0.z;
		float v373 = v44 * v289 - v45 * v288;
		float v374 = v287 * v45 - v43 * v289;
		float v46 = v43 * v288 - v287 * v44 + strailer_wV0.z;
		float v47 = v374 + strailer_wV0.y;
		float v48 = strailer_wV0.x + v373;

		CVector v381;
		v381.setFromArgs(v48, v47, v46);

		float v49 = v344 - v381.z;
		float v50 = v343 - v381.y;
		float v51 = v342 - v381.x;

		CVector v385;
		v385.setFromArgs(v51, v50, v49);

		CVector v275 = v385;
		CVector v293 = v270;
		double v52   = v270.getLength();
		float v258   = v52;

		if ( v52 > 0.0 ) {
			v293.z = v270.z / v258;
			v293.y = v270.y / v258;
			v293.x = v270.x / v258;
		}

		if ( result || *coupler_state == -1 ){
			float v179 = v275.getLength();
			float v260 = v179 / (v179 + 1.0);
			float v180 = 1.0 - v277.top.z;
			if ( v180 < 1.0 )
				v180 = v180 * (v180 * v180);
			
			CVector v269 = zeroVector;

			if ( *coupler_state == -1 ){
				float v182;
				if ( v277.at.z <= 0.0 )
					v182 = 0.0;
				else
					v182 = v277.at.z;
				if ( v258 > 0.0 )
					v182 = (v270.z * 0.3 / v258 + 1.0) * v182;
				float v183 = (v258 + 0.5) * v182 * v180 * saddleLoadMass * 1.1 / (v258 + 1.0);
				float v184 = v183 * axisZvector.z;
				float v185 = v183 * axisZvector.y;
				float v186 = v183 * axisZvector.x;
				CVector v297;
				v297.setFromArgs(v186, v185, v184);

				float v187 = v297.z * v290.at.z + v297.y * v290.top.z + v297.x * v290.right.z;
				float v188 = v297.z * v290.at.y + v297.y * v290.top.y + v297.x * v290.right.y;
				float v189 = v297.x * v290.right.x + v297.z * v290.at.x + v297.y * v290.top.x;
				CVector v346;
				v346.setFromArgs(v189, v188, v187);

				CVector v348 = v346 * -1.0;
				CVector v347 = v348;
				CVector::LinearAdd(&v269, &v347);
				
				float v193 = sqrt(*strailerMass * *tractorMass / (*strailerMass + *tractorMass)) * -150.0;
				float v194 = v193 * v275.z;
				float v195 = v193 * v275.y;
				float v196 = v193 * v275.x;
				CVector v353;
				v353.setFromArgs(v196, v195, v194);

				float v197 = v293.z * saddleLoadMass;
				float v198 = v293.y * saddleLoadMass;
				float v199 = v293.x * saddleLoadMass;
				CVector v351;
				v351.setFromArgs(v199, v198, v197);

				float v200 = v351.z * 3.0;
				float v201 = v351.y * 3.0;
				float v202 = v351.x * 3.0;
				CVector v355;
				v355.setFromArgs(v202, v201, v200);

				float v203 = v353.z - v355.z;
				float v204 = v353.y - v355.y;
				float v205 = v353.x - v355.x;
				CVector v357;
				v357.setFromArgs(v205, v204, v203);

				CVector v359 = v357;
				CVector::LinearAdd(&v269, &v359);

				//if ( v258 < 0.5 ){
				if ( v258 < couplerAttachDist ){
					*coupler_state = 0;
				}
			} else {
				float v206 = v180 * axisZvector.z;
				float v207 = v180 * axisZvector.y;
				float v208 = v180 * axisZvector.x;
				CVector v361;
				v361.setFromArgs(v208, v207, v206);

				float v336 = v361.y;
				float v335 = v361.x;
				float v337 = v361.z;

				CVector v363 = axisYvector * 1.5;
				
				float v212 = v363.z - v337;
				float v213 = v363.y - v336;
				float v214 = v363.x - v335;
				CVector v365;
				v365.setFromArgs(v214, v213, v212);
				
				float v215 = saddleLoadMass * 1.2;
				float v216 = v365.z * v215;
				float v217 = v365.y * v215;
				float v218 = v365.x * v215;
				CVector v300;
				v300.setFromArgs(v218, v217, v216);
				
				float v219 = v300.z * v290.at.z + v300.y * v290.top.z + v300.x * v290.right.z;
				float v220 = v300.z * v290.at.y + v300.y * v290.top.y + v300.x * v290.right.y;
				float v221 = v300.x * v290.right.x + v300.z * v290.at.x + v300.y * v290.top.x;
				CVector v393;
				v393.setFromArgs(v221, v220, v219);

				CVector::LinearAdd(&v269, &v393);

				float v222 = 1.0 - v260 * v260;
				float v223 = v222 * v269.z;
				float v224 = v222 * v269.y;
				float v225 = v269.x * v222;

				CVector v367;
				v367.setFromArgs(v225, v224, v223);
				v269 = v367;
			}
			//vehTractor->task.carData->unk2.field_104[v302] = v269;
			((CVector *)((char*)tractorCar_V + 0x2700 + 0x104))[v302] = v269;

			CVector v369 = v269 * -1.0;
			//vehSTrailer->task.carData->unk2.field_104[LODWORD(v280)] = v369;
			((CVector *)((char*)strailerCar_V + 0x2700 + 0x104))[v280] = v369;
		} else {
			//нормальное движение полуприцепа
			float v56 = sqrt(*strailerMass * *tractorMass / (*strailerMass + *tractorMass));
			float v254 = v56;
			float v259 = v56 * 5000.0; //а что это?
			float v58 = v258;

			if ( !(v59 | v60) )
				v58 = sqrt(v58 * 0.2);

			float v61 = v58 * v259;
			float v62 = v61 * v293.z;
			float v63 = v61 * v293.y;
			float v64 = v293.x * v61;
			CVector v350;
			v350.setFromArgs(v64, v63, v62);

			CVector v282 = v350;

			if ( *coupler_state == -1 ){
				v282 = v282 * 0.2;
				//if ( v258 < 0.5 )
				if ( v258 < couplerAttachDist ){
					*coupler_state = 0;
				}
			}

			float v66 = axisZvector.z * 0.5;
			float v67 = axisZvector.y * 0.5;
			float v68 = axisZvector.x * 0.5;
			CVector v383;
			v383.setFromArgs(v68, v67, v66);

			float v69 = v383.z + v270.z;
			float v70 = v383.y + v270.y;
			float v71 = v383.x + v270.x;
			CVector v400;
			v400.setFromArgs(v71, v70, v69);

			float v72 = v254 * 1000.0; //сопротивление инерции?

			float v295 = axisZvector.y;
			float v294 = axisZvector.x;
			float v296 = axisZvector.z;
			float v73 = v72 * v275.z;
			float v74 = v72 * v275.y;
			float v75 = v72 * v275.x;

			CVector v301;
			v301.setFromArgs(v75, v74, v73);

			if ( v275.x * v270.x + v275.y * v270.y + v275.z * v270.z > 0.0 )
				CVector::GetScaled(&v301, 2.0);
			
			if ( v258 < 0.2 ){
				float v76 = sqrt(v258 * 5.0);
				CVector::GetScaled(&v301, v76);
			}
			float v255 = v301.getLength();

			//а куда? вызов функции есть, а значение никуда не сохраняется
			//CVector::Length(&v282);

			float v77 = v259 + v259;
			if ( v77 < v255 ){
				float v78 = v77 / v255;
				CVector::GetScaled(&v301, v78);
			}
			Car_V::get_unk1_v300(tractorCar_V);
			CVector v267 = v301;
			CVector v265 = v301;

			if ( v258 < 0.1 && !(v80 | v81) )
			{
				float v256 = v267.getLength();
				CVector v291 = zeroVector;
				CVector v380;
				if ( v256 > 0.0 ){
					float v82 = 1.0 / v256;
					v291.z = v82 * v267.z;
					v291.y = v82 * v267.y;
					v291.x = v82 * v267.x;
				}
				CVector v276, v376;
				v376.z = v290.pos.z - v305;
				v376.y = v290.pos.y - v304;
				v376.x = v290.pos.x - v303;

				CVector::GetNormalized(&v276, &v376);
				float v89 = v276.y * v291.x - v291.y * v276.x;
				float v90 = v291.z * v276.x - v276.z * v291.x;
				float v91 = v276.z * v291.y - v276.y * v291.z;
				CVector v356;
				v356.setFromArgs(v91, v90, v89);

				if ( v356.x * v356.x + v356.y * v356.y + v356.z * v356.z < 0.25 )
				{
					if ( v94 | v95 ) {
						v267.x = -v256 * v276.x;
						v267.y = -v256 * v276.y;
						v267.z = -v256 * v276.z;
					} else {
						v267.x = v256 * v276.x;
						v267.y = v256 * v276.y;
						v267.z = v256 * v276.z;
					}
				}
				float v257 = v265.getLength();
				CVector v292 = zeroVector;
				if ( v257 > 0.0 )
				{
					float v104 = 1.0 / v257;
					v292.z = v265.z * v104;
					v292.y = v265.y * v104;
					v292.x = v265.x * v104;
				}
				CVector v274, v362, v382;

				v382.z = v277.pos.z - v311;
				v382.y = v277.pos.y - v310;
				v382.x = v277.pos.x - v309;

				CVector::GetNormalized(&v274, &v382);
				v362.z = v274.y * v292.x - v292.y * v274.x;
				v362.y = v292.z * v274.x - v274.z * v292.x;
				v362.x = v274.z * v292.y - v274.y * v292.z;
				if ( v362.x * v362.x + v362.y * v362.y + v362.z * v362.z < 0.25 )
				{
					if ( v116 | v117 ) {
						v265.x = -v257 * v274.x;
						v265.y = -v257 * v274.y;
						v265.z = -v257 * v274.z;
					} else {
						v265.x = v257 * v274.x;
						v265.y = v257 * v274.y;
						v265.z = v257 * v274.z;
					}
				}
			}
			

			float v126 = v296 + v282.z;
			float v127 = v295 + v282.y;
			float v128 = v294 + v282.x;
			CVector v375;
			v375.setFromArgs(v128, v127, v126);

			float v129 = v375.x;
			float v333 = v375.y;
			float v334 = v375.z;
			float v130 = v296 + v282.z;
			float v131 = v295 + v282.y;
			float v132 = v294 + v282.x;
			CVector v366;
			v366.setFromArgs(v132, v131, v130);

			float v133 = -v366.z;
			float v134 = -v366.y;
			float v135 = -v366.x;
			CVector v377;
			v377.setFromArgs(v135, v134, v133);

			float v271 = v377.x;
			float v272 = v377.y;
			float v273 = v377.z;

			//cout << "coupler_state=" << *coupler_state << endl;

			if ( *coupler_state == -1 ){
				float v137 = 1.2 - v277.top.z;
				float v138 = v296 * v137;
				float v139 = v295 * v137;
				float v140 = v294 * v137;
				CVector v368; 
				v368.setFromArgs(v140, v139, v138);

				CVector v379 = v368;
				CVector v141 = v282 + v265;
				CVector v142 = v141 + v379;

				v129 = v142.x;
				v333 = v142.y;
				v334 = v142.z;

				CVector v143 = v282 + v267;
				CVector v144 = v143 * -1.0;
				v271 = v144.x;
				v272 = v144.y;
				v273 = v144.z;
			}

			CVector v313 = zeroVector;
			CVector v286 = zeroVector;
			CVector v386;

			
			
			CVector::CrossProduct(&v386, &v290.top, &v277.top);
			//v386 = CVector::CrossProduct(v290.top, v277.top);
			
			float v145 = v386.getLength();
			
			//"выпрямитель" сцепки, когда угол между тягачом и прицепом слишком велик
			if ( v145 > 0.5340707511102649 ) {
				float v146 = strailerInertia->z * tractorInertia->z * (v145 - 0.5340707511102649) * v145 * 10.0 / (strailerInertia->z + tractorInertia->z);
				CVector* v147 = CVector::GetScaled(&v386, v146);
				v313.x = v147->x;
				v313.y = v147->y;
				v313.z = v147->z;
			}
			
			//CVector v148 = Car_V::get_iW0(vehTractor->task.carData, &v430);
			//v149 = vehSTrailer->task.carData;

			CVector tmp_tractorIw0, tmp_strailerIw0;
			Car_V::get_iW0(tractorCar_V, &tmp_tractorIw0);
			Car_V::get_iW0(strailerCar_V, &tmp_strailerIw0);

			int v151 = v302;
			
			//string n960
			CVector* v152 = &((CVector *)((char*)tractorCar_V + 0x2700 + 0x104))[v302];
			//CVector* v152 = &vehTractor->task.carData->unk2.field_104[v302];

			//cout << "v302=" << v302 << endl;
			//cout << "v152=" << v152 << endl;

			//float v154 = strailerInertia->y * tractorInertia->y * (strailer_iW0.y - tractor_iW0.y) * 10.0 / (strailerInertia->y + tractorInertia->y);
			float v154 = strailerInertia->y * tractorInertia->y * (tmp_strailerIw0.y - tmp_tractorIw0.y) * 10.0 / (strailerInertia->y + tractorInertia->y);
			v152->x = v271;
			v152->y = v272;
			v152->z = v273;
			float v156 = v154;

			CVector v157 = v267 * -1.0;
			CVector* v158 = &((CVector *)((char*)tractorCar_V + 0x2700 + 0x140))[v151];
			
			v158->x = v157.x;
			v158->y = v157.y;
			v158->z = v157.z;

			int v159 = v341;
			v159 *= 12;

			CVector* v161 = &((CVector *)((char*)strailerCar_V + 0x2700 + 0x104))[v280];
			v161->x = v129;
			v161->y = v333;
			v161->z = v334;

			((CVector *)((char*)strailerCar_V + 0x2700 + 0x140))[v280] = v265; //str 972

			
			CVector v163 = axisYvector * v156;
			CVector v164 = CVector_MulMatrix_(&v313, &v396);
			CVector v165 = v164 + v163;
			CVector* v166 = &((CVector *)((char*)tractorCar_V + 0x2700 + 0x17C))[v159];
			int v167 = 3 * v345;

			v166->x = v165.x;
			v166->y = v165.y;
			v166->z = v165.z;

			v167 *= 4;

			CVector v169 = axisYvector * -v156;
			CVector v170 = v313 * -1.0;
			CVector v171 = CVector_MulMatrix_(&v170, &v397);
			CVector v172 = v171 + v169;
			CVector* v173 = &((CVector *)((char*)strailerCar_V + 0x2700 + 0x17C))[v167];
			v173->x = v172.x;
			v173->y = v172.y;
			v173->z = v172.z;

			CVector v174 = CVector_MulMatrix_(&v286, &v396);
			CVector* v175 = &((CVector *)((char*)tractorCar_V + 0x2700 + 0x1B8))[v159];
			v175->x = v174.x;
			v175->y = v174.y;
			v175->z = v174.z;

			CVector v176 = v286 * -1.0;
			CVector v177 = CVector_MulMatrix_(&v176, &v397);
			CVector* v178 = &((CVector *)((char*)strailerCar_V + 0x2700 + 0x1B8))[v167];
			v178->x = v177.x;
			v178->y = v177.y;
			v178->z = v177.z;
			if ( v258 > 5.0 ){
				result = 1;
			}

		}

		//* debug information *//
		sprintf(debugText, "v258 = %.3f", v258);
		sprintf(debugText, "%s\ncoupler_state = %d", debugText, *coupler_state);

		switch (*coupler_state){
			case -1:
				sprintf(debugText, "%s (attaching trailer to tractor)", debugText);
				break;
			case 0:
				sprintf(debugText, "%s (normal, attached)", debugText);
				break;
			case 2:
				sprintf(debugText, "%s (detaching trailer from tractor)", debugText);
				break;
		}
		/////////////////////////

		if (tractorVar){
			tractorVar = Car_V::sub_4FEFB0(tractorCar_V);
		} else if ( *(int *)((char*)tractorCar_V + 0x3344) ) {
			float v300 = Car_V::get_unk1_v300(strailerCar_V);
			Car_V::sub_4FAE40(tractorCar_V, v300);
		}

		if (strailerVar){
			strailerVar = Car_V::sub_4FEFB0(strailerCar_V);
		} else if ( *(int *)((char*)strailerCar_V + 0x3344) ) {
			float v300 = Car_V::get_unk1_v300(tractorCar_V);
			Car_V::sub_4FAE40(strailerCar_V, v300);
		}

		//не знаю, в чём разница, но по коду так сделано, пускай будет и здесь
		if ( result || *coupler_state == -1 ){
			CVector trc_a2, trc_a3;
			CVector trl_a2, trl_a3;
			CVector fd_a2, ld_a2;

			Car_V::get_unk1_v158(tractorCar_V, &fd_a2);
			Car_V::get_unk1_v194(tractorCar_V, &ld_a2);

			CVector::FloatDivide (&trc_a3, &fd_a2, *tractorMass);
			CVector::LinearDivide(&trc_a2, &ld_a2, tractorInertia);
			Car_V::sub_4FABB0(tractorCar_V, &trc_a3, &trc_a2);


			Car_V::get_unk1_v158(strailerCar_V, &fd_a2);
			Car_V::get_unk1_v194(strailerCar_V, &ld_a2);

			CVector::FloatDivide (&trl_a3, &fd_a2, *strailerMass);
			CVector::LinearDivide(&trl_a2, &ld_a2, strailerInertia);
			Car_V::sub_4FABB0(strailerCar_V, &trl_a3, &trl_a2);
		} else {
			if ( !(tractorVar <= 1 && tractorVar != -1) || !(strailerVar <= 1 && strailerVar != -1)){
				CVector trc_a2, trc_a3;
				CVector trl_a2, trl_a3;
				CVector fd_a2, ld_a2;

				Car_V::get_unk1_v158(tractorCar_V, &fd_a2);
				Car_V::get_unk1_v194(tractorCar_V, &ld_a2);

				CVector::FloatDivide (&trc_a3, &fd_a2, *tractorMass);
				CVector::LinearDivide(&trc_a2, &ld_a2, tractorInertia);
				Car_V::sub_4FABB0(tractorCar_V, &trc_a3, &trc_a2);


				Car_V::get_unk1_v158(strailerCar_V, &fd_a2);
				Car_V::get_unk1_v194(strailerCar_V, &ld_a2);

				CVector::FloatDivide (&trl_a3, &fd_a2, *strailerMass);
				CVector::LinearDivide(&trl_a2, &ld_a2, strailerInertia);
				Car_V::sub_4FABB0(strailerCar_V, &trl_a3, &trl_a2);
			}


			//*(CVector*)(((char*)strailerCar_V) + 0x2A50) = *(CVector*)(((char*)tractorCar_V) + 0x2A50);
			//*(CVector*)(((char*)strailerCar_V) + 0x2A74) = *(CVector*)(((char*)tractorCar_V) + 0x2A74);


			////////////////////////
		}
			
		int v242 = *(int *)((char*)tractorCar_V + 0x3344);
		int v243 = *(int *)((char*)strailerCar_V + 0x3344);

		if ( v242 || v243 ){
			result = 1;
		} else {
			result = 0;

			if ( tractorVar || strailerVar ) {
				//кстати так немного свободнее рулится
				//memcpy(&playerCar_mtx, (const void *)(((char*)tractorCar_V) + 0x29F0), 0x30u);
				//memcpy(&trailerCar_mtx, (const void *)(((char*)strailerCar_V) + 0x29F0), 0x30u);

				Car_V::Multiply_unk1_col_Mtx_To_Field20(tractorCar_V, &playerCar_mtx);
				Car_V::Multiply_unk1_col_Mtx_To_Field20(strailerCar_V, &trailerCar_mtx);

				result = UnTangleSemiTrailer(tractor, &playerCar_mtx, &trailerCar_mtx) != 0;
				Vehicle::sub_55B520(tractor, &playerCar_mtx, &trailerCar_mtx);
			}
		}

		//* debug info print *//
		/*
		tractor_velocity = *(CVector*)(((char*)tractorCar_V) + 0x2A50);
		tractor_angular_velocity = *(CVector*)(((char*)tractorCar_V) + 0x2A74);

		trailer_velocity = *(CVector*)(((char*)strailerCar_V) + 0x2A50);
		trailer_angular_velocity = *(CVector*)(((char*)strailerCar_V) + 0x2A74);

		sprintf(debugText, "%s\n\nTRACTOR INFO:\nvelocity = %.3f %.3f %.3f\nangular  = %.3f %.3f %.3f\nI = %.3f %.3f %.3f\nmass = %.3f", debugText, tractor_velocity.x, tractor_velocity.y, tractor_velocity.z, tractor_angular_velocity.x, tractor_angular_velocity.y, tractor_angular_velocity.z, tractorInertia->x, tractorInertia->y, tractorInertia->z, *tractorMass);
		sprintf(debugText, "%s\n\nTRAILER INFO:\nvelocity = %.3f %.3f %.3f\nangular  = %.3f %.3f %.3f\nI = %.3f %.3f %.3f\nmass = %.3f", debugText, trailer_velocity.x, trailer_velocity.y, trailer_velocity.z, trailer_angular_velocity.x, trailer_angular_velocity.y, trailer_angular_velocity.z, strailerInertia->x, strailerInertia->y, strailerInertia->z, *strailerMass);
			

		CVector iW0;
		Car_V::get_iW0(tractorCar_V, &iW0);

		CVector wV0;
		Car_V::get_wV0(tractorCar_V, &wV0);

		sprintf(debugText, "%s\n\nTRACTOR iW0 = %.3f %.3f %.3f", debugText, iW0.x, iW0.y, iW0.z);
		sprintf(debugText, "%s\nTRACTOR wV0 = %.3f %.3f %.3f", debugText, wV0.x, wV0.y, wV0.z);
		sprintf(debugText, "%s\n\nsemitrailer_saddle_load = %.3f", debugText, saddleLoadMass);
		*/
		//sprintf(debugText, "v258 = %.3f", debugText, v258);
		//sprintf(debugText, "%s\ncoupler_state = %d", debugText, *coupler_state);
		////////////////////////
	}

	return result != 0;
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

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (!GetPrivateProfileIntA("MOD", "Enabled", 0, ".\\KoTR_PhysicsMod.ini"))
				return TRUE;

			DisplayConsole();
			cout << "PhysicsMod_v2 v0.00 (29.12.2025) started." << endl;

			//roadtrain movement function
			CPatch::RedirectCall(0x55D91C, &func_558EB0);
			//CPatch::RedirectCall(0x55D91C, &func_558EB0_v2);

			if (GetPrivateProfileIntA("DEBUG", "displayInfo", 0, ".\\KoTR_PhysicsMod.ini"))
				CPatch::RedirectCall(0x5756BE, &sub_4D2BB0_hook);

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
