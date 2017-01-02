#include "ComponentTransform2D.h"

#include "GameObject.h"

#include "ModuleWindow.h"
#include "Application.h"

#include "imgui\imgui.h"

#include <stack>

ComponentTransform2D::ComponentTransform2D() : Component()
{
	world_transform.SetIdentity();
	local_transform.SetIdentity();
	parent_transform.SetIdentity();

	local_position.Set(0.0f, 0.0f, 0.0f);
	local_size.Set(100.0f, 100.0f);

	/*local_rotation_euler_rad.Set(0.0f, 0.0f, 0.0f);
	local_rotation_euler_deg.Set(0.0f, 0.0f, 0.0f);*/
	local_rotation_quat.Set(0.0f, 0.0f, 0.0f, 1.0f);

	type = COMPONENT_TYPE::TRANSFORM_2D;
	name = GetNameByType(type);
}

bool ComponentTransform2D::Update()
{
	if (game_object->transform_applied)
		RecalcTransformations();

	return true;
}

void ComponentTransform2D::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	if (ImGui::DragFloat3("Position", &local_position.x, 0.25f, 0.0f, 0.0f, "%.3f"))
		game_object->transform_applied = true;

	if (ImGui::DragFloat2("Size", &local_size.x, 0.25f, 1.0f, 1000.0f, "%.3f"))
		game_object->transform_applied = true;

	ImGui::Separator();
}

// CalcWorldTransformMatrix recalculates both parent and local transformation for the current gameobject. Besides, the method recursively
// recalculates the world matrix transformation for all its children.
void ComponentTransform2D::RecalcTransformations()
{
	CalcWorldTransformMatrix();

	std::stack<GameObject*> go_stack;
	GameObject *go = game_object;

	while (go != nullptr)
	{
		//math::float4x4 parent_matrix = go->transform->world_transform;

		for (uint i = 0; i < go->children.size(); ++i)
			go_stack.push(go->children[i]);

		if (go_stack.empty())
			go = nullptr;
		else
		{
			go = go_stack.top();
			go_stack.pop();
			//go->transform->parent_transform = parent_matrix;
			//go->transform->CalcWorldTransformMatrix();
			go->transform_applied = true;				 // Children needs update for AABB, Frustum.. on their own Updates().
		}
	}
}

void ComponentTransform2D::CalcWorldTransformMatrix()
{
	//local_transform = CalcTransformMatrix(local_position, local_scale, local_rotation_quat);
	world_transform = parent_transform * local_transform;

	//world_transform = world_transform.Transposed();
}

math::float4x4 ComponentTransform2D::CalcTransformMatrix(const math::float3 &pos, const math::float3 &scale, const math::Quat &rot)
{
	math::float4x4 mat = math::float4x4::FromTRS(pos, rot, scale);
	return mat;
}

void ComponentTransform2D::Move(const math::vec &movement)
{
	local_position += movement;
	game_object->transform_applied = true;
}

void ComponentTransform2D::SetPos(const math::vec &pos)
{
	local_position = pos;
	game_object->transform_applied = true;
}

const math::vec &ComponentTransform2D::GetPos() const
{
	return local_position;
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


