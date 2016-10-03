#ifndef __MESH_H__
#define __MESH_H__

struct Mesh
{
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
};

#endif __MESH_H__
