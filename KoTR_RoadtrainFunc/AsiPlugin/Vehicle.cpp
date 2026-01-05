#include "CMatrix.h"
#include "Vehicle.h"

int UnTangleSemiTrailer(int* veh, CMatrix* matrix1, CMatrix* matrix2) {
	typedef int (__thiscall * sub_55B680)(int* veh, CMatrix* matrix1, CMatrix* matrix2);
	return sub_55B680(0x55B680)(veh, matrix1, matrix2);
}

int Vehicle::sub_55B520(int* veh, CMatrix *a2, CMatrix *a3){
	typedef int (__thiscall * sub_55B520)(int* veh, CMatrix *a2, CMatrix *a3);
	return sub_55B520(0x55B520)(veh, a2, a3);
}