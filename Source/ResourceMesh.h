#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"
#include "Mesh.h"

#include "opengl.h"

class ResourceMesh : public Resource
{

public:

	Mesh *mesh_data;

	ResourceMesh(ID id, int timestamp) : Resource()
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::MESHES;

		mesh_data = nullptr;
	}

	~ResourceMesh()
	{
		RELEASE(mesh_data);  // Freeing mesh data
	}

	bool LoadToMemory()
	{
		loaded_in_memory = true;

		// Vertices
		glGenBuffers(1, (GLuint*) &(mesh_data->id_vertices));
		if (mesh_data->id_vertices == 0)
		{
			DEBUG("[error] Vertices buffer has not been binded!");
			loaded_in_memory = false;
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_vertices * 3, mesh_data->vertices, GL_STATIC_DRAW);
		}

		// Normals
		glGenBuffers(1, (GLuint*) &(mesh_data->id_normals));
		if (mesh_data->id_normals == 0)
		{
			DEBUG("[error] Normals buffer has not been binded!");
			loaded_in_memory = false;
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_normals * 3, mesh_data->normals, GL_STATIC_DRAW);
		}

		// Texture coordinates
		glGenBuffers(1, (GLuint*) &(mesh_data->id_tex_coord));
		if (mesh_data->id_tex_coord == 0)
		{
			DEBUG("[error] Texture coordinates buffer has not been binded!");
			loaded_in_memory = false;
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh_data->id_tex_coord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_data->num_tex_coord * 2, mesh_data->tex_coord, GL_STATIC_DRAW);
		}

		// Indices
		glGenBuffers(1, (GLuint*) &(mesh_data->id_indices));
		if (mesh_data->id_indices == 0)
		{
			DEBUG("[error] Indices buffer has not been binded!");
			loaded_in_memory = false;
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_data->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh_data->num_indices, mesh_data->indices, GL_STATIC_DRAW);
		}

		return loaded_in_memory;
	}

	bool ResourceMesh::UnloadFromMemory()
	{
		if (loaded_in_memory)
		{
			glDeleteBuffers(1, (GLuint*)&mesh_data->id_indices);
			glDeleteBuffers(1, (GLuint*)&mesh_data->id_vertices);
			glDeleteBuffers(1, (GLuint*)&mesh_data->id_colors);
			glDeleteBuffers(1, (GLuint*)&mesh_data->id_tex_coord);
			glDeleteBuffers(1, (GLuint*)&mesh_data->id_normals);

			RELEASE(mesh_data);
			loaded_in_memory = false;
		}

		return !loaded_in_memory;
	}
};


#endif //!__RESOURCE_MESH_H__

