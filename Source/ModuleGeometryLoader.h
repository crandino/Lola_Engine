#ifndef __MODULEGEOMETRYLOADER_H__
#define __MODULEGEOMETRYLOADER_H__

#include "Module.h"

class ModuleGeometryLoader : public Module
{
public:

	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	~ModuleGeometryLoader();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

private:

};

#endif // !__MODULEGEOMETRYLOADER_H__

