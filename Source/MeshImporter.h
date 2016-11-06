#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

#include "Mesh.h"

#include "MathGeoLib\MathGeoLib.h" 

typedef unsigned int uint;

class MeshImporter : public Importer
{

public:

	uint Save(char **data, const Mesh *mesh)
	{
		/* Save follows that sequence: indices, vertices, colors, normals and texture coordinates. Each component consists
		of num_of_elements and array of that elements.*/

		uint size = 0;

		unsigned int ranges[5] = { mesh->num_indices, mesh->num_vertices, (mesh->colors) ? mesh->num_vertices : 0,
		(mesh->normals) ? mesh->num_vertices : 0, (mesh->num_tex_coord) ? mesh->num_vertices : 0 };

		size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3;

		if (mesh->colors != nullptr) size += sizeof(float) * mesh->num_vertices * 4;
		if (mesh->normals != nullptr) size += sizeof(float) * mesh->num_vertices * 3;
		if (mesh->tex_coord != nullptr) size += sizeof(float) * mesh->num_vertices * 2;

		// Allocate!
		*data = new char[size]; 
		char *cursor = *data;

		// Storing ranges...
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);
		cursor += bytes; 

		// Storing indices...
		bytes = sizeof(uint) * mesh->num_indices;
		memcpy(cursor, mesh->indices, bytes);
		cursor += bytes;

		// Storing vertices...
		bytes = sizeof(float) * mesh->num_vertices * 3 ;
		memcpy(cursor, mesh->vertices, bytes);
		cursor += bytes;

		// Storing colors...
		if (mesh->colors != nullptr)
		{
			bytes = sizeof(float) * mesh->num_colors * 4;
			memcpy(cursor, mesh->colors, bytes);
			cursor += bytes;
		}

		// Storing normals...
		if (mesh->normals != nullptr)
		{
			bytes = sizeof(float) * mesh->num_normals * 3;
			memcpy(cursor, mesh->normals, bytes);
			cursor += bytes;
		}

		// Storing tex coordinates...
		if (mesh->tex_coord != nullptr)
		{
			bytes = sizeof(float) * mesh->num_tex_coord * 2;
			memcpy(cursor, mesh->tex_coord, bytes);
			cursor += bytes;
		}

		return size;
	}

	uint Load(char **data, Mesh *mesh)
	{
		/* Load follows that sequence: indices, vertices, colors, normals and texture coordinates. Each component consists
		of num_of_elements and array of that elements.*/

		uint bytes = 0;
		char *cursor = *data;

		// Ranges: Number of indices, vertices, colors, normals and texture coordinates.
		unsigned int ranges[5];
		bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		mesh->num_indices = ranges[0];
		mesh->num_vertices = ranges[1];
		mesh->num_colors = ranges[2];
		mesh->num_normals = ranges[3];
		mesh->num_tex_coord = ranges[4];
		cursor += bytes;

		// Loading indices...
		bytes = sizeof(uint) * mesh->num_indices;
		mesh->indices = new unsigned int[bytes];
		memcpy(mesh->indices, cursor, bytes);
		cursor += bytes;

		// Loading indices...
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new math::float3[bytes];
		memcpy(mesh->vertices, cursor, bytes);
		cursor += bytes;

		// Loading colors...
		if (mesh->num_colors > 0)
		{
			bytes = sizeof(float) * mesh->num_colors * 4;
			mesh->colors = new math::float4[bytes];
			memcpy(mesh->colors, cursor, bytes);
			cursor += bytes;
		}

		// Loading normals...
		if (mesh->num_normals > 0)
		{
			bytes = sizeof(float) * mesh->num_normals * 3;
			mesh->normals = new math::float3[bytes];
			memcpy(mesh->normals, cursor, bytes);
			cursor += bytes;
		}

		// Loading texture coordinates...
		if (mesh->num_tex_coord > 0)
		{
			bytes = sizeof(float) * mesh->num_tex_coord * 2;
			mesh->tex_coord = new math::float2[bytes];
			memcpy(mesh->tex_coord, cursor, bytes);
			cursor += bytes;
		}

		return bytes;
	}
};

#endif // !__MESHIMPORTER_H__
