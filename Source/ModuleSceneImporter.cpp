#include "ModuleSceneImporter.h"

ModuleSceneImporter::ModuleSceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Scene Importer");
}

ModuleSceneImporter::~ModuleSceneImporter()
{ }