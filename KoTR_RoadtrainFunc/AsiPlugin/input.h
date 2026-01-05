#ifndef INPUT_H
#define INPUT_H

#include <windows.h>
#include <iostream>

bool IsKeyPressed(int key);
bool IsKeyJustPressed(int key);

int GetGameKeyState(int key);
bool IsGameKeyJustPressed(int key);

#endif
