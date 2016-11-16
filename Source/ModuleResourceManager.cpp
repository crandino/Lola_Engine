#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Resource Manager");
}

ModuleResourceManager::~ModuleResourceManager()
{ }
