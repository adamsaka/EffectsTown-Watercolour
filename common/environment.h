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
* 
* Sets up environmental variables
* 
* 
*********************************************************************************************************/
#pragma once

namespace mt::environment {




//Setup some compiler constants
#if defined(_MSC_VER)
	constexpr static bool is_visual_studio = true;
#else
	constexpr bool is_visual_studio = false;
#endif

#if defined(__clang__)
	constexpr bool is_clang = true;
#else
	constexpr static bool is_clang = false;
#endif

#if defined(__GNUC__)
	constexpr bool is_gcc = true;
#else
	constexpr static bool is_gcc = false;
#endif


#if defined(__EMSCRIPTEN__)
	constexpr bool emsctipten = true;
#else
	constexpr static bool is_emsctipten = false;
#endif 


//Check the arhitecture
#if defined(_M_X64) || defined(__x86_64)
	constexpr static bool is_x64 = true;
#else
	constexpr static bool is_x64 = false;
#endif


//MSVC++ does not define SSE macros 
#if (defined(_MSC_VER ) && defined(_M_X64))
	#if !defined(__SSE__)
		#define __SSE__ 1
	#endif
	#if !defined(__SSE2__)
		#define __SSE2__ 1
	#endif

	// I don't know a way to detect SSE3 and SSE4 and compile time, but it exists if AVX does
	#if defined(__AVX__)
		#define __SSE3__ 1
		#define __SSSE3__ 1
		#define __SSE4_1__ 1
		#define __SSE4_2__ 1
	#endif
#endif

//FMA is not defined by visual studio, so we need AVX2 support to compile with it.
#if !defined(__FMA__) && defined(__AVX2__)
	#define __FMA__ 1
#endif


//Setup some constexpr variables that we can use to provide some consistancy with different compilers.

#if defined(__SSE__) || defined(__AVX__)
	constexpr static bool compiler_has_sse = true;
#else
	constexpr static bool compiler_has_sse = false;
#endif

#if defined(__SSE2__) || defined(__AVX__)
	constexpr static bool compiler_has_sse2 = true;
#else
	constexpr static bool compiler_has_sse2 = false;
#endif

#if defined(__SSE3__) || defined(__AVX__)
	constexpr static bool compiler_has_sse3 = true;
#else
	constexpr static bool compiler_has_sse3 = false;
#endif

#if defined(__SSSE3__) || defined(__AVX__)
	constexpr static bool compiler_has_ssse3 = true;
#else
	constexpr static bool compiler_has_ssse3 = false;
#endif

#if defined(__SSE4_1__) || defined(__AVX__)
	constexpr static bool compiler_has_sse4_1 = true;
#else
	constexpr static bool compiler_has_sse4_1 = false;
#endif

#if defined(__SSE4_2__) || defined(__AVX__)
	constexpr static bool compiler_has_sse4_2 = true;
#else
	constexpr static bool compiler_has_sse4_2 = false;
#endif

#if defined(__AVX__)
	constexpr static bool compiler_has_avx = true;
#else
	constexpr static bool compiler_has_avx = false;
#endif

#if defined(__AVX2__)
	constexpr static bool compiler_has_avx2 = true;
#else
	constexpr static bool compiler_has_avx2 = false;
#endif

#if defined(__FMA__)
	constexpr static bool compiler_has_fma = true;
#else
	constexpr static bool compiler_has_fma = false;
#endif

#if defined(__AVX512F__)
	constexpr static bool compiler_has_avx512f = true;
#else
	constexpr static bool compiler_has_avx512f = false;
#endif

#if defined(__AVX512DQ__)
	constexpr static bool compiler_has_avx512dq = true;
#else
	constexpr static bool compiler_has_avx512dq = false;
#endif

#if defined(__AVX512VL__)
	constexpr static bool compiler_has_avx512vl = true;
#else
	constexpr static bool compiler_has_avx512vl = false;
#endif


#if defined(__AVX512BW__)
	constexpr static bool compiler_has_avx512bw = true;
#else
	constexpr static bool compiler_has_avx512bw = false;
#endif

#if defined(__AVX512CD__)
	constexpr static bool compiler_has_avx512cd = true;
#else
	constexpr static bool compiler_has_avx512cd = false;
#endif









}


 


