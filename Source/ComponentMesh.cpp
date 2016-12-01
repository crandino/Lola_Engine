#include "ComponentMesh.h"

#include "Globals.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "Assimp\include\mesh.h"
#include "imgui\imgui.h"

ComponentMesh::ComponentMesh() : Component()
{
	wire = false;

	bounding_box.SetNegativeInfinity();
	initial_bounding_box.SetNegativeInfinity();

	type = COMPONENT_TYPE::MESH;
	name = GetNameByType(type);

	resource = nullptr;
}

ComponentMesh::~ComponentMesh()
{ }

bool ComponentMesh::Update()
{
	if (game_object->transform_applied)
		ApplyTransformToAABB();

	return true;
}

void ComponentMesh::AddResource(const Resource *res)
{
	resource = (ResourceMesh*)res;

	//// Calcultating AABB	
	initial_bounding_box.Enclose((math::float3*) resource->mesh_data->vertices, resource->mesh_data->num_vertices);
	ApplyTransformToAABB();
}

void ComponentMesh::SetComponent(const aiMesh *ai_mesh)
{
	//DEBUG("Creating new mesh...");

	//// Copying vertices...
	//mesh.num_vertices = ai_mesh->mNumVertices;
	//mesh.vertices = new math::float3[mesh.num_vertices];
	//memcpy(mesh.vertices, ai_mesh->mVertices, sizeof(math::float3) * mesh.num_vertices);
	//DEBUG("  -> %d vertices", mesh.num_vertices);

	//// Copying normals...
	//mesh.num_normals = ai_mesh->mNumVertices;
	//mesh.normals = new math::float3[mesh.num_normals];
	//memcpy(mesh.normals, ai_mesh->mNormals, sizeof(math::float3) * mesh.num_normals);
	//DEBUG("  -> %d normals", mesh.num_normals);

	//// Copying texture coordinates...
	//uint UV_index = 0;
	//if (ai_mesh->HasTextureCoords(UV_index))
	//{
	//	mesh.num_tex_coord = ai_mesh->mNumVertices;
	//	mesh.tex_coord = new math::float2[mesh.num_tex_coord];
	//	for (uint l = 0; l < mesh.num_tex_coord; ++l)
	//	{
	//		mesh.tex_coord[l].x = ai_mesh->mTextureCoords[UV_index][l].x;
	//		mesh.tex_coord[l].y = ai_mesh->mTextureCoords[UV_index][l].y;
	//	}
	//	DEBUG("  -> %d texture coordinates", mesh.num_tex_coord);
	//}

	//// Copying indicies (faces on Assimp)
	//if (ai_mesh->HasFaces())
	//{
	//	mesh.num_indices = ai_mesh->mNumFaces * 3;
	//	mesh.indices = new uint[mesh.num_indices]; // assume each face is a triangle
	//	DEBUG("  -> %d indices", mesh.num_indices);
	//	for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
	//	{
	//		/*if (ai_mesh->mFaces[j].mNumIndices != 3)
	//			DEBUG("WARNING, geometry face with != 3 indices!");
	//		else
	//		{
	//			memcpy(&mesh.indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
	//		}*/
	//		memcpy(&mesh.indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
	//	}
	//}

	
}

void ComponentMesh::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mesh", &active);
	ImGui::Checkbox("Wireframe##Mesh", &wire);
	ImGui::Text("Number of vertices: %d", resource->mesh_data->num_vertices);
	ImGui::Text("Number of normals: %d", resource->mesh_data->num_normals);
	ImGui::Text("Number of texture coordinates: %d", resource->mesh_data->num_tex_coord);
	ImGui::Text("Number of indices: %d", resource->mesh_data->num_indices);

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
	JSONParser comp_mesh;

	comp_mesh.AddInt("Type", type);
	comp_mesh.AddBoolean("Wire", wire);
	comp_mesh.AddUUID("Resource ID", resource->id);

	go.AddArray(comp_mesh);

	return true;
}

bool ComponentMesh::Load(JSONParser &comp)
{
	wire = comp.GetBoolean("Wire");
	AddResource(App->resource_manager->Get(comp.GetUUID("Resource ID")));

	return true;
}