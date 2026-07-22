cmake_minimum_required(VERSION 3.15)
project(KoTR_SpeedFineMod LANGUAGES CXX)

# Builds the 32-bit Windows plugin KoTR_SpeedFineMod.asi.
#
#   Windows / Visual Studio (generates the .vcxproj for you, so there is no
#   hand-written project to trust):
#       cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
#       cmake --build build --config Release
#
#   Windows / Ninja (from an x86 Native Tools prompt):
#       cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
#       cmake --build build
#
# On MSVC the trampolines use inline __asm (dllmain.cpp, #ifdef _MSC_VER); any
# other compiler (MinGW/Clang) has no naked support and uses the GAS stub
# SpeedFineMod_x86.S instead.

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES dllmain.cpp)
if(NOT MSVC)
    enable_language(ASM)
    list(APPEND SOURCES SpeedFineMod_x86.S)
endif()

add_library(KoTR_SpeedFineMod SHARED ${SOURCES})

# Emit "KoTR_SpeedFineMod.asi" (no "lib" prefix, .asi suffix) instead of .dll.
set_target_properties(KoTR_SpeedFineMod PROPERTIES
    PREFIX ""
    OUTPUT_NAME "KoTR_SpeedFineMod"
    SUFFIX ".asi")

target_link_libraries(KoTR_SpeedFineMod PRIVATE psapi)   # GetModuleInformation (sigscan.h)

if(MSVC)
    target_compile_definitions(KoTR_SpeedFineMod PRIVATE _CRT_SECURE_NO_WARNINGS)
    # static CRT -> the .asi has no external VC runtime DLL dependency
    set_property(TARGET KoTR_SpeedFineMod PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    # MinGW/Clang: case-shim for <Windows.h>/<Psapi.h>, and bake in the runtime.
    target_include_directories(KoTR_SpeedFineMod PRIVATE compat)
    target_link_options(KoTR_SpeedFineMod PRIVATE -static -static-libgcc -static-libstdc++ -s)
endif()
