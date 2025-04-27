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

Basic SIMD Types for 64-bit Signed Integers:

FallbackInt64		- Works on all build modes and CPUs.  Forwards most requests to the standard library.

Simd128Int64		- x86_64 Microarchitecture Level 1 - Works on all x86_64 CPUs.
					- Requires SSE and SSE2 support.  Will use SSE4.1 instructions when __SSE4_1__ or __AVX__ defined.

Simd256Int64		- x86_64 Microarchitecture Level 3.
					- Requires AVX, AVX2 and FMA support.

Simd512Int64		- x86_64 Microarchitecture Level 4.
					- Requires AVX512F, AVX512DQ, ACX512VL, AVX512CD, AVX512BW

SimdNativeInt64	- A Typedef referring to one of the above types.  Chosen based on compiler support/mode.
					- Just use this type in your code if you are building for a specific platform.


Checking CPU Support:
Unless you are using a SimdNative typedef, you must check for CPU support before using any of these types.

Types reqpresenting floats, doubles, ints, longs etc are arranged in microarchitecture level groups.
Generally CPUs have more SIMD support for floats than ints (and 64 bit is better than 64-bit).
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
I've included FallbackFloat64 for use with Emscripen, but use SimdNativeFloat64 as SIMD support will be added soon.


*********************************************************************************************************/
#pragma once


#include <stdint.h>
#include "simd-cpuid.h"
#include "simd-concepts.h"

/**************************************************************************************************
* Fallback Int64 type.
* Contains 1 x 64bit Signed Integer
*
* This is a fallback for cpus that are not yet supported.
*
* It may be useful to evaluate a single value from the same code base, as it offers the same interface
* as the SIMD types.
*
* ************************************************************************************************/
struct FallbackInt64 {
	int64_t v;
	typedef int64_t F;
	FallbackInt64() = default;
	FallbackInt64(int64_t a) : v(a) {};

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
	static constexpr int size_of_element() { return sizeof(int64_t); }
	static constexpr int number_of_elements() { return 1; }
	F element(int) { return v; }
	void set_element(int, F value) { v = value; }

	//*****Make Functions****
	static FallbackInt64 make_sequential(int64_t first) { return FallbackInt64(first); }


	//*****Addition Operators*****
	FallbackInt64& operator+=(const FallbackInt64& rhs) noexcept { v += rhs.v; return *this; }
	FallbackInt64& operator+=(int64_t rhs) noexcept { v += rhs; return *this; }

	//*****Subtraction Operators*****
	FallbackInt64& operator-=(const FallbackInt64& rhs) noexcept { v -= rhs.v; return *this; }
	FallbackInt64& operator-=(int64_t rhs) noexcept { v -= rhs; return *this; }

	//*****Multiplication Operators*****
	FallbackInt64& operator*=(const FallbackInt64& rhs) noexcept { v *= rhs.v; return *this; }
	FallbackInt64& operator*=(int64_t rhs) noexcept { v *= rhs; return *this; }

	//*****Division Operators*****
	FallbackInt64& operator/=(const FallbackInt64& rhs) noexcept { v /= rhs.v; return *this; }
	FallbackInt64& operator/=(int64_t rhs) noexcept { v /= rhs;	return *this; }

	//*****Negate Operators*****
	FallbackInt64 operator-() const noexcept { return FallbackInt64(-v); }

	//*****Bitwise Logic Operators*****
	FallbackInt64& operator&=(const FallbackInt64& rhs) noexcept { v &= rhs.v; return *this; }
	FallbackInt64& operator|=(const FallbackInt64& rhs) noexcept { v |= rhs.v; return *this; }
	FallbackInt64& operator^=(const FallbackInt64& rhs) noexcept { v ^= rhs.v; return *this; }

	//*****Mathematical*****

};

