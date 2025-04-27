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

    The host independant renderer for the project.

*******************************************************************************************************/
#pragma once

#include <concepts>
#include <string>
#include <vector>
#include <numbers>
#include <typeinfo>
#include <memory>

#include "../../common/colour.h"
#include "../../common/linear-algebra.h"
#include "../../common/noise.h"
#include "../../common/parameter-list.h"
#include "..\..\common\input-transforms.h"

#include "..\..\common\simd-cpuid.h"
#include "..\..\common\simd-f32.h"
#include "..\..\common\simd-uint32.h"
#include "..\..\common\simd-concepts.h"

#include "..\..\common\ray3d-shapes.h"
#include "..\..\common\ray3d.h"

template <SimdFloat S> Scene<S> build_scene();


/**************************************************************************************************
 * The renderer class.
 * Implements a host independent pixel renderer.
 * Use type parameter to select floating point precision.
 * ************************************************************************************************/
template <SimdFloat S>
class Renderer{
    

    private:
        int width {};
        int height {};
        S::F width_f {};
        S::F height_f {};
        S::F aspect {};
        std::string seed_string{};
        uint32_t seed{};
        ParameterList params{};
        int quality{};

        Scene<S> scene{};

    public:
        //Constructor
        Renderer() noexcept {}

        //Size
        void set_size(int width, int height) noexcept;
        int get_width() const  { return width;}
        int get_height() const { return height;}

        //Set the seed as a string (an integer seed will be calculated)
        void set_seed(const std::string & s){
            this->seed=string_to_seed(s);             
            this->seed_string = s; 
        }
        //Set an integer seed. (string will be ignored)
        void set_seed_int(uint32_t s){
            this->seed  = s;
        }
        std::string get_seed() const { return seed_string;}
        uint32_t get_seed_int() const { return seed;}
        
        //Parameters
        void set_parameters(ParameterList plist){
            params = plist;
            const auto debug_string = params.get_string(ParameterID::debug_display);
            debug_none = (debug_string == "None");
            debug_show_depth = (debug_string == "Depth * 0.1");
            debug_show_normal = (debug_string == "Normal") ;
            debug_show_stepcount = (debug_string == "Step Count - Direct") ;
            debug_show_diffuse = (debug_string == "Diffuse");
            debug_show_specular = (debug_string == "Specular") ;
            
            const auto q = params.get_string(ParameterID::render_quality);
            if (q == "Draft") quality = 0;
            if (q == "Low") quality = 1;
            if (q == "Medium") quality = 2;
            if (q == "High") quality = 3;
            global_quality = quality;

            const auto epsilon = params.get_value(ParameterID::render_epsilon);
            global_min_ray_distance = 0.1 * pow(10, -epsilon);

            //Setup Scene
            this->scene = build_scene<S>(plist);
        }



        //Render
        ColourRGBA<S> render_pixel(S x, S y) const;
        ColourRGBA<S> render_pixel_with_input(S x, S y, ColourRGBA<S>) const;
        ColourRGBA<S> sample_pixel(S x, S y) const;

    private:


};


/**************************************************************************************************
 * Set the size of the image to render in pixels.
 * ************************************************************************************************/
template <SimdFloat S>
void Renderer<S>::set_size(int w, int h) noexcept {
    this->width = w;
    this->height = h;
    this->width_f = static_cast<S::F>(w);
    this->height_f = static_cast<S::F>(h);
    if (height==0) return;
    this->aspect = width_f/height_f;
}







/**************************************************************************************************
 * The scene is assembled here.
 *************************************************************************************************/
