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

Basic SIMD Types for 64-bit Unsigned Integers:

FallbackFloat32		- Works on all build modes and CPUs.  Forwards most requests to the standard library.

Simd128Float32		- x86_64 Microarchitecture Level 1 - Works on all x86_64 CPUs.
					- Requires SSE and SSE2 support.  Will use SSE4.1 instructions when __SSE4_1__ or __AVX__ defined.

Simd256Float32		- x86_64 Microarchitecture Level 3.
					- Requires AVX, AVX2 and FMA support.

Simd512Float32		- x86_64 Microarchitecture Level 4.
					- Requires AVX512F, AVX512DQ, ACX512VL, AVX512CD, AVX512BW

SimdNativeFloat32	- A Typedef referring to one of the above types.  Chosen based on compiler support/mode.
					- Just use this type in your code if you are building for a specific platform.


Checking CPU Support:
Unless you are using a SimdNative typedef, you must check for CPU support before using any of these types.

Types reqpresenting floats, doubles, ints, longs etc are arranged in microarchitecture level groups.
Generally CPUs have more SIMD support for floats than ints.
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

#include "simd-cpuid.h"
#include "simd-concepts.h"

#include <stdint.h>
#include <bit>
#include <algorithm>

/**************************************************************************************************
* Fallback I64 type.
* Contains 1 x 64bit Unsigned Integers
*
* This is a fallback for cpus that are not yet supported.
*
* It may be useful to evaluate a single value from the same code base, as it offers the same interface
* as the SIMD types.
*
* ************************************************************************************************/
struct FallbackUInt64 {
	uint64_t v;
	typedef uint64_t F;

	FallbackUInt64() = default;
	FallbackUInt64(uint64_t a) : v(a) {};

	//*****Support Informtion*****

	//Performs a runtime CPU check to see if this type is supported.  Checks this type ONLY (integers in same the same level may not be supported) 
	static bool cpu_supported() { return true; }

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported() { return true; }


#if defined(_M_X64) || defined(__x86_64)
	//Performs a runtime CPU check to see if this type is supported.  Checks this type ONLY (integers in same the same level may not be supported) 
	static bool cpu_supported(CpuInformation) { return true; }

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static bool cpu_level_supported(CpuInformation) { return true; }
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
	static constexpr int size_of_element() { return sizeof(uint64_t); }
	static constexpr int number_of_elements() { return 1; }	
	F element(int) { return v; }

	//*****Make Functions****
	static FallbackUInt64 make_sequential(uint32_t first) { return FallbackUInt64(first); }


	//*****Addition Operators*****
	FallbackUInt64& operator+=(const FallbackUInt64& rhs) noexcept { v += rhs.v; return *this; }
	FallbackUInt64& operator+=(uint64_t rhs) noexcept { v += rhs; return *this; }

	//*****Subtraction Operators*****
	FallbackUInt64& operator-=(const FallbackUInt64& rhs) noexcept { v -= rhs.v; return *this; }
	FallbackUInt64& operator-=(uint64_t rhs) noexcept { v -= rhs; return *this; }

	//*****Multiplication Operators*****
	FallbackUInt64& operator*=(const FallbackUInt64& rhs) noexcept { v *= rhs.v; return *this; }
	FallbackUInt64& operator*=(uint64_t rhs) noexcept { v *= rhs; return *this; }

	//*****Division Operators*****
	FallbackUInt64& operator/=(const FallbackUInt64& rhs) noexcept { v /= rhs.v; return *this; }
	FallbackUInt64& operator/=(uint64_t rhs) noexcept { v /= rhs;	return *this; }

	//*****Bitwise Logic Operators*****
	FallbackUInt64& operator&=(const FallbackUInt64& rhs) noexcept { v &= rhs.v; return *this; }
	FallbackUInt64& operator|=(const FallbackUInt64& rhs) noexcept { v |= rhs.v; return *this; }
	FallbackUInt64& operator^=(const FallbackUInt64& rhs) noexcept { v ^= rhs.v; return *this; }

};

//*****Addition Operators*****
inline static FallbackUInt64 operator+(FallbackUInt64  lhs, const FallbackUInt64& rhs) noexcept { lhs += rhs; return lhs; }


