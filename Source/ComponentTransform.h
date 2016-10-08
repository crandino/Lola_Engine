#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Assimp/include/scene.h"

class ComponentTransform : public Component
{
public:

	math::float3 local_position;
	math::float3 local_scale;
	math::Quat local_rotation;

	math::float4x4 transform;

	ComponentTransform()
	{
		type = COMPONENT_TYPE::TRANSFORM;
		transform.SetIdentity();
	}

	bool Update()
	{
		//SetTransformMatrix();
		return true;

	}

	void SetComponent(aiNode *go, aiNode *parent)
	{
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotating;

		go->mTransformation.Decompose(scaling, rotating, translation);

		local_position = { translation.x, translation.y, translation.z };
		local_scale = { scaling.x, scaling.y, scaling.z };
		local_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

		math::float4x4 local_matrix = CalcTransformMatrix(local_position, local_scale, local_rotation);

		math::float3 parent_position;
		math::float3 parent_scale;
		math::Quat parent_rotation;

		aiNode *node = go->mParent;
		math::float4x4 parent_matrix; parent_matrix.SetIdentity();

		while (node != nullptr)
		{
			node->mTransformation.Decompose(scaling, rotating, translation);
			parent_position = { translation.x, translation.y, translation.z };
			parent_scale = { scaling.x, scaling.y, scaling.z };
			parent_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

			math::float4x4 grand_father_matrix = CalcTransformMatrix(parent_position, parent_scale, parent_rotation);
			parent_matrix = grand_father_matrix * parent_matrix;
			node = node->mParent;
		}
		//parent->mTransformation.Decompose(scaling, rotating, translation);		

		transform = parent_matrix * local_matrix;
		transform = transform.Transposed();
	}

	math::float4x4 CalcTransformMatrix(math::float3 &pos, math::float3 &scale, math::Quat &rot)
	{
		math::float4x4 trans_mat, rot_mat, scale_mat;
		trans_mat = math::float4x4::Translate(pos);
		math::vec axis(rot.x, rot.y, rot.z);
		rot_mat = math::float4x4::RotateAxisAngle(axis, rot.Angle());
		scale_mat = math::float4x4::Scale(scale).ToFloat4x4();
		
		math::float4x4 mat = trans_mat * rot_mat *  scale_mat;
		
		return mat;
	}
};

#endif __COMPONENTTRANSFORM_H__
