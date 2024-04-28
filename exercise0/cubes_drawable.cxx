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

// < your code here >
class cubes_drawable :
	public cgv::base::base,      // This class supports reflection
	public cgv::gui::provider,   // Instances of this class provde a GUI
	public cgv::render::drawable // Instances of this class can be rendered
{
protected:
	int recursion_depth;
	float root_color_r, root_color_g, root_color_b;
	cgv::rgba root_color;
	bool enable;

public:
	cubes_drawable() :
		recursion_depth(3), 
		root_color_r(0.9f), root_color_g(0.0f), root_color_b(0.1f),
		root_color(root_color_r, root_color_g, root_color_b), enable(true) {}


	std::string get_type_name(void) const {
		return "cubes_drawable";
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh) {
		return 
			rh.reflect_member("enable", enable) &&
			rh.reflect_member("recursion_depth", recursion_depth) &&
			rh.reflect_member("root_color_r", root_color_r) &&
			rh.reflect_member("root_color_g", root_color_g) &&
			rh.reflect_member("root_color_b", root_color_b);
	}

	void on_set(void* member_ptr) {
		if (member_ptr == &recursion_depth) {
			update_member(&recursion_depth);
		}
		
		if (member_ptr == &root_color_r || member_ptr == &root_color_g || member_ptr == &root_color_b) {
			root_color.R() = root_color_r;
			root_color.G() = root_color_g;
			root_color.B() = root_color_b;
			update_member(&root_color);
		}
		
		if (member_ptr = &root_color) {
			root_color_r = root_color.R();
			root_color_g = root_color.G();
			root_color_b = root_color.B();
		}

		update_member(member_ptr);
	}


	bool gui_check_value(cgv::gui::control<int>& ctrl) {
		return true;
	}

	void gui_value_changed(cgv::gui::control<int>& ctrl) {
		post_redraw();
	}

	void create_gui(void) {
		add_member_control(this, "show", enable);

		cgv::gui::control<int>* ctrl = add_control(
			"recursion_depth", recursion_depth, "value_slider",
			"min=0;max=5" ";ticks=false"
		).operator->();
		
		add_member_control(this, "root_color", root_color);

		cgv::signal::connect(ctrl->check_value, this, &cubes_drawable::gui_check_value);
		cgv::signal::connect(ctrl->value_change, this, &cubes_drawable::gui_value_changed);
	}

	bool init(cgv::render::context& ctx) {
		bool success = true;
		return success;
	}

	void init_frame(cgv::render::context& ctx) {
		// Check if we need to recreate anything

	}

	void draw(cgv::render::context& ctx) {
		if (!enable) return;

		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_VIEWPORT_BIT | GL_POLYGON_BIT);
		GL_POLYGON_BIT;
		// glClearColor(root_color.R(), root_color.G(), root_color.B(), root_color.alpha());
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, 1000, 750);

		glPushAttrib(GL_POLYGON_BIT);
		cgv::render::shader_program& default_shader =
		ctx.ref_surface_shader_program(true);
		default_shader.enable(ctx);

		ctx.set_color(cgv::rgb(1.0f));
		ctx.push_modelview_matrix();
		ctx.mul_modelview_matrix(cgv::math::scale4(
			1.0, 1.0, 1.0
		));

		cubes_fractal cubes;


		cgv::media::color<float> color = 1.0;

		
		cubes.draw_recursive(ctx, root_color, recursion_depth, 0);

		
		glPopAttrib();
		ctx.pop_modelview_matrix();

		// Disable shader program and texture
		default_shader.disable(ctx);
	}
};
	

cgv::base::object_registration<cubes_drawable> cubes_drawable_registration(
	"cubes_drawable"
);
		



// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.

// < your code here >
