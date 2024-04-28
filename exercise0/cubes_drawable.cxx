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

class cubes_drawable:
    public cgv::base::base,
    public cgv::gui::provider,
    public cgv::render::drawable
{
protected:
	int recursion_deph;

    cgv::rgba bgcolor;

    float color_r, color_g, color_b;
    cgv::rgb color;

    struct vertex {
		cgv::vec3 pos;
		cgv::vec2 tcoord;
	};

    std::vector<vertex> vertices;
    cgv::render::vertex_buffer vb;
	cgv::render::attribute_array_binding vertex_array;
    bool fb_invalid;
public:
    cubes_drawable():
    recursion_deph(6), bgcolor(255, 0, 255, 1), color_r(0.1f), color_g(0.0f), color_b(0.6f), color(color_r, color_g, color_b), fb_invalid(true)
    {

    }

    std::string get_type_name(void) const
	{
		return "cubes_drawable";
	}

    bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		return
			rh.reflect_member("recursion_deph", recursion_deph)&&
			rh.reflect_member("color_r", color_r) &&
			rh.reflect_member("color_g", color_g) &&
			rh.reflect_member("color_b", color_b);
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

    bool gui_check_value(cgv::gui::control<int>& ctrl)
	{
        return true;
    }

    void gui_value_changed(cgv::gui::control<int>& ctrl)
	{
        post_redraw();
	}

    void create_gui(void)
	{
        cgv::gui::control<int>* ctrl = add_control(
			"recursion_deph", recursion_deph, "value_slider",
			"min=0;max=8;ticks=false"
		).operator->();
        cgv::signal::connect(ctrl->check_value, this, &cubes_drawable::gui_check_value);
		cgv::signal::connect(ctrl->value_change, this, &cubes_drawable::gui_value_changed);

        add_member_control(this, "color", color);
    }

	bool init(cgv::render::context& ctx) {
        bool success = true;
        cgv::render::shader_program& default_shader
			= ctx.ref_default_shader_program(true /* true for texture support */);
		// - generate actual geometry
		init_unit_square_geometry();
		// - obtain type descriptors for the automatic array binding facilities of the
		//   framework
		cgv::render::type_descriptor
			vec2type =
			cgv::render::element_descriptor_traits<cgv::vec2>
			::get_type_descriptor(vertices[0].tcoord),
			vec3type =
			cgv::render::element_descriptor_traits<cgv::vec3>
			::get_type_descriptor(vertices[0].pos);
		// - create buffer objects
		success = vb.create(ctx, &(vertices[0]), vertices.size()) && success;
		success = vertex_array.create(ctx) && success;
		success = vertex_array.set_attribute_array(
			ctx, default_shader.get_position_index(), vec3type, vb,
			0, // position is at start of the struct <-> offset = 0
			vertices.size(), // number of position elements in the array
			sizeof(vertex) // stride from one element to next
		) && success;
		success = vertex_array.set_attribute_array(
			ctx, default_shader.get_texcoord_index(), vec2type, vb,
			sizeof(cgv::vec3), // tex coords follow after position
			vertices.size(), // number of texcoord elements in the array
			sizeof(vertex) // stride from one element to next
		) && success;

		// Flag offscreen framebuffer as taken care of
		fb_invalid = false;

		// All initialization has been attempted
		return success;
    }

    void init_frame(cgv::render::context& ctx)
	{
		if (fb_invalid)
		{
			init(ctx);
		}
	}

	void draw(cgv::render::context& ctx)
	{
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_VIEWPORT_BIT | GL_POLYGON_BIT);
        glClearColor(bgcolor.R(), bgcolor.G(), bgcolor.B(), bgcolor.alpha());
		glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, 1000, 800);

        glPushAttrib(GL_POLYGON_BIT);
        cgv::render::shader_program& default_shader = ctx.ref_surface_shader_program(true /* true for texture support */);
        default_shader.enable(ctx);

        ctx.set_color(cgv::rgb(1.0f));

        ctx.push_modelview_matrix(); // Account for aspect ratio of the offscreen texture
		ctx.mul_modelview_matrix(cgv::math::scale4( 1.0, 1.0, 1.0));

        cubes_fractal cubes;

        cubes.draw_recursive(ctx, color, recursion_deph, 0);

        glPopAttrib();
		ctx.pop_modelview_matrix();

		// Disable shader program and texture
		default_shader.disable(ctx);
    }

	void init_unit_square_geometry(void)
	{
		// Prepare array
		vertices.resize(4);
		// lower-left
		vertices[0].pos.set(-1, -1, 0); vertices[0].tcoord.set(0, 0);
		// lower-right
		vertices[1].pos.set(1, -1, 0); vertices[1].tcoord.set(1, 0);
		// top-left
		vertices[2].pos.set(-1, 1, 0); vertices[2].tcoord.set(0, 1);
		// top-right
		vertices[3].pos.set(1, 1, 0); vertices[3].tcoord.set(1, 1);
	}
};

// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.
//
// < your code here >
cgv::base::object_registration<cubes_drawable> cubes_drawable_registration("cubes drawable");

// cgv::base::factory_registration<cubes_drawable> cubes_drawable_factory(
//     "new/cubes_drawable", // menu path
//     'D',            // the shortcut - capital D means ctrl+d
//     true            // whether the class is supposed to be a singleton
// );
