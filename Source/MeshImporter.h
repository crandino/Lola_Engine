#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"

#include "Mesh.h"
#include "Assimp\include\mesh.h"

#include "MathGeoLib\MathGeoLib.h" 

typedef long unsigned int ID;

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

		// Loading vertices...
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
		}

		return bytes;
	}

	void Load(const aiMesh *ai_mesh, Mesh *mesh)
	{
		// Copying indicies (faces on Assimp)
		if (ai_mesh->HasFaces())
		{
			mesh->num_indices = ai_mesh->mNumFaces * 3;
			mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
			for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
		}

		// Copying vertices...
		mesh->num_vertices = ai_mesh->mNumVertices;
		mesh->vertices = new math::float3[mesh->num_vertices];
		memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(math::float3) * mesh->num_vertices);

		// Copying colors...
		mesh->num_colors = ai_mesh->mNumVertices;
		mesh->colors = new math::float4[mesh->num_vertices];
		memcpy(mesh->colors, ai_mesh->mColors, sizeof(math::float4) * mesh->num_vertices);

		// Copying normals...
		mesh->num_normals = ai_mesh->mNumVertices;
		mesh->normals = new math::float3[mesh->num_normals];
		memcpy(mesh->normals, ai_mesh->mNormals, sizeof(math::float3) * mesh->num_normals);

		// Copying texture coordinates...
		uint UV_index = 0;
		if (ai_mesh->HasTextureCoords(UV_index))
		{
			mesh->num_tex_coord = ai_mesh->mNumVertices;
			mesh->tex_coord = new math::float2[mesh->num_tex_coord];
			for (uint l = 0; l < mesh->num_tex_coord; ++l)
			{
				mesh->tex_coord[l].x = ai_mesh->mTextureCoords[UV_index][l].x;
				mesh->tex_coord[l].y = ai_mesh->mTextureCoords[UV_index][l].y;
			}
		}		
	}

	bool Import(const aiMesh *ai_mesh, std::vector<std::string> &imported_files, ID res_id)
	{
		std::string lib_folder = LIBRARY_MESH;
		// Loading Mesh from Assimp
		Mesh *mesh = new Mesh();
		Load(ai_mesh, mesh);

		// Saving Mesh to own format file
		char file[SHORT_STRING];
		sprintf_s(file, SHORT_STRING, "%lu%s", res_id, ".msh");
		std::string file_name = file;

		char *buf;
		uint size = Save(&buf, mesh);
		bool ret = App->file_system->Save((lib_folder + file_name).c_str(), buf, size) != 0 ? true : false;

		imported_files.push_back(file);

		RELEASE_ARRAY(buf);
		RELEASE(mesh);

		return ret;
	}
};

#endif // !__MESHIMPORTER_H__
