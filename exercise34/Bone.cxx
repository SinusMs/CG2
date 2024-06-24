﻿// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved
//
#include "Bone.h"

#include <cgv/math/transformations.h>
#include <cgv/math/inv.h>

#include "math_helper.h"

Bone::Bone()
	: parent(nullptr), length(0.0f), direction_in_world_space(0.0, 0.0, 0.0), translationTransforms(0)
{}

Bone::~Bone()
{
	for (auto bone : children)
		delete bone;
	children.clear();

	dofs.clear();

	for (auto dof : orientation)
		delete dof;
	orientation.clear();
}

void Bone::calculate_matrices()
{
	orientationSystemTransformLocalToGlobal.identity();
	std::for_each(orientation.begin(), orientation.end(), [&](AtomicTransform* t) {
        orientationSystemTransformLocalToGlobal = orientationSystemTransformLocalToGlobal * t->calculate_matrix();
	});
    orientationModelTransformLocalToGlobal = cgv::math::inv(orientationSystemTransformLocalToGlobal);
    // ^ same thing:  orientationSystemTransformGlobalToLocal = cgv::math::inv(orientationModelTransformGlobalToLocal);

	////
	// Task 3.1: Implement matrix calculation
	translationTransformCurrentJointToNext = translate(
		cgv::math::fvec<float, 3>(1, 0, 0) * this->length);
		//this->direction_in_world_space*this->get_length());
	Vec3 zComp = Vec3(this->direction_in_world_space[0], direction_in_world_space[1], 0);
	zComp = zComp.normalize() * zComp;

	Vec3 yComp = Vec3(this->direction_in_world_space[0], 0, this->direction_in_world_space[2]);
	yComp = yComp.normalize() * yComp;

	orientationTransformPrevJointToCurrent = rotate(cgv::math::fvec<float, 3>(0, 1, 0), cgv::math::dot(yComp, Vec3(1, 0, 0)))
		* rotate(Vec3(0,0,1),dot(zComp, Vec3(1, 0, 0)));

/*
	cgv::math::fvec<float, 3> parent_vec = get_parent()->get_direction_in_world_space();

	cgv::math::fvec<float, 3> v = cgv::math::cross(parent_vec, get_direction_in_world_space());
	float s = cgv::math::sqr_length(v);
	float c = cgv::math::dot(parent_vec, get_direction_in_world_space());

	Mat4 vx;
	vx(0,1) -= v[2];
	vx(0,2) += v[1];
	vx(1,0) += v[2];
	vx(1,2) -= v[0];
	vx(2,0) -= v[1];
	vx(2,1) += v[0];

	Mat4 t;
	t.identity();
	t += vx + cgv::math::sqr(vx) * 1 / (1 + c);

*/
	////
	// Task 4.6: Implement matrix calculation (skinning)

}

Mat4 Bone::calculate_transform_prev_to_current_with_dofs()
{
	////
	// Task 3.1: Implement matrix calculation

	Mat4 t;
	return t;
}

Mat4 Bone::calculate_transform_prev_to_current_without_dofs()
{
	////
	// Task 3.1: Implement matrix calculation
	Mat4 t;

	return t;
}

void Bone::add_dof(AtomicTransform* dof)
{
	dof->set_index_in_amc((int)dofs.size());	
	if (dynamic_cast<AtomicTranslationTransform*>(dof))
	{
		dofs.push_front(std::shared_ptr<AtomicTransform>(dof));
		++translationTransforms;
	}
	else
		dofs.insert(dofs.begin() + translationTransforms, std::shared_ptr<AtomicTransform>(dof));
}

void Bone::set_name(const std::string& name) { this->name = name; }
const std::string& Bone::get_name() const { return name; }

void Bone::set_direction_in_world_space(const Vec3& direction) { this->direction_in_world_space = direction; }
const Vec3& Bone::get_direction_in_world_space() const { return direction_in_world_space; }

void Bone::set_length(float l) { this->length = l; }
float Bone::get_length() const { return length; }

void Bone::add_axis_rotation(AtomicRotationTransform* transform) { orientation.push_front(transform); }
void Bone::add_child(Bone* child)
{
	child->set_parent(this);
	children.push_back(child);
}
Bone* Bone::child_at(int i) const { return children[i]; }
int Bone::childCount() const { return (int)children.size(); }

void Bone::set_parent(Bone* parent)
{
	this->parent = parent;
}
Bone* Bone::get_parent() const { return parent; }

int Bone::dof_count() const { return (int)dofs.size(); }
std::shared_ptr<AtomicTransform> Bone::get_dof(int dofIndex) const { return dofs[dofIndex]; }

const Mat4& Bone::get_binding_pose_matrix() const
{
	return systemTransformGlobalToLocal;
}

const Mat4& Bone::get_translation_transform_current_joint_to_next() const { return translationModelTransformCurJointToNext; }
const Mat4& Bone::get_orientation_transform_prev_joint_to_current() const { return orientationModelTransformPrevJointToCur; }

Vec4 Bone::get_bone_local_root_position() const { return Vec4(0, 0, 0, 1); }
Vec4 Bone::get_bone_local_tip_position() const { return translationModelTransformCurJointToNext * Vec4(0, 0, 0, 1); }
