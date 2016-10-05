#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
#include <stack>
#include <vector>

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

private:

	uint						id = 0;
	GameObject					*root = nullptr;
	std::vector<GameObject*>	list_of_gos;

	GameObject		*CreateGameObject(const char *name, GameObject *parent);
	GameObject		*GetGameObject(uint id_to_search);
	void			ImportModel(const char *file_name);
};

#endif // !__MODULEGAMEOBJECTMANAGER_H__


