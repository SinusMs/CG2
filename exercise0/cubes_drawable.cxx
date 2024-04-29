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
	int recursion_depth;

	// Geometry buffers
	struct vertex {
		cgv::vec3 pos;
	};
	std::vector<vertex> vertices;
	cgv::render::vertex_buffer vb;
	cgv::render::attribute_array_binding vertex_array;


public:
	cubes_drawable() {

	}

	std::string get_type_name(void) const
	{
		return "cubes_drawable";
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		return rh.reflect_member("recursion_depth", recursion_depth);
	}

	void on_set(void* member_ptr) {

		
		update_member(member_ptr);

		// Also trigger a redraw in case the drawable node is active
		if (this->is_visible())
			post_redraw();
	}

	void create_gui(void) {
		cgv::gui::control<int>* ctrl = add_control(
			"recursion depth", recursion_depth, "value_slider",
			"min=0;max=8;ticks=false"
		).operator->();

	//	add_member_control(this, "recursion depth", recursion_depth);
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
