/********************************************************************************************************

Authors:		(c) 2023 Maths Town

Licence:		The MIT License

*********************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
********************************************************************************************************

Basic SIMD Types for 32-bit Unsigned Integers:

FallbackUint64		- Works on all build modes and CPUs.  Forwards most requests to the standard library.

Simd128Uint64		- x86_64 Microarchitecture Level 1 - Works on all x86_64 CPUs.
					- Requires SSE and SSE2 support.  Will use SSE4.1 instructions when __SSE4_1__ or __AVX__ defined.

Simd256Uint64		- x86_64 Microarchitecture Level 3.
					- Requires AVX, AVX2 and FMA support.

Simd512Uint64		- x86_64 Microarchitecture Level 4.
					- Requires AVX512F, AVX512DQ, ACX512VL, AVX512CD, AVX512BW

SimdNativeUint64	- A Typedef referring to one of the above types.  Chosen based on compiler support/mode.
					- Just use this type in your code if you are building for a specific platform.


Checking CPU Support:
Unless you are using a SimdNative typedef, you must check for CPU support before using any of these types.

Types reqpresenting floats, doubles, ints, longs etc are arranged in microarchitecture level groups.
Generally CPUs have more SIMD support for floats than ints (and 32 bit is better than 64-bit).
Ensure the CPU supports the full "level" if you need to use more than one type.


To check support at compile time:
	- Use compiler_level_supported()
	- If you won't use any of the type conversion functions you can use compiler_supported()

To check support at run time:
	- Use cpu_level_supported()
	- If you won't use any of the type conversion functions you can use cpu_supported()

Runtime detection notes:
Visual studio will support compiling all types and switching at runtime. However this often results in slower
code than compiling with full compiler support.  Visual studio will optimise AVX code well when build support is enabled.
If you are able, I recommend distributing code at different support levels. (1,3,4). Let the user choose which to download,
or your installer can make the switch.  It is also possible to dynamically load different .dlls

WASM Support:
I've included FallbackFloat32 for use with Emscripen, but use SimdNativeFloat32 as SIMD support will be added soon.


*********************************************************************************************************/
#pragma once


#include <stdint.h>
#include "simd-cpuid.h"

/**************************************************************************************************
* Fallback I32 type.
* Contains 1 x 32bit Unsigned Integers
*
* This is a fallback for cpus that are not yet supported.
*
* It may be useful to evaluate a single value from the same code base, as it offers the same interface
* as the SIMD types.
*
* ************************************************************************************************/
struct FallbackUInt32 {
	uint32_t v;
	typedef uint32_t F;
	FallbackUInt32() = default;
	FallbackUInt32(uint32_t a) : v(a) {};

	//*****Support Informtion*****

	//Performs a runtime CPU check to see if this type is supported.  Checks this type ONLY (integers in same the same level may not be supported) 
	static bool cpu_supported() { return true; }

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported() { return true; }


#if defined(_M_X64) || defined(__x86_64)
	//Performs a runtime CPU check to see if this type is supported.  Checks this type ONLY (integers in same the same level may not be supported) 
	static bool cpu_supported(CpuInformation) { return true; }

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported(CpuInformation ) { return true; }
#endif

	//Performs a compile time CPU check to see if this type is supported.  Checks this type ONLY (integers in same the same level may not be supported) 
	static constexpr bool compiler_supported() {
		return true;
	}

