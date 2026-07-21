#include "IntroVideo.h"

#include "../Utils/ModUtils/Patterns.h"
#include "../Utils/ModUtils/MemoryMgr.h"

int call_addr_SetupSideOffsets; //0x510E62 in 8.2;
int func_addr_SetupSideOffsets; //0x510E80 in 8.2;

int* loadingSaveName_ptr; //0x6F2D38 in 8.2, loading savegame name char array[128] start
int* menuPointer;         //0x6CEC8C in 8.2, pointer to menu object

void __cdecl OnSetupSideOffsetsCall(int a1) {
	//a1=0 - don't play new game intro, a1=1 - play

	bool is_not_loading_savegame = !*loadingSaveName_ptr;
	bool is_not_starting_multipl = !*(int*)((char*)*menuPointer + 0x524);
	bool is_not_playing_record   =  *(int*)((char*)*menuPointer + 0x514) < 2;


	if (is_not_loading_savegame && is_not_starting_multipl && is_not_playing_record)
		a1 = 1;

	//using namespace std;
	//cout << "is_not_loading_savegame=" << dec << is_not_loading_savegame << endl;
	//cout << "is_not_starting_multipl=" << dec << is_not_starting_multipl << endl;
	//cout << "is_not_playing_record  =" << dec << is_not_playing_record << endl;

	((void(__cdecl*)(int))func_addr_SetupSideOffsets)(a1);
}

bool IntroVideo::injectHooks() {
	try {
		using namespace Memory::VP;
		using namespace hook::txn;
		using namespace std;

		call_addr_SetupSideOffsets = (int)pattern("50 B9 ? ? ? ? 89 7C 24 1C A3 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 8B 11 FF 52 24 5F 5E 5B 8B 4C 24 04 64 89 0D ? ? ? ? 83 C4 10 C3 53 E8 ? ? ? ? 8B 4C 24 14 83 C4 04 64 89 0D ? ? ? ? 5F 5E 5B 83 C4 10 C3").get_first(50);
		func_addr_SetupSideOffsets = (int)ReadCallFrom(call_addr_SetupSideOffsets);

		menuPointer = (int*)*(int*)pattern("A1 ? ? ? ? 83 B8 ? ? ? ? ? 75 30 39 B0 ? ? ? ? 74 28 89 B0 ? ? ? ? A1 ? ? ? ? 68 ? ? ? ? 8D 48 40 E8 ? ? ? ? 8B 88 ? ? ? ? 05 ? ? ? ? 83 E1 FD 89 08 8B 0D").get_first(1);
		loadingSaveName_ptr = (int*)*(int*)pattern("3B FB 75 08 88 1D ? ? ? ? EB 20 83 C9 FF 33 C0 F2 AE F7 D1 2B F9 8B C1 8B F7 BF ? ? ? ? C1 E9 02 F3 A5 8B C8 83 E1 03 F3 A4").get_first(6);

		cout << "[IntroMod]: SetupBarOffsets call=" << hex << call_addr_SetupSideOffsets << " func=" << func_addr_SetupSideOffsets << endl;
		cout << "[IntroMod]: saveNamePtr=" << hex << loadingSaveName_ptr << ", menuPointer=" << menuPointer << endl;

		CPatch::RedirectCall(call_addr_SetupSideOffsets, &OnSetupSideOffsetsCall);
	}
	catch (const hook::txn_exception&)
	{
		return 0;
	}
	
}
