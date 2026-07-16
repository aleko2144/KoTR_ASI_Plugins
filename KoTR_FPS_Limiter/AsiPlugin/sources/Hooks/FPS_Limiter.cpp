#include "FPS_Limiter.h"

#include "../Utils/ModUtils/Patterns.h"
#include "../Utils/ModUtils/MemoryMgr.h"

int target_fps;
double target_time;
LARGE_INTEGER timeCurrent, timeFrame, frequency, waitStart, waitCurrent;


int func_addr_GetTickCount;
int call_addr_GetTickCount;

double __stdcall FPSLimiter::OnCallGetTickCount() {
	
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	double elapsedMs = (currentTime.QuadPart - timeFrame.QuadPart) * 1000.0 / frequency.QuadPart;

	if (elapsedMs < target_time) {
		double sleepTime = target_time - elapsedMs;

		QueryPerformanceCounter(&waitStart);

		while (true) {
			QueryPerformanceCounter(&waitCurrent);
			double waitedMs = (waitCurrent.QuadPart - waitStart.QuadPart) * 1000.0 / frequency.QuadPart;

			if (waitedMs >= sleepTime) {
				break;
			}

			if (sleepTime - waitedMs > 2.0) {
				Sleep(1);
			}
		}
	}

	QueryPerformanceCounter(&timeFrame);
	

	//return GetTickCount();
	return ((double(__stdcall*)())func_addr_GetTickCount)();
}

bool FPSLimiter::injectHooks(LPCSTR configName, int gameVersion) {
	try {
		using namespace Memory::VP;
		using namespace hook::txn;
		using namespace std;

		target_fps = GetPrivateProfileIntA("COMMON", "target_FPS", 25, configName);
		target_time = 1000.0 / (float)target_fps;

		cout << "[INFO] Target FPS: " << target_fps << endl;

		QueryPerformanceCounter(&timeFrame);
		QueryPerformanceFrequency(&frequency);

		/*
		switch (gameVersion) {
			case 100:
				func_addr_GetTickCount = 0x411EB0;
				call_addr_GetTickCount = 0x41AC41;
				break;
			case 162:
				func_addr_GetTickCount = 0x4231C0;
				call_addr_GetTickCount = 0x42D48C;
				break;
		}
		*/

		//CWinApp::DrawFrame->GetTickCount() (call from 0x5E25CA in 8.2)
		//and the same for HT1
		//GetTickCount func addr is 0x6036B0 in 8.2

		call_addr_GetTickCount = (int)pattern("E8 ? ? ? ? 8A 86 ? ? ? ? DD 5C 24 18 A8 20 75 0B 8B 8E ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? DC 64 24 18 D9 1D ? ? ? ? 8B 8E ? ? ? ? 8B 86 ? ? ? ? 3B C8 74 34 8B 96 ? ? ? ? 8B CE 8B 84 96 ? ? ? ? 50 E8 ? ? ? ? 8B 86").get_first(0);
		func_addr_GetTickCount = (int)ReadCallFrom(call_addr_GetTickCount);

		cout << "GetTickCount call=" << hex << call_addr_GetTickCount << " func addr=" << func_addr_GetTickCount << endl;

		CPatch::RedirectCall(call_addr_GetTickCount, &OnCallGetTickCount);
	}
	catch (const hook::txn_exception&)
	{
		return 0;
	}
}