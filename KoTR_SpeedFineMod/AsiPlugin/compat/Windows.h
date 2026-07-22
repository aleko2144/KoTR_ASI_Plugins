// Case-insensitive shim: these sources include <Windows.h> (fine on Windows's
// case-insensitive FS), but MinGW on Linux only ships <windows.h>. This header
// sits on the include path so <Windows.h> resolves here and forwards. Used only
// by the MinGW/Clang build (CMake, non-MSVC branch); an MSVC build never sees it.
#pragma once
#include <windows.h>
