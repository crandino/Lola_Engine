#ifndef __MESH_H__
#define __MESH_H__

struct Mesh
{
	uint id_indices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // id in VRAM
	uint num_vertices = 0;
	math::float3 *vertices = nullptr;
};

#endif __MESH_H__
