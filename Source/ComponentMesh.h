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
	//// Vertices
	//unsigned int id_vertices = 0; // id in VRAM
	//unsigned int num_vertices = 0;
	//math::float3 *vertices = nullptr;

	//// Normals
	//unsigned int id_normals = 0; // id in VRAM
	//unsigned int num_normals = 0;
	//math::float3 *normals = nullptr;

	//// Tex coordinates
	//unsigned int id_tex_coord = 0; // id in VRAM
	//unsigned int num_tex_coord = 0;
	//math::float2 *tex_coord = nullptr;

	//// Indices
	//unsigned int id_indices = 0; // id in VRAM
	//unsigned int num_indices = 0;
	//unsigned int* indices = nullptr;

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
