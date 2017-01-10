#ifndef __MODULEFONTMANAGER_H__
#define __MODULEFONTMANAGER_H__

#include "Module.h"

class ModuleFontManager : public Module
{
public:

	ModuleFontManager(Application* app, bool start_enabled = true);
	~ModuleFontManager();

	bool Awake(JSONParser &config);

	bool Start();

	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

private:

};

#endif // !__MODULEFONTMANAGER_H__
