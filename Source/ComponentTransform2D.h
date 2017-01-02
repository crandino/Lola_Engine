#ifndef __COMPONENTTRANSFORM2D_H__
#define __COMPONENTTRANSFORM2D_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform2D : public Component
{

private:

	//math::vec forward_dir, left_dir, up_dir;

	math::float3 local_position;				// Position representation;
	math::float2 local_size;					// Scale representation;	

	//math::float3 local_rotation_euler_rad;		// Euler representation on rad.
	//math::float3 local_rotation_euler_deg;		// Euler representation on degrees.

public:

	//math::vec forward, left, up;

	math::Quat local_rotation_quat;			 	// Rotation representation (quaternion);
	math::float4x4 local_transform;				// Its local matrix transformation;
	math::float4x4 world_transform;				// Its world matrix transformation;
	math::float4x4 parent_transform;			// Combined parent matrix transformation;

	ComponentTransform2D();
	bool Update();
	void AddResource(const Resource *res) {}

	void ShowEditorInfo();

	void RecalcTransformations();
	void CalcWorldTransformMatrix();
	math::float4x4 CalcTransformMatrix(const math::float3 &pos, const math::float3 &scale, const math::Quat &rot);

	// Utilities
	void Move(const math::vec &movement);
	void SetPos(const math::vec &position);
	
	// Getters
	const math::vec &GetPos() const;

	// Save
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTRANSFORM2D_H__