	//Performs a compile time support to see if the microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static constexpr bool compiler_level_supported() {
		return true;
	}

	//*****Elements*****
	static constexpr int size_of_element() { return sizeof(uint32_t); }
	static constexpr int number_of_elements() { return 1; }
	F element(int) { return v; }
	void set_element(int, F value) { v = value; }

	//*****Make Functions****
	static FallbackUInt32 make_sequential(uint32_t first) { return FallbackUInt32(first); }


	//*****Addition Operators*****
	FallbackUInt32& operator+=(const FallbackUInt32& rhs) noexcept { v += rhs.v; return *this; }
	FallbackUInt32& operator+=(uint32_t rhs) noexcept { v += rhs; return *this; }

	//*****Subtraction Operators*****
	FallbackUInt32& operator-=(const FallbackUInt32& rhs) noexcept { v -= rhs.v; return *this; }
	FallbackUInt32& operator-=(uint32_t rhs) noexcept { v -= rhs; return *this; }

	//*****Multiplication Operators*****
	FallbackUInt32& operator*=(const FallbackUInt32& rhs) noexcept { v *= rhs.v; return *this; }
	FallbackUInt32& operator*=(uint32_t rhs) noexcept { v *= rhs; return *this; }

	//*****Division Operators*****
	FallbackUInt32& operator/=(const FallbackUInt32& rhs) noexcept { v /= rhs.v; return *this; }
	FallbackUInt32& operator/=(uint32_t rhs) noexcept { v /= rhs;	return *this; }

	//*****Bitwise Logic Operators*****
	FallbackUInt32& operator&=(const FallbackUInt32& rhs) noexcept { v &= rhs.v; return *this; }
	FallbackUInt32& operator|=(const FallbackUInt32& rhs) noexcept { v |= rhs.v; return *this; }
	FallbackUInt32& operator^=(const FallbackUInt32& rhs) noexcept { v ^= rhs.v; return *this; }

	//*****Mathematical*****

};

//*****Addition Operators*****
inline static FallbackUInt32 operator+(FallbackUInt32  lhs, const FallbackUInt32& rhs) noexcept { lhs += rhs; return lhs; }


