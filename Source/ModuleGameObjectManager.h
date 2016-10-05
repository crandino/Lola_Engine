#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"

class GameObject;

class ModuleGameObjectManager : public Module
{
public:

	ModuleGameObjectManager(Application* app, bool start_enabled = true);
	~ModuleGameObjectManager();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	const GameObject *GetRoot();

private:

	uint				id = 0;
	GameObject *root = nullptr;

	GameObject *CreateGameObject(const char *name, GameObject *parent);

};

#endif // !__MODULEGAMEOBJECTMANAGER_H__


