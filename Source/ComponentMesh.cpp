#include "ComponentMesh.h"

#include "Globals.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "MeshImporter.h"

#include "Assimp\include\mesh.h"
#include "imgui\imgui.h"

ComponentMesh::ComponentMesh() : Component()
{
	wire = false;

	bounding_box.SetNegativeInfinity();
	initial_bounding_box.SetNegativeInfinity();

	type = COMPONENT_TYPE::MESH;
	name = GetNameByType(type);
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(mesh.vertices);
	RELEASE_ARRAY(mesh.normals);
	RELEASE_ARRAY(mesh.colors);
	RELEASE_ARRAY(mesh.tex_coord);
	RELEASE_ARRAY(mesh.indices);
}

bool ComponentMesh::Update()
{
	if (game_object->transform_applied)
		ApplyTransformToAABB();

	return true;
}

void ComponentMesh::SetComponent(const aiMesh *ai_mesh)
{
	DEBUG("Creating new mesh %s", ai_mesh->mName);

	// Copying vertices...
	mesh.num_vertices = ai_mesh->mNumVertices;
	mesh.vertices = new math::float3[mesh.num_vertices];
	memcpy(mesh.vertices, ai_mesh->mVertices, sizeof(math::float3) * mesh.num_vertices);
	DEBUG("  -> %d vertices", mesh.num_vertices);

	// Copying normals...
	mesh.num_normals = ai_mesh->mNumVertices;
	mesh.normals = new math::float3[mesh.num_normals];
	memcpy(mesh.normals, ai_mesh->mNormals, sizeof(math::float3) * mesh.num_normals);
	DEBUG("  -> %d normals", mesh.num_normals);

	// Copying texture coordinates...
	uint UV_index = 0;
	if (ai_mesh->HasTextureCoords(UV_index))
	{
		mesh.num_tex_coord = ai_mesh->mNumVertices;
		mesh.tex_coord = new math::float2[mesh.num_tex_coord];
		for (uint l = 0; l < mesh.num_tex_coord; ++l)
		{
			mesh.tex_coord[l].x = ai_mesh->mTextureCoords[UV_index][l].x;
			mesh.tex_coord[l].y = ai_mesh->mTextureCoords[UV_index][l].y;
		}
		DEBUG("  -> %d texture coordinates", mesh.num_tex_coord);
	}

	// Copying indicies (faces on Assimp)
	if (ai_mesh->HasFaces())
	{
		mesh.num_indices = ai_mesh->mNumFaces * 3;
		mesh.indices = new uint[mesh.num_indices]; // assume each face is a triangle
		DEBUG("  -> %d indices", mesh.num_indices);
		for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
		{
			if (ai_mesh->mFaces[j].mNumIndices != 3)
				DEBUG("WARNING, geometry face with != 3 indices!");
			else
			{
				memcpy(&mesh.indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
			}
		}
	}

	//// Calcultating AABB	
	initial_bounding_box.Enclose((math::float3*) mesh.vertices, mesh.num_vertices);
	ApplyTransformToAABB();
}

void ComponentMesh::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mesh", &active);
	ImGui::Checkbox("Wireframe##Mesh", &wire);
	ImGui::Text("Number of vertices: %d", mesh.num_vertices);
	ImGui::Text("Number of normals: %d", mesh.num_normals);
	ImGui::Text("Number of texture coordinates: %d", mesh.num_tex_coord);
	ImGui::Text("Number of indices: %d", mesh.num_indices);

	ImGui::Separator();
}

void ComponentMesh::ApplyTransformToAABB()
{
	math::OBB tmp = initial_bounding_box.Transform(game_object->transform->world_transform);
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(tmp);
}

bool ComponentMesh::Save(JSONParser &go)
{
	JSONParser component;

	component.AddInt("Type", type);
	component.AddBoolean("Wire", wire);

	MeshImporter mesh_importer;
	char save_filename[SHORT_STRING]; sprintf_s(save_filename, SHORT_STRING, "%lu%s", UUID, ".msh");
	char *buf;
	unsigned int size = mesh_importer.Save(&buf, &mesh);

	App->file_system->Save(save_filename, buf, size);
	RELEASE_ARRAY(buf);

	go.AddArray(component);

	return true;
}

void ComponentMesh::Load()
{
	MeshImporter mesh_importer;
	char save_filename[SHORT_STRING]; sprintf_s(save_filename, SHORT_STRING, "%s%lu%s", App->file_system->GetSaveDirectory(), UUID, ".msh");
	char *buf;
	App->file_system->Load(save_filename, &buf);
	Mesh test;
	unsigned int size = mesh_importer.Load(&buf, &test);
}