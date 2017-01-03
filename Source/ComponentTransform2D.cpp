#include "ComponentTransform2D.h"

#include "GameObject.h"

#include "ModuleWindow.h"
#include "Application.h"

#include "imgui\imgui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "openGL.h"

#include <stack>

ComponentTransform2D::ComponentTransform2D() : Component()
{
	local_position.Set(0.0f, 0.0f, 0.0f);
	global_position.Set(0.0f, 0.0f, 0.0f);
	
	size.Set(App->window->GetScreenWidth(), App->window->GetScreenHeight());

	canvas.num_vertices = 4;
	canvas.vertices = new math::float3[canvas.num_vertices];
	canvas.vertices[0] = local_position;
	canvas.vertices[1] = local_position + math::float3(size.x, 0.0f, 0.0f);
	canvas.vertices[2] = local_position + math::float3(0.0f, size.y, 0.0f);
	canvas.vertices[3] = local_position + math::float3(size, 0.0f);

	canvas.num_indices = 6;
	canvas.indices = new unsigned int[canvas.num_indices];
	canvas.indices[0] = 0;
	canvas.indices[1] = 2;
	canvas.indices[2] = 1;
	canvas.indices[3] = 1;
	canvas.indices[4] = 2;
	canvas.indices[5] = 3;

	type = COMPONENT_TYPE::TRANSFORM_2D;
	name = GetNameByType(type);
}

bool ComponentTransform2D::Update()
{
	if (apply_transformation)
	{
		RecalcTranslations();
		apply_transformation = false;
	}		

	glEnableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();	
	glOrtho(0, App->window->GetScreenWidth(), App->window->GetScreenHeight(), 0, -1, 1);	//
	glMatrixMode(GL_MODELVIEW);               // Select Modelview Matrix
	glPushMatrix();                     // Push The Matrix
	glLoadIdentity();

	glVertexPointer(3, GL_FLOAT, 0, canvas.vertices);
	glDrawElements(GL_TRIANGLES, canvas.num_indices, GL_UNSIGNED_INT, canvas.indices);

	glMatrixMode(GL_PROJECTION);               // Select Projection
	glPopMatrix();                     // Pop The Matrix
	glMatrixMode(GL_MODELVIEW);               // Select Modelview
	glPopMatrix();                     // Pop The Matrix

	glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

void ComponentTransform2D::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	if (ImGui::DragFloat3("Position", &local_position.x, 0.25f, 0.0f, 0.0f, "%.3f"))
		apply_transformation = true;

	if (ImGui::DragFloat2("Size", &size.x, 0.25f, 1.0f, 1000.0f, "%.3f"))
		apply_transformation = true;

	ImGui::Separator();
}

// CalcWorldTransformMatrix recalculates both parent and local transformation for the current gameobject. Besides, the method recursively
// recalculates the world matrix transformation for all its children.
void ComponentTransform2D::RecalcTranslations()
{
	CalcGlobalTranslation();

	for (uint i = 0; i < game_object->children.size(); ++i)
		game_object->children[i]->transform_2d->CalcGlobalTranslation();
}

void ComponentTransform2D::CalcGlobalTranslation()
{
	if (game_object->parent->transform_2d != nullptr)
		global_position = game_object->parent->transform_2d->GetGlobalPos() + local_position;
	else
		global_position = local_position;
}

void ComponentTransform2D::Move(const math::vec &movement)
{
	local_position += movement;
	apply_transformation = true;
}

const math::vec &ComponentTransform2D::GetLocalPos() const
{
	return local_position;
}

const math::vec &ComponentTransform2D::GetGlobalPos() const
{
	return global_position;
}

void ComponentTransform2D::SetLocalPos(const math::vec &local_pos)
{
	local_position = local_pos;
	apply_transformation = true;
}

void ComponentTransform2D::SetSize(const math::float2 &size)
{
	this->size = size;
}

bool ComponentTransform2D::Save(JSONParser &go)
{
	/*JSONParser comp_trans;

	comp_trans.AddInt("Type", type);

	comp_trans.AddPoints("Local position", local_position, 3);
	comp_trans.AddPoints("Local scale", local_scale, 3);
	comp_trans.AddPoints("Local rotation", local_rotation_quat, 4);

	comp_trans.AddPoints("Parent transform", parent_transform, 16);

	comp_trans.AddPoints("Forward", forward, 3);
	comp_trans.AddPoints("Left", left, 3);
	comp_trans.AddPoints("Up", up, 3);

	go.AddArray(comp_trans);*/

	return true;
}

bool ComponentTransform2D::Load(JSONParser &comp)
{
	//comp.GetPoints("Local position", local_position, 3);
	//comp.GetPoints("Local scale", local_scale, 3);
	//comp.GetPoints("Local rotation", local_rotation_quat, 4);

	//comp.GetPoints("Parent transform", parent_transform, 16);

	//comp.GetPoints("Forward", forward, 3);
	//comp.GetPoints("Left", left, 3);
	//comp.GetPoints("Up", up, 3);

	//CalcWorldTransformMatrix();

	//// Inspector will show Euler representation on degrees to be more understandable.
	//local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
	//local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);
	//local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);

	return true;
}


