#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component
{
public:

	math::float3 position;
	math::float3 scale;
	math::float4 rotation;

	ComponentTransform()
	{
		type = COMPONENT_TYPE::TRANSFORM;
	}

	void SetComponent(aiVector3D translation, aiVector3D scaling, aiQuaternion rotating)
	{
		position = { translation.x, translation.y, translation.z };
		scale = { scaling.x, scaling.y, scaling.z };
		rotation = { rotating.x, rotating.y, rotating.z, 1.0f };
	}
	

};

#endif __COMPONENTTRANSFORM_H__
