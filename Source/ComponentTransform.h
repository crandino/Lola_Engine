#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Assimp/include/scene.h"

class ComponentTransform : public Component
{
public:

	math::float3 local_position;
	math::float3 local_scale;
	math::Quat local_rotation_quat;

	math::float3 local_rotation_euler;

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
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
		ImGui::Text(name);

		bool input_changed = false;

		if (ImGui::DragFloat3("Position", &local_position.x, 1.0f, 0.0f, 0.0f, "%.3f"))
			input_changed = true;
		if (ImGui::DragFloat3("Scale", &local_scale.x, 1.0f, 0.0f, 0.0f, "%.3f"))
			input_changed = true;
		if (ImGui::DragFloat3("Rotation", &local_rotation_euler.x, 1.0f, 0.0f, 0.0f, "%.3f"))
		{
			input_changed = true;
			EulerToQuat();
		}			

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

		QuatToEuler();

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

		for (uint i = 0; i < game_object->children.size(); ++i)
			game_object->children[i]->transform->CalcWorldTransformMatrix(world_transform);	

		world_transform = world_transform.Transposed();
	}

	math::float4x4 CalcTransformMatrix(math::float3 &pos, math::float3 &scale, math::Quat &rot)
	{
		math::float4x4 trans_mat, rot_mat, scale_mat;
		trans_mat = math::float4x4::Translate(pos);		
		math::vec axis(rot.x, rot.y, rot.z);
		rot_mat = math::float4x4::RotateAxisAngle(axis.Normalized(), rot.Angle());
		scale_mat = math::float4x4::Scale(scale).ToFloat4x4();
		
		math::float4x4 mat = trans_mat * rot_mat *  scale_mat;
		
		return mat;
	}

	void QuatToEuler()
	{
		math::float3 euler_rad = local_rotation_quat.ToEulerXYZ();
		local_rotation_euler.x = math::RadToDeg(euler_rad.x);
		local_rotation_euler.y = math::RadToDeg(euler_rad.y);
		local_rotation_euler.z = math::RadToDeg(euler_rad.z);
	}

	void EulerToQuat()
	{
		math::float3 euler_quat;

		euler_quat.x = math::DegToRad(local_rotation_euler.x);
		euler_quat.y = math::DegToRad(local_rotation_euler.y);
		euler_quat.z = math::DegToRad(local_rotation_euler.z);

		float t0 = math::Cos(euler_quat.x * 0.5f);
		float t1 = math::Sin(euler_quat.x * 0.5f);
		float t2 = math::Cos(euler_quat.y * 0.5f);
		float t3 = math::Sin(euler_quat.y * 0.5f);
		float t4 = math::Cos(euler_quat.z * 0.5f);
		float t5 = math::Sin(euler_quat.z * 0.5f);

		local_rotation_quat.w = t0 * t2 * t4 + t1 * t3 * t5;
		local_rotation_quat.x = t0 * t3 * t4 - t1 * t2 * t5;
		local_rotation_quat.y = t0 * t2 * t5 + t1 * t3 * t4;
		local_rotation_quat.z= t1 * t2 * t4 - t0 * t3 * t5;
	}
};

#endif __COMPONENTTRANSFORM_H__
