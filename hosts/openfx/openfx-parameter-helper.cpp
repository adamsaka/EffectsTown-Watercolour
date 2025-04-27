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

	A helper class for managing OpenFX Parameters.

	Also helps keep of relationship between parameter_id and OpenFX paramter data.

*******************************************************************************************************/

#include "openfx-parameter-helper.h"

/********************************************************************************************************
* Constructor
*******************************************************************************************************/
ParameterHelper::ParameterHelper() {
	param_is_added.fill(false);
	param_handle.fill(nullptr);
}

/********************************************************************************************************
* Get the handle for each parameter so we don't have to do it each render.
* Must be called before any parameters can be read.
*******************************************************************************************************/
void ParameterHelper::load_handles() {
	check_null(paramset);
	for (size_t i = 0; i < param_is_added.size(); i++) {
		if (param_is_added.at(i)) {
			global_ParameterSuite->paramGetHandle(paramset, param_name.at(i).c_str(), &param_handle.at(i), 0);
		}
	}
}

/********************************************************************************************************
* Add a slider parmater.
* Should be called in "Describe in Context" action.
* Ensure set_paramset() is called first.
*******************************************************************************************************/
void ParameterHelper::add_slider(ParameterID id, const std::string& name, double min, double max, double slider_min, double slider_max, double value, short precision) {
	check_null(paramset);

	//Add the parameter
	OfxPropertySetHandle param_properties{};
	global_ParameterSuite->paramDefine(paramset, kOfxParamTypeDouble, name.c_str(), &param_properties);

	//Set the properties of this parameter
	global_PropertySuite->propSetString(param_properties, kOfxParamPropDoubleType, 0, kOfxParamDoubleTypeScale); 
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropDefault,	0, value);   
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropMin, 0, min);		 
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropMax, 0, max);
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropDisplayMin, 0, slider_min);
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropDisplayMax, 0, slider_max);
	global_PropertySuite->propSetDouble(param_properties, kOfxParamPropDigits, 0, precision);

	
	//Add to lookup
	param_is_added.at(parameter_id_to_int(id)) = true;
	param_name.at(parameter_id_to_int(id)) = name;

}

/********************************************************************************************************
* Reads a value from a slider
* Assumes load_handles() has been called ealier. (should be called in create instance action)
*******************************************************************************************************/
double ParameterHelper::read_slider(ParameterID id, OfxTime time) {
	double value{};
	global_ParameterSuite->paramGetValueAtTime(param_handle.at(parameter_id_to_int(id)), time, &value);
	return value;
}



/********************************************************************************************************
* Add an integer parmater.
* Should be called in "Describe in Context" action.
* Ensure set_paramset() is called first.
*******************************************************************************************************/
void ParameterHelper::add_integer(ParameterID id, const std::string& name, int min, int max , int slider_min , int slider_max , int value) {
	check_null(paramset);

	//Add the parameter
	OfxPropertySetHandle param_properties{};
	global_ParameterSuite->paramDefine(paramset, kOfxParamTypeInteger, name.c_str(), &param_properties);

	//Set the properties of this parameter
	global_PropertySuite->propSetString(param_properties, kOfxParamPropDoubleType, 0, kOfxParamDoubleTypeScale);
	global_PropertySuite->propSetInt(param_properties, kOfxParamPropDefault, 0, value);
	global_PropertySuite->propSetInt(param_properties, kOfxParamPropMin, 0, min);
	global_PropertySuite->propSetInt(param_properties, kOfxParamPropMax, 0, max);
	global_PropertySuite->propSetInt(param_properties, kOfxParamPropDisplayMin, 0, slider_min);
	global_PropertySuite->propSetInt(param_properties, kOfxParamPropDisplayMax, 0, slider_max);
	
	


	//Add to lookup
	param_is_added.at(parameter_id_to_int(id)) = true;
	param_name.at(parameter_id_to_int(id)) = name;

}
/********************************************************************************************************
* Read an integer parmater.
*******************************************************************************************************/
int ParameterHelper::read_integer(ParameterID id, OfxTime time) {
	int value{};
	global_ParameterSuite->paramGetValueAtTime(param_handle.at(parameter_id_to_int(id)), time, &value);
	return value;
}


/********************************************************************************************************
* Add a list.
*******************************************************************************************************/
void ParameterHelper::add_list(ParameterID id, const std::string& name, const std::vector<std::string>& list) {
	check_null(paramset);

	//Add the parameter
	OfxPropertySetHandle param_properties{};
	global_ParameterSuite->paramDefine(paramset, kOfxParamTypeChoice, name.c_str(), &param_properties);

	//Add the list items
	int item_number{};
	for (const auto& item : list) {
		global_PropertySuite->propSetString(param_properties, kOfxParamPropChoiceOption, item_number++, item.c_str());
	}

	//Add to lookup
	param_is_added.at(parameter_id_to_int(id)) = true;
	param_name.at(parameter_id_to_int(id)) = name;
}

/********************************************************************************************************
* Read a list parmater.
*******************************************************************************************************/
int ParameterHelper::read_list(ParameterID id, OfxTime time) {
	int value;
	global_ParameterSuite->paramGetValueAtTime(param_handle.at(parameter_id_to_int(id)), time, &value);
	return value;
}
