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

<<<<<<< HEAD
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
=======
class cubes_drawable :
    public cgv::base::base,
    public cgv::gui::provider,
    public cgv::render::drawable
{
protected:
	bool enable;
	int recursion_deph;

    float color_r, color_g, color_b;
    cgv::rgb color;

	enum DrawingMode {BUILTIN, INTERLEAVED, NONINTERLEAVED} drawing_mode;

    struct vertex {
		cgv::vec3 pos;
		cgv::vec3 normal;
	};

    std::vector<vertex> vertices_interleaved;
    cgv::render::vertex_buffer vb_interleaved;
	cgv::render::attribute_array_binding vertex_array_interleaved;

	std::vector<cgv::vec3> vertex_positions;
	std::vector<cgv::vec3> vertex_normals;
	cgv::render::vertex_buffer vb_positions;
	cgv::render::vertex_buffer vb_normals;
	cgv::render::attribute_array_binding vertex_array_noninterleaved;
public:
    cubes_drawable():
    enable(false), recursion_deph(6), color_r(0.1f), color_g(0.0f), color_b(0.6f), color(color_r, color_g, color_b), drawing_mode(NONINTERLEAVED)
    {

    }

    std::string get_type_name(void) const
>>>>>>> branch123
	{
		return "cubes_drawable";
	}

<<<<<<< HEAD
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


=======
    bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		unsigned* drawing_mode_uint = (unsigned*)&drawing_mode;

		return
			rh.reflect_member("enable", enable) &&
			rh.reflect_member("recursion_deph", recursion_deph )&&
			rh.reflect_member("color_r", color_r) &&
			rh.reflect_member("color_g", color_g) &&
			rh.reflect_member("color_b", color_b) &&
			rh.reflect_member("drawing_mode", *drawing_mode_uint);
    }

    void on_set(void* member_ptr)
	{
        if (member_ptr == &color_r || member_ptr == &color_g ||
			member_ptr == &color_b)
		{
			color.R() = color_r;
			color.G() = color_g;
			color.B() = color_b;
			update_member(&color);
		}
		if (member_ptr == &color)
		{
			color_r = color.R();
			color_g = color.G();
			color_b = color.B();
		}

        update_member(member_ptr);

		// Also trigger a redraw in case the drawable node is active
		if (this->is_visible())
			post_redraw();
	}


    void create_gui(void)
	{
		add_member_control(this, "Enable", enable);
		add_member_control(this, "Recursion Deph", recursion_deph, "value_slider", "min=;max=8");

        add_member_control(this, "color", color);
		add_member_control(this, "Drawing Mode", drawing_mode, "dropdown", "enums='BUILTIN, INTERLEAVED, NONINTERLEAVED'");
    }

	bool init(cgv::render::context& ctx) {
        bool success = true;

		init_unit_cube_geometry();

		cgv::render::type_descriptor vec3type = cgv::render::element_descriptor_traits<cgv::vec3>::get_type_descriptor(vertices_interleaved[0].pos);

		success = vb_interleaved.create(
			ctx, &(vertices_interleaved[0]), vertices_interleaved.size()
		) && success;
		success = vertex_array_interleaved.create(ctx) && success;
		success = vertex_array_interleaved.set_attribute_array(
			ctx, ctx.ref_surface_shader_program().get_position_index(),
			vec3type, vb_interleaved, 0, vertices_interleaved.size(), sizeof(vertex)
		) && success;
		success = vertex_array_interleaved.set_attribute_array(
			ctx, ctx.ref_surface_shader_program().get_normal_index(),
			vec3type, vb_interleaved, sizeof(cgv::vec3), vertices_interleaved.size(), sizeof(vertex)
		) && success;


		success = vb_positions.create(
			ctx, &(vertex_positions[0]), vertex_positions.size()
		) && success;
        success = vb_normals.create(
			ctx, &(vertex_normals[0]), vertex_normals.size()
		) && success;
        success = vertex_array_noninterleaved.create(ctx) && success;
        success = vertex_array_noninterleaved.set_attribute_array(
			ctx, ctx.ref_surface_shader_program().get_position_index(),
			vec3type, vb_positions, 0, vertex_positions.size(), sizeof(cgv::vec3)
		) && success;
        success = vertex_array_noninterleaved.set_attribute_array(
			ctx, ctx.ref_surface_shader_program().get_normal_index(),
			vec3type, vb_normals, 0, vertex_normals.size(), sizeof(cgv::vec3)
		) && success;

		return success;
    }

	void draw(cgv::render::context& ctx)
	{
		if (!enable) return;

		cgv::render::shader_program& default_shader = ctx.ref_surface_shader_program();
		default_shader.enable(ctx);

		cubes_fractal cubes = cubes_fractal();

		switch (drawing_mode) {
			case INTERLEAVED:
				cubes.use_vertex_array(&vertex_array_interleaved, vertices_interleaved.size(), GL_QUADS);
				cubes.draw_recursive(ctx, color, recursion_deph);
				break;
			case NONINTERLEAVED:
				cubes.use_vertex_array(&vertex_array_noninterleaved, vertex_positions.size(), GL_QUADS);
				cubes.draw_recursive(ctx, color, recursion_deph);
				break;
			case BUILTIN:
			default:
				cubes.draw_recursive(ctx, color, recursion_deph);
				break;
		}
		default_shader.disable(ctx);
    }

	void init_unit_cube_geometry(void)
	{
		vertices_interleaved.resize(24);
		vertices_interleaved[0].pos.set(-1, -1, 1); vertices_interleaved[0].normal.set(0, 0, 1);
		vertices_interleaved[1].pos.set(1, -1, 1); vertices_interleaved[1].normal.set(0, 0, 1);
		vertices_interleaved[2].pos.set(1, 1, 1); vertices_interleaved[2].normal.set(0, 0, 1);
		vertices_interleaved[3].pos.set(-1, 1, 1); vertices_interleaved[3].normal.set(0, 0, 1);

		vertices_interleaved[4].pos.set(-1, 1, -1); vertices_interleaved[4].normal.set(0, 0, -1);
		vertices_interleaved[5].pos.set(1, 1, -1); vertices_interleaved[5].normal.set(0, 0, -1);
		vertices_interleaved[6].pos.set(1, -1, -1); vertices_interleaved[6].normal.set(0, 0, -1);
		vertices_interleaved[7].pos.set(-1, -1, -1); vertices_interleaved[7].normal.set(0, 0, -1);

		vertices_interleaved[8].pos.set(-1, 1, 1); vertices_interleaved[8].normal.set(0, 1, 0);
		vertices_interleaved[9].pos.set(1, 1, 1); vertices_interleaved[9].normal.set(0, 1, 0);
		vertices_interleaved[10].pos.set(1, 1, -1); vertices_interleaved[10].normal.set(0, 1, 0);
		vertices_interleaved[11].pos.set(-1, 1, -1); vertices_interleaved[11].normal.set(0, 1, 0);

		vertices_interleaved[12].pos.set(-1, -1, -1); vertices_interleaved[12].normal.set(0, -1, 0);
		vertices_interleaved[13].pos.set(1, -1, -1); vertices_interleaved[13].normal.set(0, -1, 0);
		vertices_interleaved[14].pos.set(1, -1, 1); vertices_interleaved[14].normal.set(0, -1, 0);
		vertices_interleaved[15].pos.set(-1, -1, 1); vertices_interleaved[15].normal.set(0, -1, 0);

		vertices_interleaved[16].pos.set(1, 1, -1); vertices_interleaved[16].normal.set(1, 0, 0);
		vertices_interleaved[17].pos.set(1, 1, 1); vertices_interleaved[17].normal.set(1, 0, 0);
		vertices_interleaved[18].pos.set(1, -1, 1); vertices_interleaved[18].normal.set(1, 0, 0);
		vertices_interleaved[19].pos.set(1, -1, -1); vertices_interleaved[19].normal.set(1, 0, 0);

		vertices_interleaved[20].pos.set(-1, -1, -1); vertices_interleaved[20].normal.set(-1, 0, 0);
		vertices_interleaved[21].pos.set(-1, -1, 1); vertices_interleaved[21].normal.set(-1, 0, 0);
		vertices_interleaved[22].pos.set(-1, 1, 1); vertices_interleaved[22].normal.set(-1, 0, 0);
		vertices_interleaved[23].pos.set(-1, 1, -1); vertices_interleaved[23].normal.set(-1, 0, 0);

		vertex_positions.clear();
		vertex_normals.clear();
		for (int i = 0; i < vertices_interleaved.size(); i++) {
			vertex_positions.push_back(vertices_interleaved[i].pos);
			vertex_normals.push_back(vertices_interleaved[i].normal);
		}
	}
>>>>>>> branch123
};

// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.
<<<<<<< HEAD
cgv::base::object_registration<cubes_drawable> cgv_demo_registration("");

cgv::base::factory_registration<cubes_drawable> cgv_demo_factory(
	"new/recursive cubes", // menu path
	'D',            // the shortcut - capital D means ctrl+d
	true            // whether the class is supposed to be a singleton
);
=======
//
// < your code here >
cgv::base::object_registration<cubes_drawable> cubes_drawable_registration("cubes drawable");
>>>>>>> branch123
