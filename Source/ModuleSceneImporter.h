#ifndef __MODULESCENEIMPORTER_H__
#define __MODULESCENEIMPORTER_H__

#include "Module.h"

class ModuleSceneImporter : public Module
{
public:

	ModuleSceneImporter(Application* app, bool start_enabled = true);
	~ModuleSceneImporter();

	bool Awake(JSONParser &config);
	UPDATE_STATUS PreUpdate(float dt);
	/*UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);*/
	bool CleanUp();

	void ImportModel(const char *file_name, bool use_fs = true);
};


#endif __MODULESCENEIMPORTER_H__
