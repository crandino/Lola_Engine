#include "ComponentCamera.h"

#include "ComponentTransform.h"
#include "GameObject.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGameObjectManager.h"

#include "imgui\imgui.h"

ComponentCamera::ComponentCamera() : Component()
{
	camera_modified = true;
	editor_camera = false;

	math::vec pos = { 0.0f, 0.0f, 0.0f };
	math::vec fwd = { 0.0f, 0.0f, 1.0f };
	math::vec up = { 0.0f, 1.0f, 0.0f };

	origin_cam_frustum.SetFrame(pos, fwd, up);
	origin_cam_frustum.SetKind(math::FrustumProjectiveSpace::FrustumSpaceGL, math::FrustumHandedness::FrustumRightHanded);

	near_plane = near_plane_tmp = 0.5f;
	far_plane = far_plane_tmp = 200.0f; 
	max_far_plane = 1000.0f;
	min_near_plane = 0.5f;

	aspect_ratio = 1.333f;
	FOV_v = 60.0f;	

	type = COMPONENT_TYPE::CAMERA;
	name = GetNameByType(type);
}

void ComponentCamera::SetComponent()
{
	UpdateFrustum();
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
	camera_modified = false;

	near_plane_tmp = (near_plane_tmp < min_near_plane) ? min_near_plane : near_plane_tmp;
	far_plane_tmp = (far_plane_tmp > max_far_plane) ? max_far_plane : far_plane_tmp;

	near_plane_tmp = (near_plane_tmp > far_plane_tmp) ? far_plane : near_plane_tmp;	
	far_plane_tmp = (far_plane_tmp < near_plane_tmp) ? near_plane : far_plane_tmp;	

	near_plane = near_plane_tmp;
	far_plane = far_plane_tmp;

	origin_cam_frustum.SetViewPlaneDistances(near_plane, far_plane);
	origin_cam_frustum.SetVerticalFovAndAspectRatio(math::DegToRad(FOV_v), aspect_ratio);

	if(editor_camera)
		App->renderer3D->SetFrustumForProjection(origin_cam_frustum);
}

void ComponentCamera::ApplyTransformToFrustum()
{
	cam_frustum = origin_cam_frustum;

	math::float4x4 transform = game_object->transform->world_transform;
	transform.RemoveScale();
	
	cam_frustum.Transform(transform);
	if (editor_camera) 
		App->renderer3D->SetFrustumForView(cam_frustum);
}

void ComponentCamera::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	near_plane_tmp = near_plane;
	far_plane_tmp = far_plane;

	if (ImGui::Checkbox("Editor Camera", &editor_camera))
		if (editor_camera)
		{
			App->gameobject_manager->SetEditorCamera(this);
			camera_modified = true;
		}			
		else
			editor_camera = true;

	if (ImGui::DragFloat("Near plane", &near_plane_tmp, 0.5f)) camera_modified = true;
	if (ImGui::DragFloat("Far plane", &far_plane_tmp, 0.5f)) camera_modified = true;

	if(ImGui::DragFloat("FOV", &FOV_v, 1.0f, 30.0f, 140.0f)) camera_modified = true;
	if (ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.05f, 1.0f, 2.0f)) camera_modified = true;

	ImGui::Separator();
}
