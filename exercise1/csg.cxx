﻿#include <limits>
#include <cgv/math/fvec.h>
#include "implicit_group.h"

// ======================================================================================
//  Task 1.1b: GENERAL HINTS
//
//  The common super class of all CSG nodes is implicit_group. This class defines a
//  method ::get_implicit_child(unsigned int), which retrieves a pointer - already
//  casted to implicit_base<T>* - to the indicated child node, on which you can then call
//  ::evaluate() and ::evaluate_gradient().
//  Use the method ::get_nr_children() of the super super class cgv::base::group to query
//  the number of children registered with your operator.
//
// ======================================================================================

template <typename T>
class union_node : public implicit_group<T>
{
public:
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	union_node() { implicit_base<T>::gui_color = 0xffff00; }
	std::string get_type_name() const { return "union_node"; }

	T eval_and_get_index(const pnt_type& p, unsigned int& selected_i) const
	{
		T value;

		// Task 1.1b: You can outsource logic here that evaluates the operator function
		//            and reports the index of the relevant child in selected_i

		value = std::numeric_limits<double>::infinity();
		for (unsigned i = 0; i < cgv::base::group::get_nr_children(); i++) {
			const implicit_base<T>* child = get_implicit_child(i);
			T current_value = (*child).evaluate(p);
			if (current_value < value) {
				value = current_value;
				selected_i = i;
			}
		}

		return value;
	}

	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 1.1b: Evaluate the union operator at p.
		unsigned i;
		return eval_and_get_index(p, i);
	}

	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 1.1b: Return the gradient of the union operator at p
		unsigned i;
		eval_and_get_index(p, i);
		grad_f_p = (*get_implicit_child(i)).evaluate_gradient(p);

		return grad_f_p;
	}
};

template <typename T>
class intersection_node : public implicit_group<T>
{
public:
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	intersection_node() { implicit_base<T>::gui_color = 0xffff00; }
	std::string get_type_name() const { return "intersection_node"; }

	T eval_and_get_index(const pnt_type& p, unsigned int& selected_i) const
	{
		T value;

		// Task 1.1b: You can outsource logic here that evaluates the operator function
		//            and reports the index of the relevant child in selected_i
		value = -std::numeric_limits<double>::infinity();
		for (unsigned i = 0; i < cgv::base::group::get_nr_children(); i++) {
			const implicit_base<T>* child = get_implicit_child(i);
			T current_value = (*child).evaluate(p);
			if (current_value > value) {
				value = current_value;
				selected_i = i;
			}
		}

		return value;
	}

	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 1.1b: Evaluate the intersection operator at p.
		unsigned i;
		return eval_and_get_index(p, i);
	}

	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 1.1b: Return the gradient of the intersection operator at p
		unsigned i;
		eval_and_get_index(p, i);
		grad_f_p = (*get_implicit_child(i)).evaluate_gradient(p);

		return grad_f_p;
	}
};

template <typename T>
class difference_node : public implicit_group<T>
{
public:
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	difference_node() { implicit_base<T>::gui_color = 0xffff00; }
	std::string get_type_name() const { return "difference_node"; }

	T eval_and_get_index(const pnt_type& p, unsigned int& selected_i) const
	{
		T value;

		// Task 1.1b: You can outsource logic here that evaluates the operator function
		//            and reports the index of the relevant child in selected_i
		value = -std::numeric_limits<double>::infinity();
		T first_value = (*get_implicit_child(0)).evaluate(p);
		for (unsigned i = 1; i < cgv::base::group::get_nr_children(); i++) {
			const implicit_base<T>* child = get_implicit_child(i);
			T current_value = (*child).evaluate(p);
			if (std::max(first_value, -current_value) > value) {
				value = std::max(first_value, -current_value);
				selected_i = first_value > -current_value ? 0 : i;
			}
		}

		return value;
	}

	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 1.1b: Evaluate the difference operator at p.
		unsigned i;
		return eval_and_get_index(p, i);
	}

	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 1.1b: Return the gradient of the difference operator at p
		unsigned i;
		eval_and_get_index(p, i);
		grad_f_p = (*get_implicit_child(i)).evaluate_gradient(p);

		return grad_f_p;
	}
};

scene_factory_registration<union_node<double> > sfr_union("union;+");
scene_factory_registration<intersection_node<double> > sfr_intersect("intersection;*");
scene_factory_registration<difference_node<double> > sfr_difference("difference;-");
