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
	}

	void SetComponent(aiVector3D translation, aiVector3D scaling, aiQuaternion rotating)
	{
		position = { translation.x, translation.y, translation.z };
		scale = { scaling.x, scaling.y, scaling.z };
		rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

		SetTransformMatrix();
	}

	void SetTransformMatrix()
	{
		transform = { position.x, scale.x, rotation.x, 0.0f, position.y, scale.y, rotation.y, 0.0f,
					  position.z, scale.z, rotation.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		/*transform = { position.x, position.y, position.z, 0.0f, scale.x, scale.y, scale.z, 0.0f,
			rotation.x, rotation.y, rotation.z, rotation.w, 0.0f, 0.0f, 0.0f, 1.0f };*/
	}

};

#endif __COMPONENTTRANSFORM_H__
