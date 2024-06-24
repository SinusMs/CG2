﻿#include <cgv/math/fvec.h>
#include "implicit_primitive.h"


template <typename T>
struct box : public implicit_primitive<T>
{
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	box() {}
	std::string get_type_name() const { return "box"; }
	void on_set(void* member_ptr) { implicit_base<T>::update_scene(); }

	/*********************************************************************************/
	/* Task 1.1a: If you need any auxiliary functions for this task, put them here.  */

	// < your code >

	/* [END] Task 1.1a
	/*********************************************************************************/

	/// Evaluate the implicit box function at p
	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 1.1a: Implement a function of p that evaluates to 0 on the unit cube.
		//            You may use any suitable distance metric.
		f_p =  std::max(std::max(std::abs(p(0)), std::abs(p(1))),std::abs(p(2))) -1;

		f_p =  std::max(std::max(std::abs(p(0)), std::abs(p(1))),std::abs(p(2))) -1;

		return f_p;
	}

	/// Evaluate the gradient of the implicit box function at p
	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 1.1a: Return the gradient of the function at p.
<<<<<<< HEAD
		grad_f_p = vec_type();

=======
		//TODO: proper gradient implementation
		if (p.x() == 1) return vec_type(1, 0, 0);
		else if (p.x() == -1) return vec_type(-1, 0, 0);
		else if (p.y() == 1) return vec_type(0, 1, 0);
		else if (p.y() == -1) return vec_type(0, -1, 0);
		else if (p.z() == 1) return vec_type(0, 0, 1);
		else if (p.z() == -1) return vec_type(0, 0, -1);
>>>>>>> a6055c7abf770844bc1237dc869895366b2b4460
		return grad_f_p;
	}

	void create_gui()
	{
		implicit_primitive<T>::create_gui();
	}
};

scene_factory_registration<box<double> > sfr_box("box;B");
