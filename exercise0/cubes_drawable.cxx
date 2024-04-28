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
	cubes_drawable() :
		recursion_depth(3), 
		root_color_r(0.9f), root_color_g(0.0f), root_color_b(0.1f),
		root_color(root_color_r, root_color_g, root_color_b), enable(true), drawing_mode(BUILTIN) {}


	std::string get_type_name(void) const {
		return "cubes_drawable";
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh) {
		unsigned* drawing_mode_uint = (unsigned*)&drawing_mode;

		return 
			rh.reflect_member("enable", enable) &&
			rh.reflect_member("recursion_depth", recursion_depth) &&
			rh.reflect_member("root_color_r", root_color_r) &&
			rh.reflect_member("root_color_g", root_color_g) &&
			rh.reflect_member("root_color_b", root_color_b) &&
			rh.reflect_member("drawing_mode", drawing_mode);
			
	}

	void on_set(void* member_ptr) {

		
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

		if (this->is_visible()) post_redraw();
	}




	void create_gui(void) {
		add_member_control(this, "show", enable);

		add_member_control(this,
			"recursion_depth", recursion_depth, "value_slider",
			"min=0;max=6" ";ticks=false"
		);
		
		add_member_control(this, "root_color", root_color);
		add_member_control(this, "Drawng Mode", drawing_mode, "dropdown", "enums='BUILTIN, INTERLEAVED, NONINTERLEAVED'");

	}

	bool init(cgv::render::context& ctx) {
		bool success = true;

		init_cube_geometry();
		cgv::render::type_descriptor vec3type = cgv::render::element_descriptor_traits<cgv::vec3>::get_type_descriptor(vertices_interleaved[0].pos);

		success = vb_interleaved.create(ctx, &(vertices_interleaved[0]), vertices_interleaved.size()) && success;
		success = vertex_array_interleaved.create(ctx) && success;
		success = vertex_array_interleaved.set_attribute_array(ctx, ctx.ref_surface_shader_program().get_position_index(), vec3type, vb_interleaved, 0, vertices_interleaved.size(), sizeof(vertex)) && success;
		success = vertex_array_interleaved.set_attribute_array(ctx, ctx.ref_surface_shader_program().get_normal_index(), vec3type, vb_interleaved, sizeof(cgv::vec3), vertices_interleaved.size(), sizeof(vertex)) && success;

		success = vb_positions.create(ctx, &(vertex_positions[0]), vertex_positions.size()) && success;
		success = vb_normals.create(ctx, &(vertex_normals[0]), vertex_normals.size()) && success;
		success = vertex_array_noninterleaved.create(ctx) && success;
		success = vertex_array_noninterleaved.set_attribute_array(ctx, ctx.ref_surface_shader_program().get_position_index(), vec3type, vb_positions, 0, vertex_positions.size(), sizeof(cgv::vec3)) && success;
		success = vertex_array_noninterleaved.set_attribute_array(ctx, ctx.ref_surface_shader_program().get_normal_index(), vec3type, vb_normals, 0, vertex_normals.size(), sizeof(cgv::vec3)) && success;


		return success;
	}



	void draw(cgv::render::context& ctx) {
		if (!enable) return;

		

		
		cgv::render::shader_program& default_shader = ctx.ref_surface_shader_program();
		default_shader.enable(ctx);


		cubes_fractal cubes;

		switch (drawing_mode) {
		case INTERLEAVED:
			cubes.use_vertex_array(&vertex_array_interleaved, vertices_interleaved.size(), GL_QUADS);
			cubes.draw_recursive(ctx, root_color, recursion_depth);
			break;
		case NONINTERLEAVED:
			cubes.use_vertex_array(&vertex_array_noninterleaved, vertex_positions.size(), GL_QUADS);
			cubes.draw_recursive(ctx, root_color, recursion_depth);
			break;
		case BUILTIN:
			cubes.draw_recursive(ctx, root_color, recursion_depth);
		}

		default_shader.disable(ctx);
	}

	void init_cube_geometry(void) {
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
};
	

cgv::base::object_registration<cubes_drawable> cubes_drawable_registration(
	"cubes_drawable"
);
		



// [END] Tasks 0.2a, 0.2b and 0.2c
// ************************************************************************************/


// ************************************************************************************/
// Task 0.2a: register an instance of your drawable.

// < your code here >
