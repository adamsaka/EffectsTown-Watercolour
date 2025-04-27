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

Description:

********************************************************************************************************/
#pragma once

#include <string>

//A helper to format a pretty string, and throw it as an exception
void ThrowNullPointer(const std::string& file, const std::string& function, int line);
void ThrowPrettyException(const std::string& file, const std::string& function, int line, const std::string& message);


//Check if a value is null, and throw it it is.
#define check_null(EXPRESSION) if(!(EXPRESSION)) { ThrowNullPointer( __FILE__,__FUNCTION__, __LINE__); }
#define assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { ThrowPrettyException( __FILE__,__FUNCTION__, __LINE__, MESSAGE); }


void dev_log(std::string s) ;


inline static std::string truefalse(int i) {
	if (i) return "true";
	return "false";
}