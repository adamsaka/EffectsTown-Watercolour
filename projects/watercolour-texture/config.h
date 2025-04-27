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

	Project configuration


*******************************************************************************************************/
#pragma once



//**WARNING**: ALSO update these constants in AE.r.  They must match.
#define PluginName					"Watercolour Texture"
#define PluginMenu					"Effects Town"
#define PluginIdentifier			"Town.Effects.WatercolourTexture"
#define	PluginMajorVersion			1
#define	PluginMinorVersion			0
#define	PluginBugVersion			0
#define	PluginBuildVersion			1

constexpr bool project_is_generator = true;      // Project can operate in generator context (with no input)
constexpr bool project_uses_input = false;         // Does the project accept an input image.  (Effect & General context in OpenFX)
constexpr bool project_overlay_on_input = false;  // Does the project perform a transparent render that needs to be overlayed on the input afterwards.

//Indicates that a project will not return any transparent pixels.
constexpr bool project_is_solid_render = true;

//Floating point precesion to use for this project.
constexpr bool project_is_multiprecision = false;
typedef float Precision;








/********************* NEW PROJECT CHECKLIST *****************************
* How to copy a project:
*
* 1. Copy and rename visual studio project folder.
* 2. Copy ..\..\projects folder.
* 3. Add existing project to VS and rename.
* 4. Set custom build for ac.r
* 5. Rename plug-in within ac.r & this file to match.
* 6. Change location of include to point to new project folder.
* 7. makefile for wasm builds.
*
*
*
*
*
* ********************************************************************/