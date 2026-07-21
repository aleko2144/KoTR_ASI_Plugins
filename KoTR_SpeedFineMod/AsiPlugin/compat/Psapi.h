// Case shim for MinGW/Linux: sigscan.h includes <Psapi.h>; MinGW ships <psapi.h>.
#pragma once
#include <windows.h>
#include <psapi.h>
