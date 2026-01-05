#include "input.h"

using namespace std;

bool keyStates[255];

bool IsKeyPressed(int key)
{
	return HIBYTE(GetKeyState(key)) == 0xFF;
}

bool IsKeyJustPressed(int key)
{
	if (IsKeyPressed(key)){
		if (!keyStates[key]){
			keyStates[key] = true;
			return true;
		}
	}
	else {
		keyStates[key] = false;
	}
	return false;
}

bool gameKeyStates[44];

int GetGameKeyState(int key) {
	typedef int (__thiscall * GetGameKeyState)(DWORD* _this, int key);
	return GetGameKeyState(0x530010)((DWORD*)0x6D1DD8, key);
}

bool IsGameKeyJustPressed(int key){
	if (GetGameKeyState(key)){
		if (!gameKeyStates[key]){
			gameKeyStates[key] = true;
			return true;
		}
	}
	else {
		gameKeyStates[key] = false;
	}
	return false;
}