template <SimdFloat S>
Scene<S> build_scene(ParameterList plist) {
    id_selector.reset();
    Scene<S> scene {};
    
    //Set Background
    scene.background_colour = plist.get_colour(ParameterID::background_colour);
    
    //Setup Lights
    scene.ambient = plist.get_colour(ParameterID::lighting_ambient_colour) * plist.get_valuef(ParameterID::lighting_ambient_intensity);
    scene.lights.push_back(DirectionalLight<S>(
        plist.get_value(ParameterID::lighting_light1_direction_x),
        plist.get_value(ParameterID::lighting_light1_direction_y),
        plist.get_value(ParameterID::lighting_light1_direction_z),
        plist.get_value(ParameterID::lighting_light1_intensity),
        plist.get_colour(ParameterID::lighting_light1_colour)
        
    ));
    if (plist.get_value(ParameterID::lighting_light2_intensity) > 0.0009) {
        scene.lights.push_back(DirectionalLight<S>(
            plist.get_value(ParameterID::lighting_light2_direction_x),
            plist.get_value(ParameterID::lighting_light2_direction_y),
            plist.get_value(ParameterID::lighting_light2_direction_z),
            plist.get_value(ParameterID::lighting_light2_intensity),
            plist.get_colour(ParameterID::lighting_light2_colour)

        ));
    }
    if (plist.get_value(ParameterID::lighting_light3_intensity) > 0.0009) {
        scene.lights.push_back(DirectionalLight<S>(
            plist.get_value(ParameterID::lighting_light3_direction_x),
            plist.get_value(ParameterID::lighting_light3_direction_y),
            plist.get_value(ParameterID::lighting_light3_direction_z),
            plist.get_value(ParameterID::lighting_light3_intensity),
            plist.get_colour(ParameterID::lighting_light3_colour)
        ));
    }

    //Setup Fractal Object
    auto multi = std::make_unique<MultiShape<S>>();
    
    Material<S> material_fractal {};
    auto colour = plist.get_colour(ParameterID::material_diffuse);
    material_fractal.colour = { S{colour.red}, S{ colour.green }, S{ colour.blue } };
    material_fractal.roughness = plist.get_valuef(ParameterID::material_roughness);
    material_fractal.metalness = plist.get_valuef(ParameterID::material_metallic);

    auto x = plist.get_value(ParameterID::fractal_seedx);
    auto y = plist.get_value(ParameterID::fractal_seedy);
    auto z = plist.get_value(ParameterID::fractal_seedz);
    auto i = plist.get_value_integer(ParameterID::fractal_iterations);
    const float sl = static_cast<float>(plist.get_value(ParameterID::render_step_limit));
    const auto fractal = plist.get_string(ParameterID::fractal_fractal);
    if (fractal == "Mandelbulb Power 2") {
        multi->shapes.push_back(std::make_unique<MandelbulbPower2<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Julia Power 2") {
        multi->shapes.push_back(std::make_unique<MandelbulbJuliaPower2<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Power 3") {
        multi->shapes.push_back(std::make_unique<MandelbulbPower3<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Julia Power 3") {
        multi->shapes.push_back(std::make_unique<MandelbulbJuliaPower3<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Power 4") {
        multi->shapes.push_back(std::make_unique<MandelbulbPower4<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Julia Power 4") {
        multi->shapes.push_back(std::make_unique<MandelbulbJuliaPower4<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Power 5") {
        multi->shapes.push_back(std::make_unique<MandelbulbPower5<S>>(x, y, z, i, material_fractal, sl));
    }
    else if (fractal == "Mandelbulb Julia Power 5") {
        multi->shapes.push_back(std::make_unique<MandelbulbJuliaPower5<S>>(x, y, z, i, material_fractal, sl));
    }

   

    scene.base_shape = std::move(multi);



    return scene;
}


/**************************************************************************************************
 * Sample a pixel (or batch of pixels if using SIMD)
 *
 * ************************************************************************************************/
template <SimdFloat S>
ColourRGBA<S> Renderer<S>::sample_pixel(S x, S y) const {
    //Normalise to range: Hight = -1..1  Width = proportional zero centered.
    vec2<S> p(aspect * (2.0f * x / width_f - 1.0f), 2.0f * y / height_f - 1.0f);

    //Flip y, (as negative y is up in screen space, but we want positive y as up in 3D space)
    p.y = -p.y;

    //Setup Camera
    //Note:     x = left to right
    //          y = bottom to top
    //          z = front to back  
    
    auto eye = vec3<S>{ 
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_position_x))),
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_position_y))),
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_position_z)))
    };
    
    auto lookat = vec3<S>{ 
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_lookat_x))),
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_lookat_y))),
        S(static_cast<typename S::F>(this->params.get_value(ParameterID::camera_lookat_z)))
    };
    
    const auto up = vec3<S>{ 0.0, 1.0, 0.0 };
    
    auto mag = static_cast<typename S::F>(this->params.get_value(ParameterID::camera_magnification)); //Get magnification.  (This is an exponential so we can zoom smoothly)
    const auto rd = calculate_ray_direction(eye, lookat, up, S( 25.0f / pow(2.0f, mag) ), p);
    

    //Perform the actual ray-marching.
    auto [colour, hit_object_mask] = get_colour_by_ray_marching(scene, eye, rd);


    if (!(debug_show_stepcount || debug_show_normal || debug_show_depth)) {
        //HDR
        colour = ColourRGBA<S>(colour.red / (1 + colour.red), colour.green / (1 + colour.green), colour.blue / (1 + colour.blue), colour.alpha);

        //Gamma
        colour = ColourRGBA<S>(pow(colour.red, 1.0f / 2.2f), pow(colour.green, 1.0f / 2.2f), pow(colour.blue, 1.0f / 2.2f));
    }

    //Blend background colour for rays that didn't hit a pixel.
    const auto background = (debug_show_normal) ? ColourRGBA<S>{ S(0.5), S(0.5), S(1.0) } : scene.background_colour;
    colour.red = blend(background.red, colour.red, hit_object_mask);
    colour.green = blend(background.green, colour.green, hit_object_mask);
    colour.blue = blend(background.blue, colour.blue, hit_object_mask);

    return colour;
}


