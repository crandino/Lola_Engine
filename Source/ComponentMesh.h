#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

#include "MathGeoLib\MathGeoLib.h"

#include "Globals.h"

class ComponentMesh : public Component
{

public:

	// Vertices
	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	math::float3 *vertices = nullptr;

	// Normals
	uint id_normals = 0; // id in VRAM
	uint num_normals = 0;
	math::float3 *normals = nullptr;

	// Tex coordinates
	uint id_tex_coord = 0; // id in VRAM
	uint num_tex_coord = 0;
	math::float2 *tex_coord = nullptr;

	// Indices
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	ComponentMesh()
	{ 
		type = COMPONENT_TYPE::MESH;
	}

	bool Update()
	{
		App->renderer3D->DrawMesh(this);
		return true;
	}
};

#endif __COMPONENTMESH_H__
