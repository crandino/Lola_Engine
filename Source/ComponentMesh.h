#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

#include "MathGeoLib\MathGeoLib.h"
#include "Assimp\include\mesh.h"

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

	void SetComponent(const aiMesh *mesh)
	{
		DEBUG("Creating new mesh %s", mesh->mName);
		// Copying vertices...
		num_vertices = mesh->mNumVertices;
		vertices = new math::float3[num_vertices];
		memcpy(vertices, mesh->mVertices, sizeof(math::float3) * num_vertices);
		DEBUG("  -> %d vertices", num_vertices);

		// Copying normals...
		num_normals = mesh->mNumVertices;
		normals = new math::float3[num_normals];
		memcpy(normals,mesh->mNormals, sizeof(math::float3) * num_normals);
		DEBUG("  -> %d normals", num_normals);

		// Copying texture coordinates...
		uint UV_index = 0;
		if (mesh->HasTextureCoords(UV_index))
		{
			num_tex_coord = mesh->mNumVertices;
			tex_coord = new math::float2[num_tex_coord];
			for (uint l = 0; l < num_tex_coord; ++l)
			{
				tex_coord[l].x = mesh->mTextureCoords[UV_index][l].x;
				tex_coord[l].y = mesh->mTextureCoords[UV_index][l].y;
			}
			DEBUG("  -> %d texture coordinates", num_tex_coord);
		}

		// Copying indicies (faces on Assimp)
		if (mesh->HasFaces())
		{
			num_indices = mesh->mNumFaces * 3;
			indices = new uint[num_indices]; // assume each face is a triangle
			DEBUG("  -> %d indices", num_indices);
			for (uint j = 0; j < mesh->mNumFaces; ++j)
			{
				if (mesh->mFaces[j].mNumIndices != 3)
					DEBUG("WARNING, geometry face with != 3 indices!");
				else
				{
					memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
		}
	}
};

#endif __COMPONENTMESH_H__
