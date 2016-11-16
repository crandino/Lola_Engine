#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

class ModuleResourceManager : public Module
{

public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();
};

#endif // !__MODULERESOURCEMANAGER_H__
