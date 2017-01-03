#ifndef __COMPONENTTRANSFORM2D_H__
#define __COMPONENTTRANSFORM2D_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Mesh.h"

class ComponentTransform2D : public Component
{

private:
	
	math::float3 global_position;				// World position representation
	math::float3 local_position;				// Position representation;
	math::float2 size;							// Size representation;

	Mesh canvas;

	bool apply_transformation = false;

public:

	//math::float4x4 local_transform;				// Its local matrix transformation;
	//math::float4x4 world_transform;				// Its world matrix transformation;
	//math::float4x4 parent_transform;			// Combined parent matrix transformation;

	ComponentTransform2D();
	bool Update();
	void AddResource(const Resource *res) {}

	void ShowEditorInfo();

	void RecalcTranslations();
	void CalcGlobalTranslation();
	//math::float4x4 CalcTransformMatrix(const math::float3 &pos, const math::float3 &scale, const math::Quat &rot);

	// Utilities
	void Move(const math::vec &movement);
	
	// Setters / Getters
	const math::vec &GetGlobalPos() const;
	const math::vec &GetLocalPos() const;

	void SetLocalPos(const math::vec &local_pos);
	void SetSize(const math::float2 &size);

	// Save
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTTRANSFORM2D_H__
