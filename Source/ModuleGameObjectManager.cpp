#include "ModuleGameObjectManager.h"

ModuleGameObjectManager::ModuleGameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("GameObject Manager");
}

// Destructor
ModuleGameObjectManager::~ModuleGameObjectManager()
{}

// Called before render is available
bool ModuleGameObjectManager::Init()
{
	bool ret = true;
	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGameObjectManager::CleanUp()
{
	bool ret = true;
	
	return ret;
}