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

class cubes_drawable
	: public cgv::base::base,      // This class supports reflection
	public cgv::gui::provider,   // Instances of this class provde a GUI
	public cgv::render::drawable // Instances of this class can be rendered
{
protected:
	int recursion_depth = 1;
	cgv::rgba rgba;
	float color_r, color_g, color_b;

	// Geometry buffers
	struct vertex {
		cgv::vec3 pos;
	};
	std::vector<vertex> vertices;
	cgv::render::vertex_buffer vb;
	cgv::render::attribute_array_binding vertex_array;


public:
	cubes_drawable() {
		rgba = cgv::rgb(color_r, color_g, color_b, 1.0);
		init_unit_square_geometry();
	}

	std::string get_type_name(void) const
	{
		return "cubes_drawable";
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		return rh.reflect_member("recursion_depth", recursion_depth) &&
		rh.reflect_member("fb_bgcolor_r", color_r) &&
		rh.reflect_member("fb_bgcolor_g", color_g) &&
		rh.reflect_member("fb_bgcolor_b", color_b);
	}

	void on_set(void* member_ptr) {
		if (member_ptr == &color_r || member_ptr == &color_g ||
			member_ptr == &color_b)
		{
			rgba.R() = color_r;
			rgba.G() = color_g;
			rgba.B() = color_b;
			update_member(&rgba);
		}
		if (member_ptr == &rgba)
		{
			color_r = rgba.R();
			color_g = rgba.G();
			color_b = rgba.B();
		}


		
		update_member(member_ptr);

		// Also trigger a redraw in case the drawable node is active
		post_redraw();

	}

	
	bool gui_check_value(cgv::gui::control<int>& ctrl) {
		post_redraw();

		return true;
	}

	void gui_value_changed(cgv::gui::control<int>& ctrl) {
		post_redraw();
	}

	void create_gui(void) {
		cgv::gui::control<int>* ctrl = add_control(
			"recursion depth", recursion_depth, "value_slider",
			"min=0;max=8;ticks=false"
		).operator->();
		cgv::signal::connect(ctrl->check_value, this, &cubes_drawable::gui_check_value);
		cgv::signal::connect(ctrl->value_change, this, &cubes_drawable::gui_value_changed);

		add_member_control(this, "color picker", rgba);

	//	add_member_control(this, "recursion depth", recursion_depth);
	}
	void draw(cgv::render::context& ctx)
	{
		glViewport(0, 0, 1080, 1080);

		// Usually, the framework takes care of clearing the (main) framebuffer, so in
		// order to not disturb that mechanism, we save the OpenGL state pertaining to
		// the color part of the framebuffer when we set a clear-color ourselves for
		// offscreen use.
		// Also, we're adjusting the framework-managed viewport to be in line with the
		// offscreen framebuffer dimensions, so save the current viewport as well.
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_VIEWPORT_BIT | GL_POLYGON_BIT);
		glClearColor(0.4, 0.7, 0.25, 0.9);
		glClear(GL_COLOR_BUFFER_BIT);
		glPushAttrib(GL_POLYGON_BIT);
		ctx.set_color(cgv::rgb(1.0f));
		
		
		
		//
		cgv::render::shader_program& default_shader =
		ctx.ref_surface_shader_program(true /* true for texture support */);

		default_shader.enable(ctx);

//		vertex_array.bind_attribute_array(ctx, default_shader, "vertices", &vertices, 4, 12);

		cubes_fractal cubes;

		cgv::render::type_descriptor
			vec3type =
			cgv::render::element_descriptor_traits<cgv::vec3>
			::get_type_descriptor(vertices[0].pos);


		vertex_array.create(ctx);
		vb.create(ctx, &(vertices[0]), vertices.size());
		vertex_array.set_attribute_array(
			ctx, default_shader.get_position_index(), vec3type, vb,
			0, // position is at start of the struct <-> offset = 0
			vertices.size(), // number of position elements in the array
			sizeof(vertex) // stride from one element to next
		);

		cubes.use_vertex_array(&vertex_array, 8, GL_TRIANGLE_STRIP);
	//	draw_my_unit_square(ctx);
		cubes.draw_recursive(ctx, rgba, recursion_depth, 0);

		//*****************************************************************/
		glPopAttrib();
		ctx.pop_modelview_matrix();

		default_shader.disable(ctx);
	}

	void init_unit_square_geometry()
	{
		// Prepare array
		vertices.resize(4);
		// lower-left
		vertices[0].pos.set(-1, -1, 0);
		// lower-right
		vertices[1].pos.set(1, -1, 0);
		// top-left
		vertices[2].pos.set(-1, 1, 0);
		// top-right
		vertices[3].pos.set(1, 1, 0);
	}

	// Draw method for a custom quad
	void draw_my_unit_square(cgv::render::context& ctx)
	{
		vertex_array.enable(ctx);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
		vertex_array.disable(ctx);
	}


};

// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.
cgv::base::object_registration<cubes_drawable> cgv_demo_registration("");

cgv::base::factory_registration<cubes_drawable> cgv_demo_factory(
	"new/recursive cubes", // menu path
	'D',            // the shortcut - capital D means ctrl+d
	true            // whether the class is supposed to be a singleton
);
