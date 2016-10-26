#include "ComponentCamera.h"

#include "ComponentTransform.h"
#include "GameObject.h"

#include "imgui\imgui.h"

ComponentCamera::ComponentCamera()
{
	camera_modified = false;	

	math::vec pos = { 0.0f, 0.0f, 0.0f };
	math::vec fwd = { 0.0f, 0.0f, -1.0f };
	math::vec up = { 0.0f, 1.0f, 0.0f };

	origin_cam_frustum.SetFrame(pos, fwd, up);
	origin_cam_frustum.SetKind(math::FrustumProjectiveSpace::FrustumSpaceGL, math::FrustumHandedness::FrustumRightHanded);

	near_plane = near_plane_tmp = 1.0f;
	far_plane = far_plane_tmp = 25.0f; 
	max_far_plane = 50.0f;

	aspect_ratio = 1.333f;
	FOV_v = 60.0f;	

	UpdateFrustum();
	
	type = COMPONENT_TYPE::CAMERA;
	name = GetNameByType(type);
}

void ComponentCamera::SetComponent()
{
	ApplyTransformToFrustum();
}

bool ComponentCamera::Update()
{
	if (camera_modified)
	{
		UpdateFrustum();
		ApplyTransformToFrustum();
	}			

	if (game_object->transform_applied)
		ApplyTransformToFrustum();

	forward = (game_object->transform->world_transform.RotatePart() * math::float3(0.0f, 0.0f, -1.0f)).Normalized();
	left = (game_object->transform->world_transform.RotatePart() * math::float3(1.0f, 0.0f, 0.0f)).Normalized();
	up = (game_object->transform->world_transform.RotatePart() * math::float3(0.0f, 1.0f, 0.0f)).Normalized();

	return true;
}

void ComponentCamera::UpdateFrustum()
{
	near_plane_tmp = (near_plane_tmp < 1.0f) ? 1.0f : near_plane_tmp;
	far_plane_tmp = (far_plane_tmp > max_far_plane) ? max_far_plane : far_plane_tmp;

	near_plane_tmp = (near_plane_tmp > far_plane_tmp) ? far_plane : near_plane_tmp;	
	far_plane_tmp = (far_plane_tmp < near_plane_tmp) ? near_plane : far_plane_tmp;	

	near_plane = near_plane_tmp;
	far_plane = far_plane_tmp;

	origin_cam_frustum.SetViewPlaneDistances(near_plane, far_plane);
	CalcHorizontalFOV();

	camera_modified = false;
}

void ComponentCamera::ApplyTransformToFrustum()
{
	math::float4x4 transform = game_object->transform->world_transform;
	transform.RemoveScale();

	cam_frustum = origin_cam_frustum;
	cam_frustum.Transform(transform);
}

void ComponentCamera::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	near_plane_tmp = near_plane;
	far_plane_tmp = far_plane;

	if (ImGui::DragFloat("Near plane", &near_plane_tmp, 0.5f)) camera_modified = true;
	if (ImGui::DragFloat("Far plane", &far_plane_tmp, 0.5f)) camera_modified = true;

	float FOV_h_deg = math::RadToDeg(FOV_h);
	ImGui::DragFloat("Horizontal FOV", &FOV_h_deg, 0.5f, 0.0f, 180.0f);
	ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.5f, 1.0f, 2.0f);

	ImGui::Separator();
}

void ComponentCamera::CalcHorizontalFOV()
{
	FOV_h = 2.0f * math::Atan(math::Tan(math::DegToRad(FOV_v) / 2.0f) * aspect_ratio);
	origin_cam_frustum.SetPerspective(FOV_h, math::DegToRad(FOV_v));
}