inline static FallbackUInt32 operator+(FallbackUInt32  lhs, uint32_t rhs) noexcept { lhs += rhs; return lhs; }
inline static FallbackUInt32 operator+(uint32_t lhs, FallbackUInt32 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static FallbackUInt32 operator-(FallbackUInt32  lhs, const FallbackUInt32& rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackUInt32 operator-(FallbackUInt32  lhs, uint32_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackUInt32 operator-(const uint32_t lhs, FallbackUInt32 rhs) noexcept { rhs.v = lhs - rhs.v; return rhs; }

//*****Multiplication Operators*****
inline static FallbackUInt32 operator*(FallbackUInt32  lhs, const FallbackUInt32& rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackUInt32 operator*(FallbackUInt32  lhs, uint32_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackUInt32 operator*(uint32_t lhs, FallbackUInt32 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static FallbackUInt32 operator/(FallbackUInt32  lhs, const FallbackUInt32& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static FallbackUInt32 operator/(FallbackUInt32  lhs, uint32_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static FallbackUInt32 operator/(const uint32_t lhs, FallbackUInt32 rhs) noexcept { rhs.v = lhs / rhs.v; return rhs; }


//*****Bitwise Logic Operators*****
inline static FallbackUInt32 operator&(const FallbackUInt32& lhs, const FallbackUInt32& rhs) noexcept { return FallbackUInt32(lhs.v & rhs.v); }
inline static FallbackUInt32 operator|(const FallbackUInt32& lhs, const FallbackUInt32& rhs) noexcept { return FallbackUInt32(lhs.v | rhs.v); }
inline static FallbackUInt32 operator^(const FallbackUInt32& lhs, const FallbackUInt32& rhs) noexcept { return FallbackUInt32(lhs.v ^ rhs.v); }
inline static FallbackUInt32 operator~(FallbackUInt32 lhs) noexcept { return FallbackUInt32(~lhs.v); }


//*****Shifting Operators*****
inline static FallbackUInt32 operator<<(FallbackUInt32 lhs, int bits) noexcept { lhs.v <<= bits; return lhs; }
inline static FallbackUInt32 operator>>(FallbackUInt32 lhs, int bits) noexcept { lhs.v >>= bits; return lhs; }
inline static FallbackUInt32 rotl(const FallbackUInt32& a, int bits) {return a << bits | a >> (32 - bits);};
inline static FallbackUInt32 rotr(const FallbackUInt32& a, int bits) {return a >> bits | a << (32 - bits);}

//*****Min/Max*****
inline static FallbackUInt32 min(FallbackUInt32 a, FallbackUInt32 b) { return FallbackUInt32(std::min(a.v, b.v)); }
inline static FallbackUInt32 max(FallbackUInt32 a, FallbackUInt32 b) { return FallbackUInt32(std::max(a.v, b.v)); }









//***************** x86_64 only code ******************
#if defined(_M_X64) || defined(__x86_64)
#include <immintrin.h>



/**************************************************************************************************
 * SIMD 512 type.  Contains 16 x 32bit Unsigned Integers
 * Requires AVX-512F 
 * ************************************************************************************************/
struct Simd512UInt32 {
	__m512i v;
	typedef uint32_t F;

	Simd512UInt32() = default;
	Simd512UInt32(__m512i a) : v(a) {};
	Simd512UInt32(F a) : v(_mm512_set1_epi32(a)) {};

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_avx512_f();
	}

	//Performs a compile time support. Checks this type ONLY (integers in same class may not be supported) 
	static constexpr bool compiler_supported() {
		return mt::environment::compiler_has_avx512f;
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported() {
		CpuInformation cpuid{};
		cpu_level_supported(cpuid);
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported(CpuInformation cpuid) {
		return cpuid.has_avx512_f() && cpuid.has_avx512_dq() && cpuid.has_avx512_vl() && cpuid.has_avx512_bw() && cpuid.has_avx512_cd();
	}

	//Performs a compile time support to see if the microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static constexpr bool compiler_level_supported() {
		return mt::environment::compiler_has_avx512f && mt::environment::compiler_has_avx512dq && mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512bw && mt::environment::compiler_has_avx512cd;
	}

	//*****Elements*****
	static constexpr int size_of_element() { return sizeof(uint32_t); }
	static constexpr int number_of_elements() { return 16; }	
	F element(int i) { return v.m512i_u32[i]; }
	void set_element(int i, F value) { v.m512i_u32[i] = value; }

	//*****Make Functions****
	static Simd512UInt32 make_sequential(uint32_t first) { return Simd512UInt32(_mm512_set_epi32(first + 15, first + 14, first + 13, first + 12, first + 11, first + 10, first + 9, first + 8, first + 7, first + 6, first + 5, first + 4, first + 3, first + 2, first + 1, first)); }


	//*****Addition Operators*****
	Simd512UInt32& operator+=(const Simd512UInt32& rhs) noexcept { v = _mm512_add_epi32(v, rhs.v); return *this; }
	Simd512UInt32& operator+=(uint32_t rhs) noexcept { v = _mm512_add_epi32(v, _mm512_set1_epi32(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd512UInt32& operator-=(const Simd512UInt32& rhs) noexcept { v = _mm512_sub_epi32(v, rhs.v); return *this; }
	Simd512UInt32& operator-=(uint32_t rhs) noexcept { v = _mm512_sub_epi32(v, _mm512_set1_epi32(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd512UInt32& operator*=(const Simd512UInt32& rhs) noexcept { v = _mm512_mullo_epi32(v, rhs.v); return *this; }
	Simd512UInt32& operator*=(uint32_t rhs) noexcept { v = _mm512_mullo_epi32(v, _mm512_set1_epi32(rhs)); return *this; }

	//*****Division Operators*****
	Simd512UInt32& operator/=(const Simd512UInt32& rhs) noexcept { v = _mm512_div_epu32(v, rhs.v); return *this; }
	Simd512UInt32& operator/=(uint32_t rhs) noexcept { v = _mm512_div_epu32(v, _mm512_set1_epi32(rhs));	return *this; }

	//*****Bitwise Logic Operators*****
	Simd512UInt32& operator&=(const Simd512UInt32& rhs) noexcept { v = _mm512_and_si512(v, rhs.v); return *this; }
	Simd512UInt32& operator|=(const Simd512UInt32& rhs) noexcept { v = _mm512_or_si512(v, rhs.v); return *this; }
	Simd512UInt32& operator^=(const Simd512UInt32& rhs) noexcept { v = _mm512_xor_si512(v, rhs.v); return *this; }

	//*****Mathematical*****

	
	
};

//*****Addition Operators*****
inline static Simd512UInt32 operator+(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512UInt32 operator+(Simd512UInt32  lhs, uint32_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512UInt32 operator+(uint32_t lhs, Simd512UInt32 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd512UInt32 operator-(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512UInt32 operator-(Simd512UInt32  lhs, uint32_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512UInt32 operator-(const uint32_t lhs, const Simd512UInt32& rhs) noexcept { return Simd512UInt32(_mm512_sub_epi32(_mm512_set1_epi32(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd512UInt32 operator*(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512UInt32 operator*(Simd512UInt32  lhs, uint32_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512UInt32 operator*(uint32_t lhs, Simd512UInt32 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd512UInt32 operator/(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd512UInt32 operator/(Simd512UInt32  lhs, uint32_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd512UInt32 operator/(const uint32_t lhs, const Simd512UInt32& rhs) noexcept { return Simd512UInt32(_mm512_div_epi32(_mm512_set1_epi32(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd512UInt32 operator&(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd512UInt32 operator|(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd512UInt32 operator^(Simd512UInt32  lhs, const Simd512UInt32& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd512UInt32 operator~(const Simd512UInt32& lhs) noexcept { return Simd512UInt32(_mm512_xor_epi32(lhs.v, _mm512_set1_epi32(0xFFFFFFFF))); } //No bitwise not in AVX512 so we use xor with a constant..


//*****Shifting Operators*****
inline static Simd512UInt32 operator<<(const Simd512UInt32& lhs, int bits) noexcept { return Simd512UInt32(_mm512_slli_epi32(lhs.v, bits)); }
inline static Simd512UInt32 operator>>(const Simd512UInt32& lhs, int bits) noexcept { return Simd512UInt32(_mm512_srli_epi32(lhs.v, bits)); }
inline static Simd512UInt32 rotl(const Simd512UInt32& a, const int bits) noexcept { return Simd512UInt32(_mm512_rolv_epi32(a.v,_mm512_set1_epi32(bits))); }
inline static Simd512UInt32 rotr(const Simd512UInt32& a, const int bits) noexcept { return Simd512UInt32(_mm512_rorv_epi32(a.v, _mm512_set1_epi32(bits))); }


//*****Min/Max*****
inline static Simd512UInt32 min(Simd512UInt32 a, Simd512UInt32 b) { return Simd512UInt32(_mm512_min_epu32(a.v, b.v)); }
inline static Simd512UInt32 max(Simd512UInt32 a, Simd512UInt32 b) { return Simd512UInt32(_mm512_max_epu32(a.v, b.v)); }


/**************************************************************************************************
 * SIMD 256 type.  Contains 8 x 32bit Unsigned Integers
 * Requires AVX2 support.
 * ************************************************************************************************/
struct Simd256UInt32 {
	__m256i v;
	typedef uint32_t F;

	Simd256UInt32() = default;
	Simd256UInt32(__m256i a) : v(a) {};
	Simd256UInt32(F a) : v(_mm256_set1_epi32(a)) {};

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_avx() && cpuid.has_avx2();
	}

	//Performs a compile time support. Checks this type ONLY (integers in same class may not be supported) 
	static constexpr bool compiler_supported() {
		return mt::environment::compiler_has_avx && mt::environment::compiler_has_fma;
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported() {
		CpuInformation cpuid{};
		cpu_level_supported(cpuid);
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported(CpuInformation cpuid) {
		return cpuid.has_avx2() && cpuid.has_avx() && cpuid.has_fma();
	}

	//Performs a compile time support to see if the microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static constexpr bool compiler_level_supported() {
		return mt::environment::compiler_has_avx2 && mt::environment::compiler_has_avx && mt::environment::compiler_has_fma;
	}

	//*****Elements*****
	static constexpr int size_of_element() { return sizeof(uint32_t); }
	static constexpr int number_of_elements() { return 8; }	
	F element(int i) { return v.m256i_u32[i]; }
	void set_element(int i, F value) { v.m256i_u32[i] = value; }

	//*****Addition Operators*****
	Simd256UInt32& operator+=(const Simd256UInt32& rhs) noexcept { v = _mm256_add_epi32(v, rhs.v); return *this; }
	Simd256UInt32& operator+=(uint32_t rhs) noexcept { v = _mm256_add_epi32(v, _mm256_set1_epi32(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd256UInt32& operator-=(const Simd256UInt32& rhs) noexcept { v = _mm256_sub_epi32(v, rhs.v); return *this; }
	Simd256UInt32& operator-=(uint32_t rhs) noexcept { v = _mm256_sub_epi32(v, _mm256_set1_epi32(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd256UInt32& operator*=(const Simd256UInt32& rhs) noexcept {v = _mm256_mullo_epi32(v, rhs.v);	return *this; }

	Simd256UInt32& operator*=(uint32_t rhs) noexcept { *this *= Simd256UInt32(_mm256_set1_epi32(rhs)); return *this; }

	//*****Division Operators*****
	Simd256UInt32& operator/=(const Simd256UInt32& rhs) noexcept { v = _mm256_div_epu32(v, rhs.v); return *this; }
	Simd256UInt32& operator/=(uint32_t rhs) noexcept { v = _mm256_div_epu32(v, _mm256_set1_epi32(rhs));	return *this; }

	//*****Bitwise Logic Operators*****
	Simd256UInt32& operator&=(const Simd256UInt32& rhs) noexcept { v = _mm256_and_si256(v, rhs.v); return *this; }
	Simd256UInt32& operator|=(const Simd256UInt32& rhs) noexcept { v = _mm256_or_si256(v, rhs.v); return *this; }
	Simd256UInt32& operator^=(const Simd256UInt32& rhs) noexcept { v = _mm256_xor_si256(v, rhs.v); return *this; }

	//*****Make Functions****
	static Simd256UInt32 make_sequential(uint32_t first) { return Simd256UInt32(_mm256_set_epi32(first + 7, first + 6, first + 5, first + 4, first + 3, first + 2, first + 1, first)); }
	

	//*****Mathematical*****
	

	

};

//*****Addition Operators*****
inline static Simd256UInt32 operator+(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256UInt32 operator+(Simd256UInt32  lhs, uint32_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256UInt32 operator+(uint32_t lhs, Simd256UInt32 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd256UInt32 operator-(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256UInt32 operator-(Simd256UInt32  lhs, uint32_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256UInt32 operator-(const uint32_t lhs, const Simd256UInt32& rhs) noexcept { return Simd256UInt32(_mm256_sub_epi32(_mm256_set1_epi32(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd256UInt32 operator*(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256UInt32 operator*(Simd256UInt32  lhs, uint32_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256UInt32 operator*(uint32_t lhs, Simd256UInt32 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd256UInt32 operator/(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd256UInt32 operator/(Simd256UInt32  lhs, uint32_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd256UInt32 operator/(const uint32_t lhs, const Simd256UInt32& rhs) noexcept { return Simd256UInt32(_mm256_div_epi32(_mm256_set1_epi32(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd256UInt32 operator&(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd256UInt32 operator|(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd256UInt32 operator^(Simd256UInt32  lhs, const Simd256UInt32& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd256UInt32 operator~(const Simd256UInt32& lhs) noexcept { return Simd256UInt32(_mm256_xor_si256(lhs.v, _mm256_cmpeq_epi32(lhs.v, lhs.v))); } //No bitwise not in AVX2 so we use xor with a constant..


//*****Shifting Operators*****
inline static Simd256UInt32 operator<<(const Simd256UInt32& lhs, int bits) noexcept { return Simd256UInt32(_mm256_slli_epi32(lhs.v, bits)); }
inline static Simd256UInt32 operator>>(const Simd256UInt32& lhs, int bits) noexcept { return Simd256UInt32(_mm256_srli_epi32(lhs.v, bits)); }
inline static Simd256UInt32 rotl(const Simd256UInt32& a, int bits) { return a << bits | a >> (32 - bits);};
inline static Simd256UInt32 rotr(const Simd256UInt32& a, int bits) { return a >> bits | a << (32 - bits);};

//*****Min/Max*****
inline static Simd256UInt32 min(Simd256UInt32 a, Simd256UInt32 b) {  return Simd256UInt32(_mm256_min_epu32(a.v, b.v)); }
inline static Simd256UInt32 max(Simd256UInt32 a, Simd256UInt32 b) { return Simd256UInt32(_mm256_max_epu32(a.v, b.v)); }









/**************************************************************************************************
*SIMD 128 type.Contains 4 x 32bit Unsigned Integers
* Requires SSE2 support.
* (will be faster with SSE4.1 enabled at compile time)
* ************************************************************************************************/
struct Simd128UInt32 {
	__m128i v;
	typedef uint32_t F;

	Simd128UInt32() = default;
	Simd128UInt32(__m128i a) : v(a) {};
	Simd128UInt32(F a) : v(_mm_set1_epi32(a)) {};

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_sse2() && cpuid.has_sse();
	}

	//Performs a compile time support. Checks this type ONLY (integers in same class may not be supported) 
	static constexpr bool compiler_supported() {
		return mt::environment::compiler_has_sse && mt::environment::compiler_has_sse2;
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported() {
		CpuInformation cpuid{};
		cpu_level_supported(cpuid);
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported(CpuInformation cpuid) {
		return cpuid.has_sse2() && cpuid.has_sse();
	}

	//Performs a compile time support to see if the microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static constexpr bool compiler_level_supported() {
		return mt::environment::compiler_has_sse && mt::environment::compiler_has_sse2;
	}

	//*****Elements*****
	static constexpr int size_of_element() { return sizeof(uint32_t); }
	static constexpr int number_of_elements() { return 4; }	
	F element(int i) { return v.m128i_u32[i]; }
	void set_element(int i, F value) { v.m128i_u32[i] = value; }

	//*****Addition Operators*****
	Simd128UInt32& operator+=(const Simd128UInt32& rhs) noexcept { v = _mm_add_epi32(v, rhs.v); return *this; }
	Simd128UInt32& operator+=(uint32_t rhs) noexcept { v = _mm_add_epi32(v, _mm_set1_epi32(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd128UInt32& operator-=(const Simd128UInt32& rhs) noexcept { v = _mm_sub_epi32(v, rhs.v); return *this; }
	Simd128UInt32& operator-=(uint32_t rhs) noexcept { v = _mm_sub_epi32(v, _mm_set1_epi32(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd128UInt32& operator*=(const Simd128UInt32& rhs) noexcept {
		if constexpr (mt::environment::compiler_has_sse4_1) {
			v = _mm_mullo_epi32(v, rhs.v); //SSE4.1
			return *this;
		}
		else {
			auto result02 = _mm_mul_epu32(v, rhs.v);  //Multiply words 0 and 2.  
			auto result13 = _mm_mul_epu32(_mm_srli_si128(v, 4), _mm_srli_si128(rhs.v, 4));  //Multiply words 1 and 3, by shifting them into 0,2.
			v = _mm_unpacklo_epi32(_mm_shuffle_epi32(result02, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(result13, _MM_SHUFFLE(0, 0, 2, 0))); // shuffle and pack
			return *this;
		}
	}  

	Simd128UInt32& operator*=(uint32_t rhs) noexcept { *this *= Simd128UInt32(_mm_set1_epi32(rhs)); return *this; } 

	//*****Division Operators*****
	Simd128UInt32& operator/=(const Simd128UInt32& rhs) noexcept { v = _mm_div_epu32(v, rhs.v); return *this; }
	Simd128UInt32& operator/=(uint32_t rhs) noexcept { v = _mm_div_epu32(v, _mm_set1_epi32(rhs));	return *this; } //SSE

	//*****Bitwise Logic Operators*****
	Simd128UInt32& operator&=(const Simd128UInt32& rhs) noexcept { v = _mm_and_si128(v, rhs.v); return *this; } //SSE2
	Simd128UInt32& operator|=(const Simd128UInt32& rhs) noexcept { v = _mm_or_si128(v, rhs.v); return *this; }
	Simd128UInt32& operator^=(const Simd128UInt32& rhs) noexcept { v = _mm_xor_si128(v, rhs.v); return *this; }

	//*****Make Functions****
	static Simd128UInt32 make_sequential(uint32_t first) { return Simd128UInt32(_mm_set_epi32(first + 3, first + 2, first + 1, first)); }


	//*****Mathematical*****




};

//*****Addition Operators*****
inline static Simd128UInt32 operator+(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128UInt32 operator+(Simd128UInt32  lhs, uint32_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128UInt32 operator+(uint32_t lhs, Simd128UInt32 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd128UInt32 operator-(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128UInt32 operator-(Simd128UInt32  lhs, uint32_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128UInt32 operator-(const uint32_t lhs, const Simd128UInt32& rhs) noexcept { return Simd128UInt32(_mm_sub_epi32(_mm_set1_epi32(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd128UInt32 operator*(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128UInt32 operator*(Simd128UInt32  lhs, uint32_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128UInt32 operator*(uint32_t lhs, Simd128UInt32 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd128UInt32 operator/(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd128UInt32 operator/(Simd128UInt32  lhs, uint32_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd128UInt32 operator/(const uint32_t lhs, const Simd128UInt32& rhs) noexcept { return Simd128UInt32(_mm_div_epi32(_mm_set1_epi32(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd128UInt32 operator&(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd128UInt32 operator|(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd128UInt32 operator^(Simd128UInt32  lhs, const Simd128UInt32& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd128UInt32 operator~(const Simd128UInt32& lhs) noexcept { return Simd128UInt32(_mm_xor_si128(lhs.v, _mm_cmpeq_epi32(lhs.v,lhs.v))); } 


//*****Shifting Operators*****
inline static Simd128UInt32 operator<<(const Simd128UInt32& lhs, const int bits) noexcept { return Simd128UInt32(_mm_slli_epi32(lhs.v, bits)); } //SSE2
inline static Simd128UInt32 operator>>(const Simd128UInt32& lhs, const int bits) noexcept { return Simd128UInt32(_mm_srli_epi32(lhs.v, bits)); }

inline static Simd128UInt32 rotl(const Simd128UInt32& a, int bits) { 
	if constexpr (mt::environment::compiler_has_avx512f) {
		return _mm_rolv_epi32(a.v, _mm_set1_epi32(bits));
	}
	else {
		return a << bits | a >> (32 - bits);
	}
};


inline static Simd128UInt32 rotr(const Simd128UInt32& a, int bits) { 
	if constexpr (mt::environment::compiler_has_avx512f) {
		return _mm_rorv_epi32(a.v, _mm_set1_epi32(bits));
	}
	else {
		return a >> bits | a << (32 - bits);
	}
};

//*****Min/Max*****
inline static Simd128UInt32 min(Simd128UInt32 a, Simd128UInt32 b) {
	if constexpr (mt::environment::compiler_has_sse4_1) {
		return Simd128UInt32(_mm_min_epu32(a.v, b.v)); //SSE4.1
	}
	else {
		//No min/max or compare for unsigned ints in SSE2 so we will just unroll.
		auto m3 = std::min(a.v.m128i_u32[3], b.v.m128i_u32[3]);
		auto m2 = std::min(a.v.m128i_u32[2], b.v.m128i_u32[2]);
		auto m1 = std::min(a.v.m128i_u32[1], b.v.m128i_u32[1]);
		auto m0 = std::min(a.v.m128i_u32[0], b.v.m128i_u32[0]);
		return Simd128UInt32(_mm_set_epi32(m3, m2, m1, m0));
	}
}



inline static Simd128UInt32 max(Simd128UInt32 a, Simd128UInt32 b) {
	if constexpr (mt::environment::compiler_has_sse4_1) {
		return Simd128UInt32(_mm_max_epu32(a.v, b.v));  //SSE4.1
	}
	else {
		//No min/max or compare for unsigned ints in SSE2 so we will just unroll.
		auto m3 = std::max(a.v.m128i_u32[3], b.v.m128i_u32[3]);
		auto m2 = std::max(a.v.m128i_u32[2], b.v.m128i_u32[2]);
		auto m1 = std::max(a.v.m128i_u32[1], b.v.m128i_u32[1]);
		auto m0 = std::max(a.v.m128i_u32[0], b.v.m128i_u32[0]);
		return Simd128UInt32(_mm_set_epi32(m3, m2, m1, m0));
	}
}


#endif //x86_64





/**************************************************************************************************
 * Check that each type implements the desired types from simd-concepts.h
 * (Not sure why this fails on intelisense, but compliles ok.)
 * ************************************************************************************************/
static_assert(Simd<FallbackUInt32>, "FallbackUInt32 does not implement the concept Simd");
static_assert(SimdUInt<FallbackUInt32>, "FallbackUInt32 does not implement the concept SimdUint");
static_assert(SimdUInt32<FallbackUInt32>, "FallbackUInt32 does not implement the concept SimdUInt32");

#if defined(_M_X64) || defined(__x86_64)
static_assert(Simd<Simd128UInt32>, "Simd128UInt32 does not implement the concept Simd");
static_assert(Simd<Simd256UInt32>, "Simd256UInt32 does not implement the concept Simd");
static_assert(Simd<Simd512UInt32>, "Simd512UInt32 does not implement the concept Simd");

static_assert(SimdUInt<Simd128UInt32>, "Simd256UInt32 does not implement the concept SimdUint");
static_assert(SimdUInt<Simd256UInt32>, "Simd256UInt32 does not implement the concept SimdUint");
static_assert(SimdUInt<Simd512UInt32>, "Simd512UInt32 does not implement the concept SimdUint");



static_assert(SimdUInt32<Simd128UInt32>, "Simd128UInt32 does not implement the concept SimdUInt32");
static_assert(SimdUInt32<Simd256UInt32>, "Simd256UInt32 does not implement the concept SimdUInt32");
static_assert(SimdUInt32<Simd512UInt32>, "Simd512UInt32 does not implement the concept SimdUInt32");
#endif



/**************************************************************************************************
 Define SimdNativeUInt32 as the best supported type at compile time.
*************************************************************************************************/
#if defined(_M_X64) || defined(__x86_64)
#if defined(__AVX512F__) && defined(__AVX512DQ__) 
typedef Simd512UInt32 SimdNativeUInt32;
#else
#if defined(__AVX2__) && defined(__AVX__) 
typedef Simd256UInt32 SimdNativeUInt32;
#else
#if defined(__SSE4_1__) && defined(__SSE4_1__) && defined(__SSE3__) && defined(__SSSE3__) 
typedef Simd128UInt32 SimdNativeUInt32;
#else
typedef Simd128UInt32 SimdNativeUInt32;
#endif	
#endif	
#endif
#else
typedef FallbackUInt32 SimdNativeUInt32;
#endif