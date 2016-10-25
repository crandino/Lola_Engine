#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

struct aiNode;

class ComponentTransform : public Component
{

private:

	math::float3 local_position;				// Position representation;
	math::float3 local_scale;					// Scale representation;
	

	math::float3 local_rotation_euler_rad;		// Euler representation on rad.
	math::float3 local_rotation_euler_deg;		// Euler representation on degrees.

public:
	
	math::Quat local_rotation_quat;			 	// Rotation representation (quaternion);
	math::float4x4 local_transform;				// Its local matrix transformation;
	math::float4x4 world_transform;				// Its world matrix transformation;
	math::float4x4 parent_transform;			// Combined parent matrix transformation;

	ComponentTransform();
	void SetComponent(aiNode *go);

	void ShowEditorInfo();	

	void RecalcTransformations();
	void CalcWorldTransformMatrix(const math::float4x4 &parent_mat);
	math::float4x4 CalcTransformMatrix(const math::float3 &pos,const math::float3 &scale, const math::Quat &rot);

	void QuatToEuler(math::Quat &quat, math::float3 &out_euler);
	void EulerToQuat(math::float3 &euler, math::Quat &out_quat);
	

};

#endif __COMPONENTTRANSFORM_H__
