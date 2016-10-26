#include "ModuleCameraEditor.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGameObjectManager.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

//#include "PhysBody3D.h"

#include "MathGeoLib\MathGeoLib.h"

ModuleCameraEditor::ModuleCameraEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Camera3D");

	//CalculateViewMatrix();

	X.Set(1.0f, 0.0f, 0.0f);
	Y.Set(0.0f, 1.0f, 0.0f);
	Z.Set(0.0f, 0.0f, 1.0f);

	Position.Set(0.0f, 0.0f,0.0f);
	Reference.Set(0.0f, 0.0f, 0.0f);
}

ModuleCameraEditor::~ModuleCameraEditor()
{}


bool ModuleCameraEditor::Init()
{
	//CreateEditorCamera();
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
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		CreateEditorCamera();

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	// Mouse motion ----------------
	/*if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		int dx = App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		ComponentCamera* cam = (ComponentCamera*)main_camera->GetComponentByType(COMPONENT_TYPE::CAMERA);
		if(dx != 0) main_camera->transform->Move((dx * dt) * cam->left);
		if(dy != 0) main_camera->transform->Move((dy * dt) * cam->up);
	}	

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseXMotion();

		main_camera->transform->Move(math::vec(dx * 0.25f, 0.0f, 0.0f));
	}*/

	math::vec newPos(0.0f, 0.0f, 0.0f);

	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.05f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			/* On glmath.h
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));*/
			
			math::float4x4 rot = math::float4x4::RotateAxisAngle(math::vec(0.0f, 1.0f, 0.0f), DeltaX);

			X = (rot * math::float4(X, 1.0f)).Float3Part();
			Y = (rot * math::float4(Y, 1.0f)).Float3Part();
			Z = (rot * math::float4(Z, 1.0f)).Float3Part();
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			/* On glmath.h
			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);*/

			math::float4x4 rot = math::float4x4::RotateAxisAngle(X, DeltaY);			

			Y = (rot * math::float4(Y, 1.0f)).xyz();
			Z = (rot * math::float4(Z, 1.0f)).xyz();

			if(Y.y < 0.0f)
			{
				Z = math::vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCameraEditor::Look(const math::vec &Position, const math::vec &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (math::Cross(math::vec(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCameraEditor::LookAt( const math::vec &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (math::Cross(math::vec(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCameraEditor::Move(const math::vec &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCameraEditor::GetViewMatrix()
{
	return *ViewMatrix.v;
}

// -----------------------------------------------------------------
//void ModuleCameraEditor::CalculateViewMatrix()
//{
//	// With glmath.h
//	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
//
//	// The code below introduce values on a row-format. But doesn't work.
//	/*ViewMatrix = math::float4x4(X.x, X.y, X.z, -X.Dot(Position),
//		Y.x, Y.y, Y.z, -Y.Dot(Position),
//		Z.x, Z.y, Z.z, -Z.Dot(Position),
//		0.0f, 0.0f, 0.0f, 1.0f);*/
//	
//	ViewMatrixInverse = ViewMatrix.Inverted();
//}

void ModuleCameraEditor::CalculateViewMatrix()
{
	// With glmath.h
	/*math::float4x4 mat;
	mat = main_camera->transform->world_transform;
	mat[0][3] *= -1;
	mat[1][3] *= -1;
	mat[2][3] *= -1;*/

	if (App->camera->main_camera == nullptr)
		ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
	else
	{
		ComponentCamera *c = (ComponentCamera*)App->camera->main_camera->GetComponentByType(COMPONENT_TYPE::CAMERA);
		math::float3x4 view;
		view = c->cam_frustum.ComputeViewMatrix();

		math::float4x4 view_matrix;
		view_matrix[0][0] = view[0][0];
		view_matrix[0][1] = view[1][0];
		view_matrix[0][2] = view[2][0];
		view_matrix[0][3] = 0.0f;
		view_matrix[1][0] = view[0][1];
		view_matrix[1][1] = view[1][1];
		view_matrix[1][2] = view[1][2];
		view_matrix[1][3] = 0.0f;
		view_matrix[2][0] = view[0][2];
		view_matrix[2][1] = view[1][2];
		view_matrix[2][2] = view[2][2];
		view_matrix[2][3] = 0.0f;
		view_matrix[3][0] = view[0][3];
		view_matrix[3][1] = view[1][3];
		view_matrix[3][2] = view[2][3];
		view_matrix[3][3] = 1.0f;			

		ViewMatrix = view_matrix;
	}
		
	//ViewMatrix = mat.Transposed();
	//ViewMatrixInverse = ViewMatrix.Inverted();
}

void ModuleCameraEditor::CreateEditorCamera()
{
	main_camera = App->gameobject_manager->CreateGameObject("Main_camera", nullptr);
	main_camera->AddComponent(COMPONENT_TYPE::TRANSFORM);
	ComponentCamera *c = (ComponentCamera*)main_camera->AddComponent(COMPONENT_TYPE::CAMERA);
	c->SetComponent();
}