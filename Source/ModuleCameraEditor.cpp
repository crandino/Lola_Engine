#include "ModuleCameraEditor.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGameObjectManager.h"
//#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

//#include "PhysBody3D.h"

#include "MathGeoLib\MathGeoLib.h"

ModuleCameraEditor::ModuleCameraEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Camera3D");	
}

ModuleCameraEditor::~ModuleCameraEditor()
{}


bool ModuleCameraEditor::Init()
{
	camera = App->gameobject_manager->CreateGameObject("Main_camera", nullptr);
	camera->AddComponent(COMPONENT_TYPE::TRANSFORM);
	ComponentCamera *c = (ComponentCamera*)camera->AddComponent(COMPONENT_TYPE::CAMERA);
	c->editor_camera = true;
	c->SetComponent();

	//App->renderer3D->SetFrustumForProjection(c->cam_frustum);

	return true;
}

bool ModuleCameraEditor::Start()
{
	DEBUG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCameraEditor::CleanUp()
{
	DEBUG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
UPDATE_STATUS ModuleCameraEditor::Update(float dt)
{
	math::vec movement = math::vec::zero;

	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += speed * camera->transform->up;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement += speed * -camera->transform->up;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += speed * camera->transform->forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement += speed * -camera->transform->forward;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement += speed * camera->transform->left;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += speed * -camera->transform->left;

	camera->transform->Move(movement);

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity = 0.025f;

		camera->transform->RotateAngleAxis(dx * sensitivity, camera->transform->up);
		//main_camera->transform->RotateAngleAxis(dy * sensitivity, main_camera->transform->left);
	}
	
	return UPDATE_CONTINUE;
}

void ModuleCameraEditor::SetAsEditorCamera(GameObject *go) { camera = go; }
const math::vec &ModuleCameraEditor::GetCameraPos() const { return camera->transform->GetPos(); }
const GameObject *ModuleCameraEditor::GetEditorCamera() const { return camera; }