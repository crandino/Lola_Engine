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

};

#endif __COMPONENTTRANSFORM_H__
