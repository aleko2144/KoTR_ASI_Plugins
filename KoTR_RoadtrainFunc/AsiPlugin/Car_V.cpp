#include "CVector.h"
#include "CMatrix.h"
#include "Car_V.h"

void Car_V::SetVelocity(int* Car_V, CVector *a2) {
	typedef void (__thiscall * Car_V__SetVelocity)(int* Car_V, CVector *a2);
	return Car_V__SetVelocity(0x4F1E60)(Car_V, a2);
}

int Car_V::SetAngularVelocity(int* Car_V, CVector *a2) {
	typedef int (__thiscall * Car_V__SetAngularVelocity)(int* Car_V, CVector *a2);
	return Car_V__SetAngularVelocity(0x4F1F80)(Car_V, a2);
}

void Car_V::set_hi_fi(int* Car_V, int mode){
	Car_V[2446] = mode;
}

int Car_V::sub_4FABB0(int* Car_V, CVector *a2, CVector *a3){
	typedef int (__thiscall * Car_V__sub_4FABB0)(int* Car_V, CVector* a2, CVector* a3);
	return Car_V__sub_4FABB0(0x4FABB0)(Car_V, a2, a3);
}

int Car_V::sub_4FEFB0(int* Car_V){
	typedef int (__thiscall * Car_V__sub_4FEFB0)(int* Car_V);
	return Car_V__sub_4FEFB0(0x4FEFB0)(Car_V);
}

const char * Car_V::sub_4D6590(int* Car_V){
	typedef const char * (__thiscall * Car_V__sub_4D6590)(int* Car_V);
	return Car_V__sub_4D6590(0x4D6590)(Car_V);
}

CVector* Car_V::get_iW0(int* Car_V, CVector *a2){
	*a2 = *(CVector*)((char*)Car_V + 0x2224 + 0x134);
	return a2;
}

CVector* Car_V::get_wV0(int* Car_V, CVector *a2){
	*a2 = *(CVector*)((char*)Car_V + 0x2224 + 0x110);
	return a2;
}

CVector* Car_V::get_unk1_v158(int* Car_V, CVector *a2){
	*a2 = *(CVector*)((char*)Car_V + 0x2224 + 0x158);
	return a2;
}

CVector* Car_V::get_unk1_v194(int* Car_V, CVector *a2){
	*a2 = *(CVector*)((char*)Car_V + 0x2224 + 0x194);
	return a2;
}

double Car_V::get_unk1_v300(int* Car_V){
	return *(float*)((char*)Car_V + 0x2224 + 0x300);
}

int Car_V::sub_4FAE40(int* Car_V, float a2){
	typedef int (__thiscall * Car_V__sub_4FAE40)(int* Car_V, float a2);
	return Car_V__sub_4FAE40(0x4FEFB0)(Car_V, a2);
}

CMatrix * Car_V::Multiply_unk1_col_Mtx_To_Field20(int* Car_V, CMatrix* a2){
	typedef CMatrix * (__thiscall * Car_V__sub_4F80F0)(int* Car_V, CMatrix* a2);
	return Car_V__sub_4F80F0(0x4F80F0)(Car_V, a2);
}

CMatrix * Car_V::Multiply_unk1_Field50_To_col_iMtx(int* Car_V, CMatrix* a2){
	typedef CMatrix * (__thiscall * Car_V__sub_4F8120)(int* Car_V, CMatrix* a2);
	return Car_V__sub_4F8120(0x4F8120)(Car_V, a2);
}

double Car_V::getSemiTrailerSaddleLoad(int* Car_V, CVector* a2){
	typedef int (__thiscall * Car_V__sub_4EED10)(int* Car_V, CVector* a2);
	return Car_V__sub_4EED10(0x4EED10)(Car_V, a2);
}

int Car_V::sub_4EBB70(int* Car_V_unk2, CVector *a2, CVector *a3, CVector *a4){
	typedef int (__thiscall * Car_V__sub_4EBB70)(int* Car_V_unk2, CVector *a2, CVector *a3, CVector *a4);
	return Car_V__sub_4EBB70(0x4EBB70)(Car_V_unk2, a2, a3, a4);
}

int Car_V::sub_4EBC00(int* Car_V_unk2, CVector *a2, CVector *a3){
	typedef int (__thiscall * Car_V__sub_4EBC00)(int* Car_V_unk2, CVector *a2, CVector *a3);
	return Car_V__sub_4EBC00(0x4EBC00)(Car_V_unk2, a2, a3);
}