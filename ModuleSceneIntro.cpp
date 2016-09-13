#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Point2D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{ }

ModuleSceneIntro::~ModuleSceneIntro()
{ }

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro scene");
	bool ret = true;
	return ret;
}

// Cleaning assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	return true;
}

// Update
UPDATE_STATUS ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