inline static FallbackUInt64 operator+(FallbackUInt64  lhs, uint64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static FallbackUInt64 operator+(uint64_t lhs, FallbackUInt64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static FallbackUInt64 operator-(FallbackUInt64  lhs, const FallbackUInt64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackUInt64 operator-(FallbackUInt64  lhs, uint64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackUInt64 operator-(const uint64_t lhs, FallbackUInt64 rhs) noexcept { rhs.v = lhs - rhs.v; return rhs; }

//*****Multiplication Operators*****
inline static FallbackUInt64 operator*(FallbackUInt64  lhs, const FallbackUInt64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackUInt64 operator*(FallbackUInt64  lhs, uint64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackUInt64 operator*(uint64_t lhs, FallbackUInt64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static FallbackUInt64 operator/(FallbackUInt64  lhs, const FallbackUInt64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static FallbackUInt64 operator/(FallbackUInt64  lhs, uint64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static FallbackUInt64 operator/(const uint64_t lhs, FallbackUInt64 rhs) noexcept { rhs.v = lhs / rhs.v; return rhs; }


//*****Bitwise Logic Operators*****
inline static FallbackUInt64 operator&(const FallbackUInt64& lhs, const FallbackUInt64& rhs) noexcept { return FallbackUInt64(lhs.v & rhs.v); }
inline static FallbackUInt64 operator|(const FallbackUInt64& lhs, const FallbackUInt64& rhs) noexcept { return FallbackUInt64(lhs.v | rhs.v); }
inline static FallbackUInt64 operator^(const FallbackUInt64& lhs, const FallbackUInt64& rhs) noexcept { return FallbackUInt64(lhs.v ^ rhs.v); }
inline static FallbackUInt64 operator~(FallbackUInt64 lhs) noexcept { return FallbackUInt64(~lhs.v); }


//*****Shifting Operators*****
inline static FallbackUInt64 operator<<(FallbackUInt64 lhs, int bits) noexcept { lhs.v <<= bits; return lhs; }
inline static FallbackUInt64 operator>>(FallbackUInt64 lhs, int bits) noexcept { lhs.v >>= bits; return lhs; }

inline static FallbackUInt64 rotl(const FallbackUInt64& a, int bits) { return a << bits | a >> (64 - bits); };
inline static FallbackUInt64 rotr(const FallbackUInt64& a, int bits) { return a >> bits | a<< (64 - bits); };

//*****Min/Max*****
inline static FallbackUInt64 min(FallbackUInt64 a, FallbackUInt64 b) { return FallbackUInt64(std::min(a.v, b.v)); }
inline static FallbackUInt64 max(FallbackUInt64 a, FallbackUInt64 b) { return FallbackUInt64(std::max(a.v, b.v)); }



//***************** x86_64 only code ******************
#if defined(_M_X64) || defined(__x86_64)
#include <immintrin.h>


/**************************************************************************************************
 * SIMD 512 type.  Contains 8 x 64bit Unsigned Integers
 * Requires AVX-512F and AVX-512DQ support.
 * ************************************************************************************************/
struct Simd512UInt64 {
	__m512i v;
	typedef uint64_t F;

	Simd512UInt64() = default;
	Simd512UInt64(__m512i a) : v(a) {};
	Simd512UInt64(F a) : v(_mm512_set1_epi64(a)) {}

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_avx512_f() && cpuid.has_avx512_dq();
	}
	
	//Performs a compile time support. Checks this type ONLY (integers in same class may not be supported) 
	static constexpr bool compiler_supported() {
		return mt::environment::compiler_has_avx512f && mt::environment::compiler_has_avx512dq;
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
	F element(int i) { return v.m512i_u64[i]; }
	void set_element(int i, F value) { v.m512i_u64[i] = value; }
	static constexpr int size_of_element() { return sizeof(uint64_t); }
	static constexpr int number_of_elements() { return 8; }

	//*****Make Functions****
	static Simd512UInt64 make_sequential(uint64_t first) { return Simd512UInt64(_mm512_set_epi64(first + 7, first + 6, first + 5, first + 4, first + 3, first + 2, first + 1, first)); }


	//*****Addition Operators*****
	Simd512UInt64& operator+=(const Simd512UInt64& rhs) noexcept { v = _mm512_add_epi64(v, rhs.v); return *this; }
	Simd512UInt64& operator+=(uint64_t rhs) noexcept { v = _mm512_add_epi64(v, _mm512_set1_epi64(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd512UInt64& operator-=(const Simd512UInt64& rhs) noexcept { v = _mm512_sub_epi64(v, rhs.v); return *this; }
	Simd512UInt64& operator-=(uint64_t rhs) noexcept { v = _mm512_sub_epi64(v, _mm512_set1_epi64(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd512UInt64& operator*=(const Simd512UInt64& rhs) noexcept { v = _mm512_mullo_epi64(v, rhs.v); return *this; }
	Simd512UInt64& operator*=(uint64_t rhs) noexcept { v = _mm512_mullo_epi64(v, _mm512_set1_epi64(rhs)); return *this; }

	//*****Division Operators*****
	Simd512UInt64& operator/=(const Simd512UInt64& rhs) noexcept { v = _mm512_div_epu64(v, rhs.v); return *this; }
	Simd512UInt64& operator/=(uint64_t rhs) noexcept { v = _mm512_div_epu64(v, _mm512_set1_epi64(rhs));	return *this; }

	//*****Bitwise Logic Operators*****
	Simd512UInt64& operator&=(const Simd512UInt64& rhs) noexcept {v= _mm512_and_si512(v, rhs.v); return *this; }
	Simd512UInt64& operator|=(const Simd512UInt64& rhs) noexcept {v= _mm512_or_si512(v, rhs.v); return *this; }
	Simd512UInt64& operator^=(const Simd512UInt64& rhs) noexcept {v= _mm512_xor_si512(v, rhs.v); return *this; }



};

//*****Addition Operators*****
inline static Simd512UInt64 operator+(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512UInt64 operator+(Simd512UInt64  lhs, uint64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512UInt64 operator+(uint64_t lhs, Simd512UInt64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd512UInt64 operator-(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512UInt64 operator-(Simd512UInt64  lhs, uint64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512UInt64 operator-(const uint64_t lhs, const Simd512UInt64& rhs) noexcept { return Simd512UInt64(_mm512_sub_epi64(_mm512_set1_epi64(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd512UInt64 operator*(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512UInt64 operator*(Simd512UInt64  lhs, uint64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512UInt64 operator*(uint64_t lhs, Simd512UInt64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd512UInt64 operator/(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd512UInt64 operator/(Simd512UInt64  lhs, uint64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd512UInt64 operator/(const uint64_t lhs, const Simd512UInt64& rhs) noexcept { return Simd512UInt64(_mm512_div_epi64(_mm512_set1_epi64(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd512UInt64 operator&(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd512UInt64 operator|(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd512UInt64 operator^(Simd512UInt64  lhs, const Simd512UInt64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd512UInt64 operator~(const Simd512UInt64& lhs) noexcept { return Simd512UInt64(_mm512_xor_epi64(lhs.v, _mm512_set1_epi64(0xFFFFFFFFFFFFFFFF))); } //No bitwise not in AVX512 so we use xor with a constant..


//*****Shifting Operators*****
inline static Simd512UInt64 operator<<(const Simd512UInt64& lhs, int bits) noexcept { return Simd512UInt64(_mm512_slli_epi64(lhs.v, bits)); }
inline static Simd512UInt64 operator>>(const Simd512UInt64& lhs, int bits) noexcept { return Simd512UInt64(_mm512_srli_epi64(lhs.v, bits)); }

inline static Simd512UInt64 rotl(const Simd512UInt64& a, const int bits) noexcept { return Simd512UInt64(_mm512_rolv_epi64(a.v, _mm512_set1_epi64(bits))); }
inline static Simd512UInt64 rotr(const Simd512UInt64& a, const int bits) noexcept { return Simd512UInt64(_mm512_rorv_epi64(a.v, _mm512_set1_epi64(bits))); }

//*****Min/Max*****
inline static Simd512UInt64 min(Simd512UInt64 a, Simd512UInt64 b) { return Simd512UInt64(_mm512_min_epu64(a.v, b.v)); }
inline static Simd512UInt64 max(Simd512UInt64 a, Simd512UInt64 b) { return Simd512UInt64(_mm512_max_epu64(a.v, b.v)); }



/**************************************************************************************************
 * SIMD 256 type.  Contains 4 x 64bit Unsigned Integers
 * Requires AVX2 support.
 * ************************************************************************************************/
struct Simd256UInt64 {
	__m256i v;

	typedef uint64_t F;

	Simd256UInt64() = default;
	Simd256UInt64(__m256i a) : v(a) {};
	Simd256UInt64(F a) : v(_mm256_set1_epi64x(a)) {}

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
		return mt::environment::compiler_has_avx2 && mt::environment::compiler_has_avx && mt::environment::compiler_has_fma;
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
	static constexpr int size_of_element() { return sizeof(uint64_t); }
	static constexpr int number_of_elements() { return 4; }	
	F element(int i) { return v.m256i_u64[i]; }
	

	//*****Addition Operators*****
	Simd256UInt64& operator+=(const Simd256UInt64& rhs) noexcept {v = _mm256_add_epi64(v, rhs.v); return *this;}
	Simd256UInt64& operator+=(const uint64_t rhs) noexcept {v = _mm256_add_epi64(v, _mm256_set1_epi64x(rhs));	return *this;}

	//*****Subtraction Operators*****
	Simd256UInt64& operator-=(const Simd256UInt64& rhs) noexcept {v = _mm256_sub_epi64(v, rhs.v); return *this;}
	Simd256UInt64& operator-=(const uint64_t rhs) noexcept {v = _mm256_sub_epi64(v, _mm256_set1_epi64x(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd256UInt64& operator*=(const Simd256UInt64& rhs) noexcept {
		if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512dq) {
			//There is a slight chance a user is using this struct but has compiled the code with avx512dq and avx512vl enabled.
			this->v = _mm256_mullo_epi64(v, rhs.v);											
			return *this;
		}
		else {			
			//Multiplication (i64*i64->i64) is not implemented for AVX2.
			//Algorithm:
			//		If x*y = [a,b]*[c,d] (where a,b,c,d are digits/dwords).
			//			   = (2^64) ac + (2^32) (ad + bc) + bd
			//				 (We ignore upper 64bits so, we can ignore the (2^64) term.)
			//
			//Given lhs = [a,b] and rhs=[c,d]
			auto digit1 = _mm256_mul_epu32(v, rhs.v);											//=[0+carry, bd]	: Calculate bd (i32*i32->i64)
			auto rhs_swap = _mm256_shuffle_epi32(rhs.v, 0xB1);									//=[d,c]			: Swaps the low and high dwords on RHS . 
			auto ad_bc = _mm256_mullo_epi32(v, rhs_swap);										//=[ad,bc]			: Multiply every dword together (i32*i32->i32).          
			auto bc_00 = _mm256_slli_epi64(ad_bc, 32);											//=[bc,0]			: Shift Left to put bc in the upper dword.
			auto ad_plus_bc = _mm256_add_epi32(ad_bc, bc_00);									//=[ad+bc,bc]		: Perofrm addition in the upper dword
			auto digit2 = _mm256_and_si256(ad_plus_bc, _mm256_set1_epi64x(0xFFFFFFFF00000000)); //=[ad+bc,0]		: Zero lower dword using &
			this->v = _mm256_add_epi64(digit1, digit2);											//=[ad+dc+carry,bd] : Add digits to get final result. 
			return *this;
		}
		
	}

	Simd256UInt64& operator*=(uint64_t rhs) noexcept { *this *= Simd256UInt64(_mm256_set1_epi64x(rhs)); return *this; }

	//*****Division Operators*****
	Simd256UInt64& operator/=(const Simd256UInt64& rhs) noexcept { v = _mm256_div_epu64(v, rhs.v); return *this; }
	Simd256UInt64& operator/=(uint64_t rhs) noexcept { v = _mm256_div_epu64(v, _mm256_set1_epi64x(rhs));	return *this; }

	//*****Bitwise Logic Operators*****
	Simd256UInt64& operator&=(const Simd256UInt64& rhs) noexcept {v=_mm256_and_si256(v, rhs.v);return *this;}
	Simd256UInt64& operator|=(const Simd256UInt64& rhs) noexcept {v=_mm256_or_si256(v, rhs.v); return *this;}
	Simd256UInt64& operator^=(const Simd256UInt64&rhs) noexcept {v=_mm256_xor_si256(v, rhs.v);return *this;}

	//*****Make Functions****
	static Simd256UInt64 make_sequential(uint64_t first) noexcept { return Simd256UInt64(_mm256_set_epi64x(first + 3, first + 2, first + 1, first)); }
	


};

//*****Addition Operators*****
inline static Simd256UInt64 operator+(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256UInt64 operator+(Simd256UInt64  lhs, uint64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256UInt64 operator+(uint64_t lhs, Simd256UInt64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd256UInt64 operator-(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256UInt64 operator-(Simd256UInt64  lhs, uint64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256UInt64 operator-(const uint64_t lhs, const Simd256UInt64&  rhs) noexcept { return Simd256UInt64(_mm256_sub_epi64(_mm256_set1_epi64x(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd256UInt64 operator*(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256UInt64 operator*(Simd256UInt64  lhs, uint64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256UInt64 operator*(uint64_t lhs, Simd256UInt64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd256UInt64 operator/(Simd256UInt64  lhs, const Simd256UInt64 & rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd256UInt64 operator/(Simd256UInt64  lhs, uint64_t rhs) noexcept {lhs /= rhs; return lhs; }
inline static Simd256UInt64 operator/(const uint64_t lhs, const Simd256UInt64& rhs) noexcept { return Simd256UInt64(_mm256_div_epi64(_mm256_set1_epi64x(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd256UInt64 operator&(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd256UInt64 operator|(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd256UInt64 operator^(Simd256UInt64  lhs, const Simd256UInt64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd256UInt64 operator~(const Simd256UInt64& lhs) noexcept { return Simd256UInt64(_mm256_xor_si256(lhs.v, _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFF))); } //No bitwise not in AVX2 so we use xor with a constant..
	

//*****Shifting Operators*****
inline static Simd256UInt64 operator<<(const Simd256UInt64& lhs, int bits) noexcept { return Simd256UInt64(_mm256_slli_epi64(lhs.v, bits)); }
inline static Simd256UInt64 operator>>(const Simd256UInt64& lhs, int bits) noexcept { return Simd256UInt64(_mm256_srli_epi64(lhs.v, bits)); }
inline static Simd256UInt64 rotl(const Simd256UInt64& a, int bits) {return a << bits | a >> (64 - bits);};
inline static Simd256UInt64 rotr(const Simd256UInt64& a, int bits) {return a >> bits | a << (64 - bits);};

//*****Min/Max*****
inline static Simd256UInt64 min(Simd256UInt64 a, Simd256UInt64 b) noexcept { return Simd256UInt64(_mm256_min_epu64(a.v, b.v)); }
inline static Simd256UInt64 max(Simd256UInt64 a, Simd256UInt64 b) noexcept { return Simd256UInt64(_mm256_max_epu64(a.v, b.v)); }






/**************************************************************************************************
 * SIMD 128 type.  Contains 4 x 64bit Unsigned Integers
 * Requires SSE support.
 * 
 * //BROKEN:  CURRENTLY REQUIRES SSE`4.1
 * ************************************************************************************************/
struct Simd128UInt64 {
	__m128i v;

	typedef uint64_t F;

	Simd128UInt64() = default;
	Simd128UInt64(__m128i a) : v(a) {};
	Simd128UInt64(F a) : v(_mm_set1_epi64x(a)) {}

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_sse() && cpuid.has_sse2();
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
		return cpuid.has_sse2() && cpuid.has_sse2() ;
	}


	//Performs a compile time support to see if the microarchitecture level is supported.  (This will ensure that referernced integer types are also supported)
	static constexpr bool compiler_level_supported() {
		return mt::environment::compiler_has_sse && mt::environment::compiler_has_sse2;
	}


	static constexpr int size_of_element() { return sizeof(uint64_t); }
	static constexpr int number_of_elements() { return 2; }

	//*****Elements*****
	F element(int i) { return v.m128i_u64[i]; }


	//*****Addition Operators*****
	Simd128UInt64& operator+=(const Simd128UInt64& rhs) noexcept { v = _mm_add_epi64(v, rhs.v); return *this; } //sse2
	Simd128UInt64& operator+=(const uint64_t rhs) noexcept { v = _mm_add_epi64(v, _mm_set1_epi64x(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd128UInt64& operator-=(const Simd128UInt64& rhs) noexcept { v = _mm_sub_epi64(v, rhs.v); return *this; } //sse2
	Simd128UInt64& operator-=(const uint64_t rhs) noexcept { v = _mm_sub_epi64(v, _mm_set1_epi64x(rhs));	return *this; }



	//*****Multiplication Operators*****
	Simd128UInt64& operator*=(const Simd128UInt64& rhs) noexcept {
		if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512dq) {
			//There is a slight chance a user is using this struct but has compiled the code with avx512dq and avx512vl enabled.
			this->v = _mm_mullo_epi64(v, rhs.v);
			return *this;
		} 
		else {
			//Not supported, we will just unroll as there are only 2 values anyway.			
			const auto m1 = v.m128i_u64[1] * rhs.v.m128i_u64[1];
			const auto m0 = v.m128i_u64[0] * rhs.v.m128i_u64[0];
			v = _mm_set_epi64x(m1, m0);			
			return *this;
		}

	}

	Simd128UInt64& operator*=(uint64_t rhs) noexcept { *this *= Simd128UInt64(_mm_set1_epi64x(rhs)); return *this; }

	//*****Division Operators*****
	Simd128UInt64& operator/=(const Simd128UInt64& rhs) noexcept { v = _mm_div_epu64(v, rhs.v); return *this; } //sse
	Simd128UInt64& operator/=(uint64_t rhs) noexcept { v = _mm_div_epu64(v, _mm_set1_epi64x(rhs));	return *this; }

	//*****Bitwise Logic Operators*****
	Simd128UInt64& operator&=(const Simd128UInt64& rhs) noexcept { v = _mm_and_si128(v, rhs.v); return *this; } //sse2
	Simd128UInt64& operator|=(const Simd128UInt64& rhs) noexcept { v = _mm_or_si128(v, rhs.v); return *this; }
	Simd128UInt64& operator^=(const Simd128UInt64& rhs) noexcept { v = _mm_xor_si128(v, rhs.v); return *this; }

	//*****Make Functions****
	static Simd128UInt64 make_sequential(uint64_t first) noexcept { return Simd128UInt64(_mm_set_epi64x(first + 1, first)); }

	private:

    //32-bit mullo multiply using only sse2
	__m128i software_mullo_epu32(__m128i a, __m128i b) {
		auto result02 = _mm_mul_epu32(a, b);  //Multiply words 0 and 2.  
		auto result13 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));  //Multiply words 1 and 3, by shifting them into 0,2.
		return  _mm_unpacklo_epi32(_mm_shuffle_epi32(result02, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(result13, _MM_SHUFFLE(0, 0, 2, 0))); // shuffle and pack
	}

};

//*****Addition Operators*****
inline static Simd128UInt64 operator+(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128UInt64 operator+(Simd128UInt64  lhs, uint64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128UInt64 operator+(uint64_t lhs, Simd128UInt64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd128UInt64 operator-(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128UInt64 operator-(Simd128UInt64  lhs, uint64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128UInt64 operator-(const uint64_t lhs, const Simd128UInt64& rhs) noexcept { return Simd128UInt64(_mm_sub_epi64(_mm_set1_epi64x(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd128UInt64 operator*(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128UInt64 operator*(Simd128UInt64  lhs, uint64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128UInt64 operator*(uint64_t lhs, Simd128UInt64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd128UInt64 operator/(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd128UInt64 operator/(Simd128UInt64  lhs, uint64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd128UInt64 operator/(const uint64_t lhs, const Simd128UInt64& rhs) noexcept { return Simd128UInt64(_mm_div_epi64(_mm_set1_epi64x(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd128UInt64 operator&(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd128UInt64 operator|(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd128UInt64 operator^(Simd128UInt64  lhs, const Simd128UInt64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd128UInt64 operator~(const Simd128UInt64& lhs) noexcept { return Simd128UInt64(_mm_xor_si128(lhs.v, _mm_set1_epi64x(0xFFFFFFFFFFFFFFFF))); } 


//*****Shifting Operators*****
inline static Simd128UInt64 operator<<(const Simd128UInt64& lhs, int bits) noexcept { return Simd128UInt64(_mm_slli_epi64(lhs.v, bits)); } //sse2
inline static Simd128UInt64 operator>>(const Simd128UInt64& lhs, int bits) noexcept { return Simd128UInt64(_mm_srli_epi64(lhs.v, bits)); }

inline static Simd128UInt64 rotl(const Simd128UInt64& a, int bits) {
	if constexpr (mt::environment::compiler_has_avx512f) {
		return _mm_rolv_epi64(a.v, _mm_set1_epi64x(bits));
	}
	else {
		return a << bits | a >> (64 - bits);
	}
};


inline static Simd128UInt64 rotr(const Simd128UInt64& a, int bits) {
	if constexpr (mt::environment::compiler_has_avx512f) {
		return _mm_rorv_epi64(a.v, _mm_set1_epi64x(bits));
	}
	else {
		return a >> bits | a << (64 - bits);
	}
};

//*****Min/Max*****
inline static Simd128UInt64 min(Simd128UInt64 a, Simd128UInt64 b) noexcept {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512dq) {
		return Simd128UInt64(_mm_min_epu64(a.v, b.v)); //AVX-512
	}
	else {
		//No min/max or compare for unsigned ints in SSE2 so we will just unroll.
		auto m1 = std::min(a.v.m128i_u64[1], b.v.m128i_u64[1]);
		auto m0 = std::min(a.v.m128i_u64[0], b.v.m128i_u64[0]);
		return Simd128UInt64(_mm_set_epi64x(m1, m0));
	}
}

	
inline static Simd128UInt64 max(Simd128UInt64 a, Simd128UInt64 b) noexcept {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512dq) {
		return Simd128UInt64(_mm_max_epu64(a.v, b.v)); //AVX-512
	}
	else {
		//No min/max or compare for unsigned ints in SSE2 so we will just unroll.
		auto m1 = std::max(a.v.m128i_u64[1], b.v.m128i_u64[1]);
		auto m0 = std::max(a.v.m128i_u64[0], b.v.m128i_u64[0]);
		return Simd128UInt64(_mm_set_epi64x(m1, m0));
	}
}


#endif //x86_64


/**************************************************************************************************
 * Check that each type implements the desired types from simd-concepts.h
 * ************************************************************************************************/
static_assert(Simd<FallbackUInt64>, "FallbackUInt64 does not implement the concept Simd");
static_assert(SimdUInt<FallbackUInt64>, "FallbackUInt64 does not implement the concept SimdUInt");
static_assert(SimdUInt64<FallbackUInt64>, "FallbackUInt64 does not implement the concept SimdUInt64");


#if defined(_M_X64) || defined(__x86_64)
static_assert(Simd<Simd128UInt64>, "Simd128UInt64 does not implement the concept Simd");
static_assert(Simd<Simd256UInt64>, "Simd256UInt64 does not implement the concept Simd");
static_assert(Simd<Simd512UInt64>, "Simd512UInt64 does not implement the concept Simd");

static_assert(SimdUInt<Simd128UInt64>, "Simd128UInt64 does not implement the concept SimdUInt");
static_assert(SimdUInt<Simd256UInt64>, "Simd256UInt64 does not implement the concept SimdUInt");
static_assert(SimdUInt<Simd512UInt64>, "Simd512UInt64 does not implement the concept SimdUInt");

static_assert(SimdUInt64<Simd128UInt64>, "Simd128UInt64 does not implement the concept SimdUInt64");
static_assert(SimdUInt64<Simd256UInt64>, "Simd256UInt64 does not implement the concept SimdUInt64");
static_assert(SimdUInt64<Simd512UInt64>, "Simd512UInt64 does not implement the concept SimdUInt64");




#endif


/**************************************************************************************************
 Define SimdNativeUInt64 as the best supported type at compile time.
*************************************************************************************************/
#if defined(_M_X64) || defined(__x86_64)
	#if defined(__AVX512F__) && defined(__AVX512DQ__) 
	typedef Simd512UInt64 SimdNativeUInt64;
	#else
	#if defined(__AVX2__) && defined(__AVX__) 
	typedef Simd256UInt64 SimdNativeUInt64;
	#else
	#if defined(__SSE4_1__) && defined(__SSE4_1__) && defined(__SSE3__) && defined(__SSSE3__) 
	typedef Simd128UInt64 SimdNativeUInt64;
	#else
	typedef Simd128UInt64 SimdNativeUInt64;
	#endif	
	#endif	
	#endif
#else
	typedef FallbackUInt64 SimdNativeUInt64;
#endif