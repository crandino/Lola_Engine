#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

#include "Mesh.h"

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
};

#endif // !__MESHIMPORTER_H__
