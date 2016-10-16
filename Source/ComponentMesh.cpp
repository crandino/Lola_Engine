#include "ComponentMesh.h"

#include "Globals.h"

#include "Assimp\include\mesh.h"
#include "imgui\imgui.h"

ComponentMesh::ComponentMesh() : Component()
{
	wire = false;
	type = COMPONENT_TYPE::MESH;
	name = GetNameByType(type);
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(tex_coord);
	RELEASE_ARRAY(indices);
}

void ComponentMesh::SetComponent(const aiMesh *mesh)
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
	memcpy(normals, mesh->mNormals, sizeof(math::float3) * num_normals);
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

void ComponentMesh::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mesh", &active);
	ImGui::Checkbox("Wireframe##Mesh", &wire);
	ImGui::Text("Number of vertices: %d", num_vertices);
	ImGui::Text("Number of normals: %d", num_normals);
	ImGui::Text("Number of texture coordinates: %d", num_tex_coord);
	ImGui::Text("Number of indices: %d", num_indices);

	ImGui::Separator();
}