/**************************************************************************************************
 * Render a pixel (or batch of pixels if using SIMD)
 * 
 * ************************************************************************************************/
template <SimdFloat S>
ColourRGBA<S> Renderer<S>::render_pixel(S x, S y) const {
    if (!this->scene.base_shape) [[unlikely]] return ColourRGBA<S>{};
    if (width <=0 || height <=0) [[unlikely]] return ColourRGBA<S>{};
    next_random<typename S::F>(seed); //Reset random seed so it is the same for each pixel
    
    global_pass = 0;
    auto colour = sample_pixel(x, y);
    //if (!debug_none) return colour;

    if (quality >= 1) {
        global_pass++;
        colour += sample_pixel(x + 0.3, y + 0.3);
        global_pass++;
        colour += sample_pixel(x - 0.3, y + 0.3);
        global_pass++;
        colour += sample_pixel(x + 0.3, y - 0.3);
        global_pass++;
        colour += sample_pixel(x - 0.3, y - 0.3);
        global_pass++;
    }
    if (quality >= 2) {
        colour += sample_pixel(x + 0.2, y);
        global_pass++;
        colour += sample_pixel(x - 0.2, y);
        global_pass++;
        colour += sample_pixel(x, y + 0.2);
        global_pass++;
        colour += sample_pixel(x, y - 0.2);
        global_pass++;
    }
    if (quality >= 3) {
        colour += sample_pixel(x + 0.1, y + 0.1);
        global_pass++;
        colour += sample_pixel(x - 0.1, y + 0.1);
        global_pass++;
        colour += sample_pixel(x + 0.1, y - 0.1);
        global_pass++;
        colour += sample_pixel(x - 0.1, y - 0.1);
        global_pass++;
    }
        
    if (quality == 1) {
        colour.red /= 5.0;
        colour.green /= 5.0;
        colour.blue /= 5.0;
        colour.alpha =1.0f;
    }
    else if (quality == 2) {
        colour.red /= 9.0;
        colour.green /= 9.0;
        colour.blue /= 9.0;
        colour.alpha = 1.0f;
    }
    else if (quality == 3) {
        colour.red /= 13.0;
        colour.green /= 13.0;
        colour.blue /= 13.0;
        colour.alpha = 1.0f;
    }

    return colour;
}    

/**************************************************************************************************
 * Render a pixel (or batch of pixels if using SIMD)
 * an input pixel is given
 * ************************************************************************************************/
template <SimdFloat S>
ColourRGBA<S> Renderer<S>::render_pixel_with_input(S x, S y, ColourRGBA<S>) const {
    render_pixel(x, y);
}

