//*****Addition Operators*****
inline static FallbackInt64 operator+(FallbackInt64  lhs, const FallbackInt64& rhs) noexcept { lhs += rhs; return lhs; }
inline static FallbackInt64 operator+(FallbackInt64  lhs, int64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static FallbackInt64 operator+(int64_t lhs, FallbackInt64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static FallbackInt64 operator-(FallbackInt64  lhs, const FallbackInt64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackInt64 operator-(FallbackInt64  lhs, int64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static FallbackInt64 operator-(const int64_t lhs, FallbackInt64 rhs) noexcept { rhs.v = lhs - rhs.v; return rhs; }

//*****Multiplication Operators*****
inline static FallbackInt64 operator*(FallbackInt64  lhs, const FallbackInt64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackInt64 operator*(FallbackInt64  lhs, int64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static FallbackInt64 operator*(int64_t lhs, FallbackInt64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static FallbackInt64 operator/(FallbackInt64  lhs, const FallbackInt64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static FallbackInt64 operator/(FallbackInt64  lhs, int64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static FallbackInt64 operator/(const int64_t lhs, FallbackInt64 rhs) noexcept { rhs.v = lhs / rhs.v; return rhs; }


//*****Bitwise Logic Operators*****
inline static FallbackInt64 operator&(const FallbackInt64& lhs, const FallbackInt64& rhs) noexcept { return FallbackInt64(lhs.v & rhs.v); }
inline static FallbackInt64 operator|(const FallbackInt64& lhs, const FallbackInt64& rhs) noexcept { return FallbackInt64(lhs.v | rhs.v); }
inline static FallbackInt64 operator^(const FallbackInt64& lhs, const FallbackInt64& rhs) noexcept { return FallbackInt64(lhs.v ^ rhs.v); }
inline static FallbackInt64 operator~(FallbackInt64 lhs) noexcept { return FallbackInt64(~lhs.v); }


//*****Shifting Operators*****
inline static FallbackInt64 operator<<(FallbackInt64 lhs, int bits) noexcept { lhs.v <<= bits; return lhs; }
inline static FallbackInt64 operator>>(FallbackInt64 lhs, int bits) noexcept { lhs.v >>= bits; return lhs; }


//*****Min/Max*****
inline static FallbackInt64 min(FallbackInt64 a, FallbackInt64 b) { return FallbackInt64(std::min(a.v, b.v)); }
inline static FallbackInt64 max(FallbackInt64 a, FallbackInt64 b) { return FallbackInt64(std::max(a.v, b.v)); }


//*****Math Operators*****
inline static FallbackInt64 abs(FallbackInt64 a) { return FallbackInt64(std::abs(a.v)); }





//***************** x86_64 only code ******************
#if defined(_M_X64) || defined(__x86_64)
#include <immintrin.h>



/**************************************************************************************************
 * SIMD 512 type.  Contains 16 x 64bit Signed Integers
 * Requires AVX-512F
 * ************************************************************************************************/
struct Simd512Int64 {
	__m512i v;
	typedef int64_t F;

	Simd512Int64() = default;
	Simd512Int64(__m512i a) : v(a) {};
	Simd512Int64(F a) : v(_mm512_set1_epi64(a)) {};

	//*****Support Informtion*****
	static bool cpu_supported() {
		CpuInformation cpuid{};
		cpu_supported(cpuid);
	}
	static bool cpu_supported(CpuInformation cpuid) {
		return cpuid.has_avx512_dq() &&  cpuid.has_avx512_f();
	}

	//Performs a compile time support. Checks this type ONLY (integers in same class may not be supported) 
	static constexpr bool compiler_supported() {
		return mt::environment::compiler_has_avx512dq &&  mt::environment::compiler_has_avx512f;
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  
	static bool cpu_level_supported() {
		CpuInformation cpuid{};
		cpu_level_supported(cpuid);
	}

	//Performs a runtime CPU check to see if this type's microarchitecture level is supported.  
	static bool cpu_level_supported(CpuInformation cpuid) {
		return cpuid.has_avx512_f() && cpuid.has_avx512_dq() && cpuid.has_avx512_vl() && cpuid.has_avx512_bw() && cpuid.has_avx512_cd();
	}

	//Performs a compile time support to see if the microarchitecture level is supported. 
	static constexpr bool compiler_level_supported() {
		return mt::environment::compiler_has_avx512f && mt::environment::compiler_has_avx512dq && mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512bw && mt::environment::compiler_has_avx512cd;
	}

	//*****Elements*****
	static constexpr int size_of_element() { return sizeof(int64_t); }
	static constexpr int number_of_elements() { return 8; }
	F element(int i) { return v.m512i_u64[i]; }
	void set_element(int i, F value) { v.m512i_u64[i] = value; }

	//*****Make Functions****
	static Simd512Int64 make_sequential(int64_t first) { return Simd512Int64(_mm512_set_epi64(first + 7, first + 6, first + 5, first + 4, first + 3, first + 2, first + 1, first)); }


	//*****Addition Operators*****
	Simd512Int64& operator+=(const Simd512Int64& rhs) noexcept { v = _mm512_add_epi64(v, rhs.v); return *this; }
	Simd512Int64& operator+=(int64_t rhs) noexcept { v = _mm512_add_epi64(v, _mm512_set1_epi64(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd512Int64& operator-=(const Simd512Int64& rhs) noexcept { v = _mm512_sub_epi64(v, rhs.v); return *this; }
	Simd512Int64& operator-=(int64_t rhs) noexcept { v = _mm512_sub_epi64(v, _mm512_set1_epi64(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd512Int64& operator*=(const Simd512Int64& rhs) noexcept { v = _mm512_mullo_epi64(v, rhs.v); return *this; }
	Simd512Int64& operator*=(int64_t rhs) noexcept { v = _mm512_mullo_epi64(v, _mm512_set1_epi64(rhs)); return *this; }

	//*****Division Operators*****
	Simd512Int64& operator/=(const Simd512Int64& rhs) noexcept { v = _mm512_div_epi64(v, rhs.v); return *this; }
	Simd512Int64& operator/=(int64_t rhs) noexcept {		
		v = _mm512_div_epi64(v, _mm512_set1_epi64(rhs));
		return *this;
		
	}

	//*****Negate Operators*****
	Simd512Int64 operator-() const noexcept {
		return Simd512Int64(_mm512_sub_epi64(_mm512_setzero_si512(), v));
	}

	//*****Bitwise Logic Operators*****
	Simd512Int64& operator&=(const Simd512Int64& rhs) noexcept { v = _mm512_and_si512(v, rhs.v); return *this; }
	Simd512Int64& operator|=(const Simd512Int64& rhs) noexcept { v = _mm512_or_si512(v, rhs.v); return *this; }
	Simd512Int64& operator^=(const Simd512Int64& rhs) noexcept { v = _mm512_xor_si512(v, rhs.v); return *this; }




};

//*****Addition Operators*****
inline static Simd512Int64 operator+(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512Int64 operator+(Simd512Int64  lhs, int64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd512Int64 operator+(int64_t lhs, Simd512Int64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd512Int64 operator-(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512Int64 operator-(Simd512Int64  lhs, int64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd512Int64 operator-(const int64_t lhs, const Simd512Int64& rhs) noexcept { return Simd512Int64(_mm512_sub_epi64(_mm512_set1_epi64(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd512Int64 operator*(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512Int64 operator*(Simd512Int64  lhs, int64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd512Int64 operator*(int64_t lhs, Simd512Int64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd512Int64 operator/(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd512Int64 operator/(Simd512Int64  lhs, int64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd512Int64 operator/(const int64_t lhs, const Simd512Int64& rhs) noexcept { return Simd512Int64(_mm512_div_epi64(_mm512_set1_epi64(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd512Int64 operator&(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd512Int64 operator|(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd512Int64 operator^(Simd512Int64  lhs, const Simd512Int64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd512Int64 operator~(const Simd512Int64& lhs) noexcept { return Simd512Int64(_mm512_xor_epi64(lhs.v, _mm512_set1_epi64(0xFFFFFFFFFFFFFFFF))); } //No bitwise not in AVX512 so we use xor with a constant..


//*****Shifting Operators*****
inline static Simd512Int64 operator<<(const Simd512Int64& lhs, int bits) noexcept { return Simd512Int64(_mm512_slli_epi64(lhs.v, bits)); }

//Arithmatic Shift is used for signed integers
inline static Simd512Int64 operator>>(const Simd512Int64& lhs, int bits) noexcept { return Simd512Int64(_mm512_srai_epi64(lhs.v, bits)); }



//*****Min/Max*****
inline static Simd512Int64 min(Simd512Int64 a, Simd512Int64 b) { return Simd512Int64(_mm512_min_epi64(a.v, b.v)); }
inline static Simd512Int64 max(Simd512Int64 a, Simd512Int64 b) { return Simd512Int64(_mm512_max_epi64(a.v, b.v)); }


//*****Mathematical*****
inline static Simd512Int64 abs(Simd512Int64 a) { return Simd512Int64(_mm512_abs_epi64(a.v)); }

/**************************************************************************************************
 * SIMD 256 type.  Contains 8 x 64bit Signed Integers
 * Requires AVX2 support.
 * ************************************************************************************************/
struct Simd256Int64 {
	__m256i v;
	typedef int64_t F;

	Simd256Int64() = default;
	Simd256Int64(__m256i a) : v(a) {};
	Simd256Int64(F a) : v(_mm256_set1_epi64x(a)) {};

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
	static constexpr int size_of_element() { return sizeof(int64_t); }
	static constexpr int number_of_elements() { return 4; }
	F element(int i) { return v.m256i_u64[i]; }
	void set_element(int i, F value) { v.m256i_u64[i] = value; }

	//*****Addition Operators*****
	Simd256Int64& operator+=(const Simd256Int64& rhs) noexcept { v = _mm256_add_epi64(v, rhs.v); return *this; }
	Simd256Int64& operator+=(int64_t rhs) noexcept { v = _mm256_add_epi64(v, _mm256_set1_epi64x(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd256Int64& operator-=(const Simd256Int64& rhs) noexcept { v = _mm256_sub_epi64(v, rhs.v); return *this; }
	Simd256Int64& operator-=(int64_t rhs) noexcept { v = _mm256_sub_epi64(v, _mm256_set1_epi64x(rhs));	return *this; }

	//*****Multiplication Operators*****
	Simd256Int64& operator*=(const Simd256Int64& rhs) noexcept {
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
	Simd256Int64& operator*=(int64_t rhs) noexcept { *this *= Simd256Int64(_mm256_set1_epi64x(rhs)); return *this; }

	//*****Division Operators*****
	Simd256Int64& operator/=(const Simd256Int64& rhs) noexcept {
		v = _mm256_div_epi64(v, rhs.v);
	}
	Simd256Int64& operator/=(int64_t rhs) noexcept {		
		v = _mm256_div_epi64(v, _mm256_set1_epi64x(rhs));
		return *this;
	}

	//*****Negate Operators*****
	Simd256Int64 operator-() const noexcept {
		return Simd256Int64(_mm256_sub_epi64(_mm256_setzero_si256(), v));
	}

	//*****Bitwise Logic Operators*****
	Simd256Int64& operator&=(const Simd256Int64& rhs) noexcept { v = _mm256_and_si256(v, rhs.v); return *this; }
	Simd256Int64& operator|=(const Simd256Int64& rhs) noexcept { v = _mm256_or_si256(v, rhs.v); return *this; }
	Simd256Int64& operator^=(const Simd256Int64& rhs) noexcept { v = _mm256_xor_si256(v, rhs.v); return *this; }

	//*****Make Functions****
	static Simd256Int64 make_sequential(int64_t first) { return Simd256Int64(_mm256_set_epi64x( first + 3, first + 2, first + 1, first)); }


};

//*****Addition Operators*****
inline static Simd256Int64 operator+(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256Int64 operator+(Simd256Int64  lhs, int64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd256Int64 operator+(int64_t lhs, Simd256Int64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd256Int64 operator-(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256Int64 operator-(Simd256Int64  lhs, int64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd256Int64 operator-(const int64_t lhs, const Simd256Int64& rhs) noexcept { return Simd256Int64(_mm256_sub_epi64(_mm256_set1_epi64x(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd256Int64 operator*(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256Int64 operator*(Simd256Int64  lhs, int64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd256Int64 operator*(int64_t lhs, Simd256Int64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline Simd256Int64 operator/(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline Simd256Int64 operator/(Simd256Int64  lhs, int64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline Simd256Int64 operator/(const int64_t lhs, const Simd256Int64& rhs) noexcept { return Simd256Int64(_mm256_div_epi64(_mm256_set1_epi64x(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd256Int64 operator&(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd256Int64 operator|(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd256Int64 operator^(Simd256Int64  lhs, const Simd256Int64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd256Int64 operator~(const Simd256Int64& lhs) noexcept { return Simd256Int64(_mm256_xor_si256(lhs.v, _mm256_cmpeq_epi64(lhs.v, lhs.v))); } //No bitwise not in AVX2 so we use xor with a constant..


//*****Shifting Operators*****
inline static Simd256Int64 operator<<(const Simd256Int64& lhs, int bits) noexcept { return Simd256Int64(_mm256_slli_epi64(lhs.v, bits)); }
inline static Simd256Int64 operator>>(const Simd256Int64& lhs, int bits) noexcept {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd256Int64(_mm256_srai_epi64(lhs.v, bits)); //AVX-512
	}
	else {
		//No Arithmatic Shift Right for AVX2
		auto m3 = lhs.v.m256i_i64[3] >> bits;
		auto m2 = lhs.v.m256i_i64[2] >> bits;
		auto m1 = lhs.v.m256i_i64[1] >> bits;
		auto m0 = lhs.v.m256i_i64[0] >> bits;
		return Simd256Int64(_mm256_set_epi64x(m3,m2, m1, m0));
	}
}


//*****Min/Max*****
inline static Simd256Int64 min(Simd256Int64 a, Simd256Int64 b) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd256Int64(_mm256_min_epi64(a.v, b.v)); 
	}
	else {
		auto m3 = std::min(a.v.m256i_i64[3], b.v.m256i_i64[3]);
		auto m2 = std::min(a.v.m256i_i64[2], b.v.m256i_i64[2]);
		auto m1 = std::min(a.v.m256i_i64[1], b.v.m256i_i64[1]);
		auto m0 = std::min(a.v.m256i_i64[0], b.v.m256i_i64[0]);
		return Simd256Int64(_mm256_set_epi64x(m3, m2, m1, m0));
	}
}
inline static Simd256Int64 max(Simd256Int64 a, Simd256Int64 b) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd256Int64(_mm256_max_epi64(a.v, b.v)); 
	}
	else {
		auto m3 = std::max(a.v.m256i_i64[3], b.v.m256i_i64[3]);
		auto m2 = std::max(a.v.m256i_i64[2], b.v.m256i_i64[2]);
		auto m1 = std::max(a.v.m256i_i64[1], b.v.m256i_i64[1]);
		auto m0 = std::max(a.v.m256i_i64[0], b.v.m256i_i64[0]);
		return Simd256Int64(_mm256_set_epi64x(m3, m2, m1, m0));
	}
}

//*****Mathematical*****
inline static Simd256Int64 abs(Simd256Int64 a) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd256Int64(_mm256_abs_epi64(a.v));
	}
	else {
		//No AVX2
		auto m3 = std::abs(a.v.m256i_i64[3]);
		auto m2 = std::abs(a.v.m256i_i64[2]);
		auto m1 = std::abs(a.v.m256i_i64[1]);
		auto m0 = std::abs(a.v.m256i_i64[0]);
		return Simd256Int64(_mm256_set_epi64x(m3, m2, m1, m0));
	}
}








/**************************************************************************************************
*SIMD 128 type.Contains 4 x 64bit Signed Integers
* Requires SSE2 support.
* (will be faster with SSE4.1 enabled at compile time)
* ************************************************************************************************/
struct Simd128Int64 {
	__m128i v;
	typedef int64_t F;

	Simd128Int64() = default;
	Simd128Int64(__m128i a) : v(a) {};
	Simd128Int64(F a) : v(_mm_set1_epi64x(a)) {};

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
	static constexpr int size_of_element() { return sizeof(int64_t); }
	static constexpr int number_of_elements() { return 2; }
	F element(int i) { return v.m128i_u64[i]; }
	void set_element(int i, F value) { v.m128i_u64[i] = value; }

	//*****Addition Operators*****
	Simd128Int64& operator+=(const Simd128Int64& rhs) noexcept { v = _mm_add_epi64(v, rhs.v); return *this; }
	Simd128Int64& operator+=(int64_t rhs) noexcept { v = _mm_add_epi64(v, _mm_set1_epi64x(rhs));	return *this; }

	//*****Subtraction Operators*****
	Simd128Int64& operator-=(const Simd128Int64& rhs) noexcept { v = _mm_sub_epi64(v, rhs.v); return *this; }
	Simd128Int64& operator-=(int64_t rhs) noexcept { v = _mm_sub_epi64(v, _mm_set1_epi64x(rhs));	return *this; } 

	//*****Multiplication Operators*****
	Simd128Int64& operator*=(const Simd128Int64& rhs) noexcept {
		if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
			v = _mm_mullo_epi64(v, rhs.v); //AVX-512
			return *this;
		}
		else {
			v = _mm_set_epi64x(v.m128i_i64[1] * rhs.v.m128i_i64[1], v.m128i_i64[0] * rhs.v.m128i_i64[0]);
			return *this;
		}
	}

	Simd128Int64& operator*=(int64_t rhs) noexcept { *this *= Simd128Int64(_mm_set1_epi64x(rhs)); return *this; }

	//*****Division Operators*****
	Simd128Int64& operator/=(const Simd128Int64& rhs) noexcept { v = _mm_div_epi64(v, rhs.v); return *this; }
	Simd128Int64& operator/=(int64_t rhs) noexcept { v = _mm_div_epi64(v, _mm_set1_epi64x(rhs));	return *this; } //SSE

	//*****Negate Operators*****
	Simd128Int64 operator-() const noexcept {
		return Simd128Int64(_mm_sub_epi64(_mm_setzero_si128(), v));
	}

	//*****Bitwise Logic Operators*****
	Simd128Int64& operator&=(const Simd128Int64& rhs) noexcept { v = _mm_and_si128(v, rhs.v); return *this; } //SSE2
	Simd128Int64& operator|=(const Simd128Int64& rhs) noexcept { v = _mm_or_si128(v, rhs.v); return *this; }
	Simd128Int64& operator^=(const Simd128Int64& rhs) noexcept { v = _mm_xor_si128(v, rhs.v); return *this; }

	//*****Make Functions****
	static Simd128Int64 make_sequential(int64_t first) { return Simd128Int64(_mm_set_epi64x( first + 1, first)); }






};

//*****Addition Operators*****
inline static Simd128Int64 operator+(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128Int64 operator+(Simd128Int64  lhs, int64_t rhs) noexcept { lhs += rhs; return lhs; }
inline static Simd128Int64 operator+(int64_t lhs, Simd128Int64 rhs) noexcept { rhs += lhs; return rhs; }

//*****Subtraction Operators*****
inline static Simd128Int64 operator-(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128Int64 operator-(Simd128Int64  lhs, int64_t rhs) noexcept { lhs -= rhs; return lhs; }
inline static Simd128Int64 operator-(const int64_t lhs, const Simd128Int64& rhs) noexcept { return Simd128Int64(_mm_sub_epi64(_mm_set1_epi64x(lhs), rhs.v)); }

//*****Multiplication Operators*****
inline static Simd128Int64 operator*(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128Int64 operator*(Simd128Int64  lhs, int64_t rhs) noexcept { lhs *= rhs; return lhs; }
inline static Simd128Int64 operator*(int64_t lhs, Simd128Int64 rhs) noexcept { rhs *= lhs; return rhs; }

//*****Division Operators*****
inline static Simd128Int64 operator/(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs /= rhs;	return lhs; }
inline static Simd128Int64 operator/(Simd128Int64  lhs, int64_t rhs) noexcept { lhs /= rhs; return lhs; }
inline static Simd128Int64 operator/(const int64_t lhs, const Simd128Int64& rhs) noexcept { return Simd128Int64(_mm_div_epi64(_mm_set1_epi64x(lhs), rhs.v)); }


//*****Bitwise Logic Operators*****
inline static Simd128Int64 operator&(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs &= rhs; return lhs; }
inline static Simd128Int64 operator|(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs |= rhs; return lhs; }
inline static Simd128Int64 operator^(Simd128Int64  lhs, const Simd128Int64& rhs) noexcept { lhs ^= rhs; return lhs; }
inline static Simd128Int64 operator~(const Simd128Int64& lhs) noexcept { return Simd128Int64(_mm_xor_si128(lhs.v, _mm_cmpeq_epi64(lhs.v, lhs.v))); }


//*****Shifting Operators*****
inline static Simd128Int64 operator<<(const Simd128Int64& lhs, const int bits) noexcept { return Simd128Int64(_mm_slli_epi64(lhs.v, bits)); } //SSE2
//Arithmatic Shift is used for signed integers
inline static Simd128Int64 operator>>(const Simd128Int64& lhs, const int bits) noexcept { 
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd128Int64(_mm_srai_epi64(lhs.v, bits)); //AVX-512
	}
	else {
		//No Arithmatic Shift Right for SSE or AVX2
		auto m1 = lhs.v.m128i_i64[1] >> bits;
		auto m0 = lhs.v.m128i_i64[0] >> bits;
		return Simd128Int64(_mm_set_epi64x(m1, m0));
	}
}




//*****Min/Max*****
inline static Simd128Int64 min(Simd128Int64 a, Simd128Int64 b) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd128Int64(_mm_min_epi64(a.v, b.v)); //AVX-512
	}
	else {
		//No min/max or compare for Signed ints in SSE2 so we will just unroll.
		auto m1 = std::min(a.v.m128i_i64[1], b.v.m128i_i64[1]);
		auto m0 = std::min(a.v.m128i_i64[0], b.v.m128i_i64[0]);
		return Simd128Int64(_mm_set_epi64x(m1, m0));
	}
}



inline static Simd128Int64 max(Simd128Int64 a, Simd128Int64 b) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd128Int64(_mm_max_epi64(a.v, b.v));  //avx-512
	}
	else {
		//No min/max or compare for Signed ints in SSE2 so we will just unroll.
		auto m1 = std::max(a.v.m128i_i64[1], b.v.m128i_i64[1]);
		auto m0 = std::max(a.v.m128i_i64[0], b.v.m128i_i64[0]);
		return Simd128Int64(_mm_set_epi64x( m1, m0));
	}
}

//*****Mathematical*****
inline static Simd128Int64 abs(Simd128Int64 a) {
	if constexpr (mt::environment::compiler_has_avx512vl && mt::environment::compiler_has_avx512f) {
		return Simd128Int64(_mm_abs_epi64(a.v));  //avx-512
	}
	else {
		//Not supported by SSE2, so we need to emulate it.
		//This clever little code sequence is thanks to Agner Fog.
		const auto sign = _mm_srai_epi64(a.v, 31); //shift right moving in sign bits
		const auto inv = _mm_xor_si128(a.v, sign);   // invert bits if negative
		const auto result = _mm_sub_epi64(inv, sign); //add 1 if needed
		return Simd128Int64(result);
	}
}


#endif //x86_64





/**************************************************************************************************
 * Check that each type implements the desired types from simd-concepts.h
 * (Not sure why this fails on intelisense, but compliles ok.)
 * ************************************************************************************************/
static_assert(Simd<FallbackInt64>, "FallbackInt64 does not implement the concept Simd");
static_assert(SimdSigned<FallbackInt64>, "FallbackInt64 does not implement the concept SimdSigned");
static_assert(SimdInt<FallbackInt64>, "FallbackInt64 does not implement the concept SimdInt");
static_assert(SimdInt64<FallbackInt64>, "FallbackInt64 does not implement the concept SimdInt64");

#if defined(_M_X64) || defined(__x86_64)
static_assert(Simd<Simd128Int64>, "Simd128Int64 does not implement the concept Simd");
static_assert(Simd<Simd256Int64>, "Simd256Int64 does not implement the concept Simd");
static_assert(Simd<Simd512Int64>, "Simd512Int64 does not implement the concept Simd");

static_assert(SimdSigned<Simd128Int64>, "Simd128Int64 does not implement the concept SimdSigned");
static_assert(SimdSigned<Simd256Int64>, "Simd256Int64 does not implement the concept SimdSigned");
static_assert(SimdSigned<Simd512Int64>, "Simd512Int64 does not implement the concept SimdSigned");

static_assert(SimdInt<Simd128Int64>, "Simd256Int64 does not implement the concept SimdInt");
static_assert(SimdInt<Simd256Int64>, "Simd256Int64 does not implement the concept SimdInt");
static_assert(SimdInt<Simd512Int64>, "Simd512Int64 does not implement the concept SimdInt");



static_assert(SimdInt64<Simd128Int64>, "Simd128Int64 does not implement the concept SimdInt64");
static_assert(SimdInt64<Simd256Int64>, "Simd256Int64 does not implement the concept SimdInt64");
static_assert(SimdInt64<Simd512Int64>, "Simd512Int64 does not implement the concept SimdInt64");
#endif



/**************************************************************************************************
 Define SimdNativeInt64 as the best supported type at compile time.
*************************************************************************************************/
#if defined(_M_X64) || defined(__x86_64)
#if defined(__AVX512F__) && defined(__AVX512DQ__) 
typedef Simd512Int64 SimdNativeInt64;
#else
#if defined(__AVX2__) && defined(__AVX__) 
typedef Simd256Int64 SimdNativeInt64;
#else
#if defined(__SSE4_1__) && defined(__SSE4_1__) && defined(__SSE3__) && defined(__SSSE3__) 
typedef Simd128Int64 SimdNativeInt64;
#else
typedef Simd128Int64 SimdNativeInt64;
#endif	
#endif	
#endif
#else
typedef FallbackInt64 SimdNativeInt64;
#endif
