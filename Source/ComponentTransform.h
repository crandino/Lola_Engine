#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Assimp/include/scene.h"

class ComponentTransform : public Component
{

private:

	math::float3 local_position;
	math::float3 local_scale;
	math::Quat local_rotation_quat;

	math::float3 local_rotation_euler_rad;
	math::float3 local_rotation_euler_deg;

public:
	
	math::float4x4 world_transform;
	math::float4x4 local_transform;
	math::float4x4 parent_transform;

	ComponentTransform()
	{
		world_transform.SetIdentity();
		local_transform.SetIdentity();
		parent_transform.SetIdentity();

		type = COMPONENT_TYPE::TRANSFORM;
		name = GetNameByType(type);
	}

	void ShowEditorInfo()
	{
		ImGui::Checkbox("Active", &game_object->active);
		ImGui::Separator();

		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
		ImGui::Text(name);

		bool input_changed = false;

		if (ImGui::DragFloat3("Position", &local_position.x, 0.25f, 0.0f, 0.0f, "%.3f"))
			input_changed = true;		
		if (ImGui::DragFloat3("Rotation", &local_rotation_euler_deg.x, 0.25f, 0.0f, 0.0f, "%.3f"))
		{
			input_changed = true;
			EulerToQuat(local_rotation_euler_deg, local_rotation_quat);
		}	
		if (ImGui::DragFloat3("Scale", &local_scale.x, 0.25f, 0.0f, 0.0f, "%.3f"))
			input_changed = true;

		if (ImGui::TreeNode("Transform matrix"))
		{
			ImGui::InputFloat4("", &world_transform[0][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &world_transform[1][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &world_transform[2][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &world_transform[3][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::TreePop();
		}		

		ImGui::Separator();

		if (input_changed)
			CalcWorldTransformMatrix(parent_transform);
	}

	void SetComponent(aiNode *go)
	{
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotating;

		go->mTransformation.Decompose(scaling, rotating, translation);

		local_position = { translation.x, translation.y, translation.z };
		local_scale = { scaling.x, scaling.y, scaling.z };
		local_rotation_quat = { rotating.x, rotating.y, rotating.z, rotating.w };

		QuatToEuler(local_rotation_quat, local_rotation_euler_rad);

		local_rotation_euler_deg.x = math::RadToDeg(local_rotation_euler_rad.x);
		local_rotation_euler_deg.y = math::RadToDeg(local_rotation_euler_rad.y);		
		local_rotation_euler_deg.z = math::RadToDeg(local_rotation_euler_rad.z);

		local_transform = CalcTransformMatrix(local_position, local_scale, local_rotation_quat);

		math::float3 parent_position;
		math::float3 parent_scale;
		math::Quat parent_rotation;

		aiNode *node = go->mParent;
		while (node != nullptr)
		{
			node->mTransformation.Decompose(scaling, rotating, translation);
			parent_position = { translation.x, translation.y, translation.z };
			parent_scale = { scaling.x, scaling.y, scaling.z };
			parent_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

			math::float4x4 parent_matrix = CalcTransformMatrix(parent_position, parent_scale, parent_rotation);
			parent_transform = parent_matrix * parent_transform;
			node = node->mParent;
		}

		world_transform = parent_transform * local_transform;
		world_transform = world_transform.Transposed();
	}

	void CalcWorldTransformMatrix(math::float4x4 parent_mat)
	{
		parent_transform = parent_mat;
		local_transform = CalcTransformMatrix(local_position, local_scale, local_rotation_quat);
		world_transform = parent_transform * local_transform;

		QuatToEuler(local_rotation_quat, local_rotation_euler_rad);

		for (uint i = 0; i < game_object->children.size(); ++i)
			game_object->children[i]->transform->CalcWorldTransformMatrix(world_transform);	

		world_transform = world_transform.Transposed();
	}

	math::float4x4 CalcTransformMatrix(math::float3 &pos, math::float3 &scale, math::Quat &rot)
	{
		math::float4x4 mat = math::float4x4::FromTRS(pos, rot, scale);		
		return mat;
	}

	void QuatToEuler(math::Quat &quat, math::float3 &out_euler)
	{
		float ysqr = quat.y * quat.y;
		float t0 = -2.0f * (ysqr + quat.z * quat.z) + 1.0f;
		float t1 = +2.0f * (quat.x * quat.y + quat.w * quat.z);
		float t2 = -2.0f * (quat.x * quat.z - quat.w * quat.y);
		float t3 = +2.0f * (quat.y * quat.z + quat.w * quat.x);
		float t4 = -2.0f * (quat.x * quat.x + ysqr) + 1.0f;

		t2 = t2 > 1.0f ? 1.0f : t2;
		t2 = t2 < -1.0f ? -1.0f : t2;		
		
		out_euler.y = math::Asin(t2);			// Pitch
		out_euler.x = math::Atan2(t3, t4);		// Roll
		out_euler.z = math::Atan2(t1, t0);		// Yaw

		//Roll  rotation about the X - axis
		//Pitch  rotation about the Y - axis
		//Yaw rotation about the Z - axis*/
	}

	void EulerToQuat(math::float3 &euler, math::Quat &out_quat )
	{
		local_rotation_euler_rad.x = math::DegToRad(euler.x);
		local_rotation_euler_rad.y = math::DegToRad(euler.y);
		local_rotation_euler_rad.z = math::DegToRad(euler.z);

		float t0 = math::Cos(local_rotation_euler_rad.z * 0.5f); // Yaw
		float t1 = math::Sin(local_rotation_euler_rad.z * 0.5f);
		float t2 = math::Cos(local_rotation_euler_rad.x * 0.5f); // Roll
		float t3 = math::Sin(local_rotation_euler_rad.x * 0.5f);
		float t4 = math::Cos(local_rotation_euler_rad.y * 0.5f); // Pitch
		float t5 = math::Sin(local_rotation_euler_rad.y * 0.5f);

		out_quat.w = t0 * t2 * t4 + t1 * t3 * t5;
		out_quat.x = t0 * t3 * t4 - t1 * t2 * t5;
		out_quat.y = t0 * t2 * t5 + t1 * t3 * t4;
		out_quat.z = t1 * t2 * t4 - t0 * t3 * t5;

		out_quat.Normalize();
	}
};

#endif __COMPONENTTRANSFORM_H__
