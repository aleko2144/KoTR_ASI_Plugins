/*
 * SpeedFineMod trampolines -- GCC/MinGW (i686) build only.
 * Portable equivalents of the __declspec(naked) __asm blocks in dllmain.cpp
 * (guarded by #ifdef _MSC_VER there); x86 GCC has no naked support. An MSVC
 * build never compiles this file -- CMake only adds it for non-MSVC compilers,
 * and build.bat compiles dllmain.cpp alone, both using the MSVC __asm branch.
 *
 * Both stubs re-run a stolen "load the speed limit" sequence with sft_deltaKph
 * (= ToleranceKph - 5) added, so police cars / radar posts only fine above
 * (signposted limit + ToleranceKph). See dllmain.cpp.
 *
 * The C globals sft_* are extern "C" -> underscore-decorated on i686 Windows.
 * `jmp dword ptr [sym]` matches the MSVC `jmp sym` semantics for a data symbol
 * (indirect jump through the variable's stored address).
 */
	.intel_syntax noprefix
	.text

	/* Police car: entered with st0 = player speed (m/s); load the adjusted zone
	   limit (double kph at [esp+0x18]), convert to m/s, resume at the fcompp. */
	.globl	_sft_policeTrigger
_sft_policeTrigger:
	fld	qword ptr [esp + 0x18]
	fadd	qword ptr [_sft_deltaKph]
	fmul	qword ptr [_sft_kph2ms]
	jmp	dword ptr [_sft_policeResume]

	/* Radar post: load the adjusted radar limit (float kph at [edi]), compare
	   against the player speed (double kph at [esp+0x24]), resume at the fnstsw. */
	.globl	_sft_radarTrigger
_sft_radarTrigger:
	fld	dword ptr [edi]
	fadd	qword ptr [_sft_deltaKph]
	fcomp	qword ptr [esp + 0x24]
	jmp	dword ptr [_sft_radarResume]
