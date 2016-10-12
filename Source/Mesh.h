#ifndef __MESH_H__
#define __MESH_H__

// NOT USED IN CODE! Remains here for Importer/Exporter moment...

#include "MathGeoLib\MathGeoLib.h"

struct Mesh
{
	// Vertices
	unsigned int id_vertices = 0; // id in VRAM
	unsigned int num_vertices = 0;
	math::float3 *vertices = nullptr;

	// Normals
	unsigned int id_normals = 0; // id in VRAM
	unsigned int num_normals = 0;
	math::float3 *normals = nullptr;

	// Colors
	unsigned int id_colors = 0; // id in VRAM
	unsigned int num_colors = 0;
	math::float4 *colors = nullptr;

	// Tex coordinates
	unsigned int id_tex_coord = 0; // id in VRAM
	unsigned int num_tex_coord = 0;
	math::float2 *tex_coord = nullptr;

	// Indices
	unsigned int id_indices = 0; // id in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;
};

#endif //!__MESH_H__

