#include "ModuleFontManager.h"

ModuleFontManager::ModuleFontManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Font_Manager");
}

// Destructor
ModuleFontManager::~ModuleFontManager()
{}

// Called before render is available
bool ModuleFontManager::Awake(JSONParser &config)
{
	bool ret = true;
	return ret;
}

// Called before the first frame
bool ModuleFontManager::Start()
{
	return true;
}

// Update all guis
UPDATE_STATUS ModuleFontManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called after all Updates
UPDATE_STATUS ModuleFontManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFontManager::CleanUp()
{
	DEBUG("Freeing Font Manager");

	return true;
}
