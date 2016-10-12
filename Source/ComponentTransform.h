#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

struct aiNode;

class ComponentTransform : public Component
{

private:

	math::float3 local_position;
	math::float3 local_scale;
	math::Quat local_rotation_quat;

	math::float3 local_rotation_euler_rad;
	math::float3 local_rotation_euler_deg;

	math::float4x4 local_transform;
	math::float4x4 parent_transform;

public:
	
	math::float4x4 world_transform;	

	ComponentTransform();
	void SetComponent(aiNode *go);

	void ShowEditorInfo();	

	void CalcWorldTransformMatrix(math::float4x4 parent_mat);
	math::float4x4 CalcTransformMatrix(math::float3 &pos, math::float3 &scale, math::Quat &rot);

	void QuatToEuler(math::Quat &quat, math::float3 &out_euler);
	void EulerToQuat(math::float3 &euler, math::Quat &out_quat);

};

#endif __COMPONENTTRANSFORM_H__
