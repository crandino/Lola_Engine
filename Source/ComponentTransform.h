#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

#include "Assimp/include/scene.h"

class ComponentTransform : public Component
{
public:

	math::float3 local_position;
	math::float3 local_scale;
	math::Quat local_rotation;

	math::float4x4 transform;

	ComponentTransform()
	{
		type = COMPONENT_TYPE::TRANSFORM;
		name = GetNameByType(type);
		transform.SetIdentity();
	}

	void ShowEditorInfo()
	{
		ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
		ImGui::Text(name);

		bool input_changed = false;

		if (ImGui::InputFloat3("Position", &local_position.x, 3, ImGuiInputTextFlags_EnterReturnsTrue))
			input_changed = true;
		if (ImGui::InputFloat3("Scale", &local_scale.x, 3, ImGuiInputTextFlags_EnterReturnsTrue))
			input_changed = true;
		if(ImGui::InputFloat4("Rotation", &local_rotation.x, 3, ImGuiInputTextFlags_EnterReturnsTrue))
			input_changed = true;

		if (ImGui::TreeNode("Transform matrix"))
		{
			ImGui::InputFloat4("", &transform[0][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &transform[1][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &transform[2][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat4("", &transform[3][0], 3, ImGuiInputTextFlags_ReadOnly);
			ImGui::TreePop();
		}		

		ImGui::Separator();

		if (input_changed)
			;		
	}

	void SetComponent(aiNode *go)
	{
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotating;

		go->mTransformation.Decompose(scaling, rotating, translation);

		local_position = { translation.x, translation.y, translation.z };
		local_scale = { scaling.x, scaling.y, scaling.z };
		local_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

		transform = CalcTransformMatrix(local_position, local_scale, local_rotation);

		math::float3 parent_position;
		math::float3 parent_scale;
		math::Quat parent_rotation;

		aiNode *node = go->mParent;
		//math::float4x4 parent_matrix; parent_matrix.SetIdentity();

		while (node != nullptr)
		{
			node->mTransformation.Decompose(scaling, rotating, translation);
			parent_position = { translation.x, translation.y, translation.z };
			parent_scale = { scaling.x, scaling.y, scaling.z };
			parent_rotation = { rotating.x, rotating.y, rotating.z, rotating.w };

			math::float4x4 parent_matrix = CalcTransformMatrix(parent_position, parent_scale, parent_rotation);
			transform = parent_matrix * transform;
			node = node->mParent;
		}

		transform = transform.Transposed();
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

};

#endif __COMPONENTTRANSFORM_H__
