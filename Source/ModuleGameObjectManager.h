#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
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

	void ImportModel(const char *file_name, bool use_fs = true);
	const GameObject *GetRoot() const;

	GameObject *CreateGameObject(const char *name, GameObject *parent);
	bool DeleteGameObject(const GameObject *go_to_delete);

private:

	uint						    id_to_assign = 0;
	GameObject					    *root = nullptr;
	std::vector<GameObject*>	    list_of_gos;

	
	GameObject		*GetGameObject(uint id_to_search) const;

	bool DeleteGameObject(unsigned int id_to_delete);
	bool DeleteChildrenGameObject(const GameObject *go_to_delete);
	bool RemoveChildFromChildren(const GameObject *go_to_delete);

};

#endif // !__MODULEGAMEOBJECTMANAGER_H__


