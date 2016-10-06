#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component
{
public:

	math::float3 position;
	math::float3 scale;
	math::Quat rotation;

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

		position = { translation.x, translation.y, translation.z };
		scale = { scaling.x, scaling.y, scaling.z };
		rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

		SetTransformMatrix();
	}

	void SetTransformMatrix()
	{
		math::float4x4 trans_mat, rot_mat, scale_mat;
		trans_mat = math::float4x4::Translate(position);
		math::vec axis(rotation.x, rotation.y, rotation.z);
		rot_mat = math::float4x4::RotateAxisAngle(axis, rotation.Angle());
		scale_mat = math::float4x4::Scale(scale).ToFloat4x4();
		
		transform = trans_mat * rot_mat * scale_mat * transform;
		transform = transform.Transposed();
	}
};

#endif __COMPONENTTRANSFORM_H__
