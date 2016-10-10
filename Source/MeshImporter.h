#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\postprocess.h"

#include "Mesh.h"

#include <stack>

typedef unsigned int uint;

class MeshImporter : public Importer
{
	bool Save(const char *saved_filename, const Mesh *mesh)
	{
		/* Save follows that sequence: indices, vertices, colors, normals and texture coordinates. Each component consists
		of num_of_elements and array of that elements. */

		unsigned int ranges[5] = { mesh->num_indices, mesh->num_vertices, (mesh->colors) ? mesh->num_vertices : 0,
		(mesh->normals) ? mesh->num_vertices : 0, (mesh->num_tex_coord) ? mesh->num_vertices : 0 };

		uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3;

		if (mesh->colors != nullptr) size += sizeof(float) * mesh->num_vertices * 4;
		if (mesh->normals != nullptr) size += sizeof(float) * mesh->num_vertices * 3;
		if (mesh->tex_coord != nullptr) size += sizeof(float) * mesh->num_vertices * 2;

		char* data = new char[size]; // Allocate

		char* cursor = data;

		uint bytes = sizeof(ranges); // First store ranges

		memcpy(cursor, ranges, bytes);

		cursor += bytes; // Store indices

		bytes = sizeof(uint) * mesh->num_indices;

		memcpy(cursor, mesh->indices, bytes);
	}
};

#endif // !__MESHIMPORTER_H__
