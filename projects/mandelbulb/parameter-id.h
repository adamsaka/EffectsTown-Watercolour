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

	A list of parameterID to refer to each parameter.

	After Effects requires that ID remain the same accross different versions.
	So, do not remove ununsed parameters from list, just add new ones.

	Actual specification for project parameters in parameters.cpp


*******************************************************************************************************/
#pragma once



enum class ParameterID {
	input = 0,	   //Reserve ID zero (for AE).
	seed,		   //Reserved for Random Seed.
	seed_button,   //Reserved
	seed_int,	   //Reserved
	render_precision, //Reserved

	//Input Transforms.  Should keep in enum so code compiles, order only needs to remain the same for this project.
	input_transform_group_start = 100,
	input_transform_group_end,
	input_transform_type,
	input_transform_scale,
	input_transform_rotation,
	input_transform_translate_x,
	input_transform_translate_y,
	input_transform_special1,
	input_transform_special2,
	input_transform_special3,
	input_transform_special4,
	

	//Project
	debug_group_start = 1000,
	debug_group_end,
	debug_display,
	background_colour,
	background_group_start,
	background_group_end,
	fractal_group_start,
	fractal_group_end,
	fractal_fractal,
	fractal_seedx,
	fractal_seedy,
	fractal_seedz,
	fractal_iterations,
	render_quality,
	render_group_start,
	render_group_end,	
	render_epsilon,
	camera_position_x,
	camera_position_y,
	camera_position_z,
	camera_lookat_x,
	camera_lookat_y,
	camera_lookat_z,
	camera_magnification,
	camera_group_start,
	camera_group_end,
	render_step_limit,
	material_group_start,
	material_group_end,
	material_diffuse,
	material_metallic,
	material_roughness,
	lighting_group_start,
	lighting_group_end,
	lighting_ambient_colour,
	lighting_ambient_intensity,
	lighting_light1_group_start,
	lighting_light1_group_end,
	lighting_light1_direction_x,
	lighting_light1_direction_y,
	lighting_light1_direction_z,
	lighting_light1_colour,
	lighting_light1_intensity,
	lighting_light2_group_start,
	lighting_light2_group_end,
	lighting_light2_direction_x,
	lighting_light2_direction_y,
	lighting_light2_direction_z,
	lighting_light2_colour,
	lighting_light2_intensity,
	lighting_light3_group_start,
	lighting_light3_group_end,
	lighting_light3_direction_x,
	lighting_light3_direction_y,
	lighting_light3_direction_z,
	lighting_light3_colour,
	lighting_light3_intensity,






	__last  //Must be last (used for array memory allocation)
};

constexpr int parameter_id_to_int(ParameterID p) noexcept { return static_cast<int>(p); }


