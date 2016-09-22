#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib\MathGeoLib.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Camera3D");

	CalculateViewMatrix();

	X.Set(1.0f, 0.0f, 0.0f);
	Y.Set(0.0f, 1.0f, 0.0f);
	Z.Set(0.0f, 0.0f, 1.0f);

	Position.Set(0.0f, 0.0f, 5.0f);
	Reference.Set(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->console.AddLOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	App->console.AddLOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
UPDATE_STATUS ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

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
void ModuleCamera3D::Look(const math::vec &Position, const math::vec &Reference, bool RotateAroundReference)
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
void ModuleCamera3D::LookAt( const math::vec &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (math::Cross(math::vec(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const math::vec &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return *ViewMatrix.v;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	// With glmath.h
	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);

	// The code below introduce values on a row-format. But doesn't work.
	/*ViewMatrix = math::float4x4(X.x, X.y, X.z, -X.Dot(Position),
		Y.x, Y.y, Y.z, -Y.Dot(Position),
		Z.x, Z.y, Z.z, -Z.Dot(Position),
		0.0f, 0.0f, 0.0f, 1.0f);*/
	
	ViewMatrixInverse = ViewMatrix.Inverted();
}