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

	A helper class for managing After Effects Parameters.

*******************************************************************************************************/
#pragma once
#include "after-effects-sdk.h"
#include "parameter-id.h"
#include <array>
#include <string>
#include <vector>

class ParameterHelper {
private:
	static int paramsAdded;  ///Counter used to track parameters as they are added .
	static std::array<int, static_cast<int>(ParameterID::__last)> paramTranslate; ///To translate param ID to location

public:
	static void AddSlider(ParameterID id, const std::string& name, float min, float max, float sliderMin, float sliderMax, float value, short precision);
	static void AddCheckBox(ParameterID id, const std::string& name, const std::string& comment, bool value);
	static void AddButton(ParameterID id, const std::string& name, const std::string& buttonText);
	static void AddList(ParameterID id, const std::string& name, const std::string& choices, short value, bool supervise = false);
	static void AddGroupStart(ParameterID id, const std::string name);
	static void AddGroupEnd(ParameterID id);
	static void AddColour(ParameterID id, const std::string& name, unsigned char red, unsigned char green, unsigned char blue);
	static void AddAngle(ParameterID id, const std::string& name, double value);
	static void AddLayer(ParameterID id, const std::string& name);

	static double ReadSlider(ParameterID paramID);
	static double ReadAngle(ParameterID paramID);
	static long ReadList(ParameterID paramID);
	static std::string ReadListAsString(ParameterID paramID, const std::vector<std::string>& list);
	static bool ReadCheckBox(ParameterID paramID);
	static PF_Pixel ReadColour(ParameterID paramID);
	static long ReadLayerLocation(ParameterID paramID);
	static int GetNumberParameters() noexcept { return paramsAdded; }
	static int GetParameterLocation(ParameterID id);

	static void EnableParameter(ParameterID id, const PF_ParamDef* const params[]);
	static void DisableParameter(ParameterID id, const PF_ParamDef* const params[]);

private:
	static void addToLookup(ParameterID id) noexcept;
};