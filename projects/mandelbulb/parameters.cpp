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

	This is the list of parameters that will actually be displayed to the user.  
	This function and list is host-independant.

	
	Each entry must have a unique parameter-id

	After Effects requires that ID remain the same accross different versions.
	So, do not remove ununsed parameter-ids, just add new ones.

	Add ParameterIF::seed if you'd like to expose the random seed selection to user.

*******************************************************************************************************/

#include "parameters.h"
#include "parameter-id.h" 
//#include "..\..\common\input-transforms.h"

ParameterList build_project_parameters() {
	ParameterList params;
	//params.add_entry(ParameterEntry::make_seed(ParameterID::seed, "Random Seed"));


	params.add_entry(ParameterEntry::make_group_start(ParameterID::render_group_start, "Quality vs Performance"));
	std::vector<std::string> list_quality;
	list_quality.push_back("Draft");
	list_quality.push_back("Low");
	list_quality.push_back("Medium");
	list_quality.push_back("High");
	params.add_entry(ParameterEntry::make_list(ParameterID::render_quality, "Render Quality", list_quality));
	list_quality.clear();
	list_quality.push_back("32-Bit");
	list_quality.push_back("64-Bit");
	params.add_entry(ParameterEntry::make_list(ParameterID::render_precision, "Floating Point", list_quality));
	params.add_entry(ParameterEntry::make_number(ParameterID::render_epsilon, "Epislon (Exponential)", 0.0, 30.0, 3.0, 0.0, 30.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::render_step_limit, "Step Multiplier", 0.001, 1.0, 0.9, 0.001, 1.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::render_group_end));

	//Fractal Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::fractal_group_start, "Fractal"));
	std::vector<std::string> list_fractal;
	list_fractal.push_back("Mandelbulb Power 2");
	list_fractal.push_back("Mandelbulb Julia Power 2");
	list_fractal.push_back("Mandelbulb Power 3");
	list_fractal.push_back("Mandelbulb Julia Power 3");
	list_fractal.push_back("Mandelbulb Power 4");
	list_fractal.push_back("Mandelbulb Julia Power 4");
	list_fractal.push_back("Mandelbulb Power 5");
	list_fractal.push_back("Mandelbulb Julia Power 5");
	list_fractal.push_back("Mandelbulb Power 6");
	list_fractal.push_back("Mandelbulb Julia Power 6");
	list_fractal.push_back("Mandelbulb Power 7");
	list_fractal.push_back("Mandelbulb Julia Power 7");
	list_fractal.push_back("Mandelbulb Power 8");
	list_fractal.push_back("Mandelbulb Julia Power 8");
	params.add_entry(ParameterEntry::make_list(ParameterID::fractal_fractal, "Fractal", list_fractal));
	params.add_entry(ParameterEntry::make_number(ParameterID::fractal_seedx, "Seed x", -3.0, 3.0, 0.0, -3.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::fractal_seedy, "Seed y", -3.0, 3.0, 0.0, -3.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::fractal_seedz, "Seed z", -3.0, 3.0, 0.0, -3.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::fractal_iterations, "Iterations", 0.0, 10000, 50.0, 0.0, 500.0, 0));

	params.add_entry(ParameterEntry::make_group_end(ParameterID::fractal_group_end));

	//Camera Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::camera_group_start, "Camera"));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_magnification, "Magnification (Exponential)", 0.001, 100.0, 1.0, 0.001, 100.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_position_x, "Position x", -15.0, 15.0, 0.0, -15.0, 15.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_position_y, "Position y", -15.0, 15.0, 0.0, -15.0, 15.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_position_z, "Position z", -15.0, 15.0, -5.0, -15.0, 15.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_lookat_x, "Look-at x", -5.0, 5.0, 0.0, -5.0, 5.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_lookat_y, "Look-at y", -5.0, 5.0, 0.0, -5.0, 5.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::camera_lookat_z, "Look-at z", -5.0, 5.0, 0.0, -5.0, 5.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::camera_group_end));

	//Material Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::material_group_start, "Material"));
	params.add_entry(ParameterEntry::make_colour(ParameterID::material_diffuse, "Colour", 0.4f, 0.4f, 1.0f));
	params.add_entry(ParameterEntry::make_number(ParameterID::material_metallic, "Metallic", 0.0,1.0,0.0,0.0,1.0,3));
	params.add_entry(ParameterEntry::make_number(ParameterID::material_roughness, "Roughness", 0.0, 1.0, 0.5, 0.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::material_group_end));


	//Lighting Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::lighting_group_start, "Lighting"));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_ambient_intensity, "Ambient Intensity", 0.0, 1.0, 0.05, 0.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_colour(ParameterID::lighting_ambient_colour, "Ambient Colour", 1.0f, 1.0f, 1.0f));
	
	params.add_entry(ParameterEntry::make_group_start(ParameterID::lighting_light1_group_start, "Directional Light 1"));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light1_intensity, "Light 1 Intensity", 0.0, 3.0, 1.2, 0.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_colour(ParameterID::lighting_light1_colour, "Light 1 Colour", 1.0f, 1.0f, 1.0f));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light1_direction_x, "Light 1 Direction x", -1.0, 1.0, 1.0, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light1_direction_y, "Light 1 Direction y", -1.0, 1.0, 0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light1_direction_z, "Light 1 Direction z", -1.0, 1.0, -0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::lighting_light1_group_end));

	params.add_entry(ParameterEntry::make_group_start(ParameterID::lighting_light2_group_start, "Directional Light 2"));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light2_intensity, "Light 2 Intensity", 0.0, 3.0, 0.0, 0.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_colour(ParameterID::lighting_light2_colour, "Light 2 Colour", 1.0f, 1.0f, 1.0f));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light2_direction_x, "Light 2 Direction x", -1.0, 1.0, 1.0, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light2_direction_y, "Light 2 Direction y", -1.0, 1.0, 0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light2_direction_z, "Light 2 Direction z", -1.0, 1.0, -0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::lighting_light2_group_end));

	params.add_entry(ParameterEntry::make_group_start(ParameterID::lighting_light3_group_start, "Directional Light 3"));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light3_intensity, "Light 3 Intensity", 0.0, 3.0, 0.0, 0.0, 3.0, 3));
	params.add_entry(ParameterEntry::make_colour(ParameterID::lighting_light3_colour, "Light 3 Colour", 1.0f, 1.0f, 1.0f));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light3_direction_x, "Light 3 Direction x", -1.0, 1.0, 1.0, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light3_direction_y, "Light 3 Direction y", -1.0, 1.0, 0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_number(ParameterID::lighting_light3_direction_z, "Light 3 Direction z", -1.0, 1.0, -0.4, -1.0, 1.0, 3));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::lighting_light3_group_end));
	
	params.add_entry(ParameterEntry::make_group_end(ParameterID::lighting_group_end));



	//Background Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::background_group_start, "Background"));
	params.add_entry(ParameterEntry::make_colour(ParameterID::background_colour, "Background Colour", 1.0f, 1.0f, 1.0f));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::background_group_end));



	//Debug Group
	params.add_entry(ParameterEntry::make_group_start(ParameterID::debug_group_start, "Debug"));
	std::vector<std::string> list_debug;
	list_debug.push_back("None");
	list_debug.push_back("Depth * 0.1");
	list_debug.push_back("Normal");
	list_debug.push_back("Step Count - Direct");
	list_debug.push_back("Diffuse");
	list_debug.push_back("Specular");
	params.add_entry(ParameterEntry::make_list(ParameterID::debug_display, "Display", list_debug));
	params.add_entry(ParameterEntry::make_group_end(ParameterID::debug_group_end));


	//Input Transforms (builds from common set used in multiple projects)
	//build_input_transforms_parameter_list(params);

	return params;
}