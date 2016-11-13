#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

#include "Mesh.h"

#include "MathGeoLib\MathGeoLib.h"

struct aiMesh;

class ComponentMesh : public Component
{

public:

	bool wire = false;

	math::AABB initial_bounding_box;
	math::AABB bounding_box;

	Mesh mesh;

	ComponentMesh();
	~ComponentMesh();

	bool Update();
	void SetComponent(const aiMesh *mesh);

	void ShowEditorInfo();

	void ApplyTransformToAABB();

	bool Save(JSONParser &go);
	void Load(); // TESTING!!!
};

#endif __COMPONENTMESH_H__
