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
	test.SetNegativeInfinity();

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

	//transform_aabb.SetIdentity();
	//quat_aabb = quat_aabb.identity;

	//// Calcultating AABB	
	//bounding_box.SetNegativeInfinity();
	//bounding_box.Enclose((math::float3*) vertices, num_vertices);
	//test = bounding_box.ToOBB();
	
	/*ScaleAABB();
	RotateAABB();
	TranslateAABB();	*/
	
	//UpdateTransformAABB();
	//UpdateTransformAABB();
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

void ComponentMesh::TranslateAABB()
{
	math::float3 displacement = (game_object->transform->world_transform.Transposed().TranslatePart() - transform_aabb.Transposed().TranslatePart());
	bounding_box.Translate(displacement);	
}

void ComponentMesh::RotateAABB()
{
	math::float3x3 rot = (game_object->transform->world_transform.RotatePart());// -transform_aabb.Transposed().RotatePart());
	
	//math::float4x4 transformation; 
	math::OBB tmp = bounding_box.Transform(rot.Transposed());	
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(tmp);
	
}

void ComponentMesh::ScaleAABB()
{
	math::float3 scale_factor;

	scale_factor.x = game_object->transform->world_transform.GetScale().x / transform_aabb.GetScale().x;
	scale_factor.y = game_object->transform->world_transform.GetScale().y / transform_aabb.GetScale().y;
	scale_factor.z = game_object->transform->world_transform.GetScale().z / transform_aabb.GetScale().z;
	
	bounding_box.Scale(bounding_box.CenterPoint(), scale_factor);
}

void ComponentMesh::UpdateTransformAABB()
{
	math::float4x4 transform = transform_aabb.Inverted() * game_object->transform->world_transform;
	
	//test.Transform(transform.Transposed());
	test.Transform(transform.Transposed());
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose(test);

	transform_aabb = game_object->transform->world_transform;

	/*math::float3 scale_factor;

	scale_factor.x = game_object->transform->world_transform.GetScale().x / transform_aabb.GetScale().x;
	scale_factor.y = game_object->transform->world_transform.GetScale().y / transform_aabb.GetScale().y;
	scale_factor.z = game_object->transform->world_transform.GetScale().z / transform_aabb.GetScale().z;

	bounding_box.Scale(bounding_box.CenterPoint(), scale_factor);*/

	//math::Quat diff = quat_aabb * game_object->transform->local_rotation_quat.Inverted();

	//DEBUG("%f, %f, %f, %f", diff.x, diff.y, diff.z, diff.w);

	//test.SetNegativeInfinity();
	//
	//test = bounding_box.Transform(diff);
	//bounding_box.SetNegativeInfinity();
	//bounding_box.Enclose(test);

	/*math::float3 displacement = (game_object->transform->world_transform.Transposed().TranslatePart() - transform_aabb.Transposed().TranslatePart());
	bounding_box.Translate(displacement);*/

	//quat_aabb = game_object->transform->local_rotation_quat;
	//transform_aabb = game_object->transform->world_transform;
}