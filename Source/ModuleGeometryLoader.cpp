#include "ModuleGeometryLoader.h"

ModuleGeometryLoader::ModuleGeometryLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Geometry Loader");
}

// Destructor
ModuleGeometryLoader::~ModuleGeometryLoader()
{}

// Called before render is available
bool ModuleGeometryLoader::Init()
{
	bool ret = true;
	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGeometryLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGeometryLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGeometryLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGeometryLoader::CleanUp()
{
	bool ret = true;
	return ret;
}