#ifndef FPS_LIMITER_H
#define FPS_LIMITER_H

#include "../Utils/CPatch.h"

#include <windows.h>
#include <iostream>

namespace FPSLimiter {
	//void loadConfig();
	double __stdcall OnCallGetTickCount();
	bool injectHooks(LPCSTR configName, int gameVersion);
}

#endif
