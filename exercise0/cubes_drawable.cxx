// This source code is property of the Computer Graphics and Visualization chair of the
// TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved
//
// The main file of the plugin. It defines a class that demonstrates how to register with
// the scene graph, drawing primitives, creating a GUI, using a config file and various
// other parts of the framework.

// Framework core
#include <cgv/base/register.h>
#include <cgv/gui/provider.h>
#include <cgv/gui/trigger.h>
#include <cgv/render/drawable.h>
#include <cgv/render/shader_program.h>
#include <cgv/render/vertex_buffer.h>
#include <cgv/render/attribute_array_binding.h>
#include <cgv/math/ftransform.h>

// Framework standard plugins
#include <cgv_gl/gl/gl.h>

// Local includes
#include "cubes_fractal.h"


// ************************************************************************************/
// Task 0.2a: Create a drawable that provides a (for now, empty) GUI and supports
//            reflection, so that its properties can be set via config file.
//
// Task 0.2b: Utilize the cubes_fractal class to render a fractal of hierarchically
//            transformed cubes. Expose its recursion depth and color properties to GUI
//            manipulation and reflection. Set reasonable values via the config
//            file.
//
// Task 0.2c: Implement an option (configurable via GUI and config file) to use a vertex
//            array object for rendering the cubes. The vertex array functionality 
//            should support (again, configurable via GUI and config file) both
//            interleaved (as in cgv_demo.cpp) and non-interleaved attributes.

class cubes_drawable : public cgv::base::base, public cgv::gui::provider, public cgv::render::drawable
{
protected:
	bool enable;
	float r, g, b;
	cgv::rgb color;
	unsigned max_depth;

public:
	cubes_drawable() : enable(false), r(1.0), g(1.0), b(1.0), color(r, g, b), max_depth(3) { }

	std::string get_type_name(void) const
	{
		return "Cube Fractal";
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		return rh.reflect_member("enable", enable) &&
			rh.reflect_member("r", r) &&
			rh.reflect_member("g", g) &&
			rh.reflect_member("b", b) &&
			rh.reflect_member("max_depth", max_depth);
	}

	void on_set(void* member_ptr)
	{
		// Reflect the change to r/g/b in color (can only happen via reflection)
		if (member_ptr == &r || member_ptr == &g ||	member_ptr == &b)
		{
			color.R() = r;
			color.G() = g;
			color.B() = b;
			update_member(&color);
		}
		// ...and vice versa (can only happen via GUI interaction)
		if (member_ptr == &color)
		{
			r = color.R();
			g = color.G();
			b = color.B();
		}
		update_member(member_ptr);
		if (this->is_visible())
			post_redraw();
	}

	void create_gui(void)
	{
		add_member_control(this, "Enable", enable);
		add_member_control(this, "Color", color);
		add_member_control(this, "Max Depth", max_depth);
	}

	void draw(cgv::render::context& ctx)
	{
		if (!enable) return;

		cgv::render::shader_program& default_shader = ctx.ref_surface_shader_program();
		default_shader.enable(ctx);

		cubes_fractal fractal = cubes_fractal();
		
		fractal.draw_recursive(ctx, color, max_depth);
	}
};

// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.

cgv::base::object_registration<cubes_drawable> cubes_drawable_registration("cubes_drawable");
