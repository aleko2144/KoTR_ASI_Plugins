#ifndef CAR_V_H
#define CAR_V_H

namespace Car_V {
	void        SetVelocity(int* Car_V, CVector *a2);
	int         SetAngularVelocity(int* Car_V, CVector *a2);
	void		set_hi_fi(int* Car_V, int mode);
	int         sub_4FABB0(int* Car_V, CVector *a2, CVector *a3);
	int         sub_4FEFB0(int* Car_V);
	const char* sub_4D6590(int* Car_V);
	CVector*    get_iW0(int* Car_V, CVector *a2);
	CVector*	get_wV0(int* Car_V, CVector *a2);
	CVector*	get_unk1_v158(int* Car_V, CVector *a2);
	CVector*	get_unk1_v194(int* Car_V, CVector *a2);
	double      get_unk1_v300(int* Car_V);
	int         sub_4FAE40(int* Car_V, float a2);
	CMatrix *   Multiply_unk1_col_Mtx_To_Field20(int* Car_V, CMatrix* a2);
	CMatrix *   Multiply_unk1_Field50_To_col_iMtx(int* Car_V, CMatrix* a2);
	double		getSemiTrailerSaddleLoad(int* Car_V, CVector* a2);
	int			sub_4EBB70(int* Car_V_unk2, CVector *a2, CVector *a3, CVector *a4);
	int			sub_4EBC00(int* Car_V_unk2, CVector *a2, CVector *a3);
}

#endif
