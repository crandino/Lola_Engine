#include "ModuleGameObjectManager.h"

#include "Application.h"
#include "ModuleInput.h"

#include "GameObject.h"

ModuleGameObjectManager::ModuleGameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("GameObject Manager");
}

// Destructor
ModuleGameObjectManager::~ModuleGameObjectManager()
{ }

// Called before render is available
bool ModuleGameObjectManager::Init()
{
	bool ret = true;

	root = CreateGameObject("Root", nullptr);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		CreateGameObject("A", root);
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		CreateGameObject("B", nullptr);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGameObjectManager::CleanUp()
{
	bool ret = true;
	
	return ret;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name, GameObject *parent)
{
	GameObject *new_go = nullptr;

	// With nullptr pointer parent, root will be the parent if it's not the first gameobject on the game.
	if (parent == nullptr && id != 0)
		parent = root;				
	
	new_go = new GameObject(id++, name, parent);

	return new_go;
}

GameObject *ModuleGameObjectManager::GetGameObject(uint id_to_search)
{
	std::stack<GameObject*> go_stack;
	go_stack.push(root);
	
	while (!go_stack.empty())
	{
		GameObject *top_go = go_stack.top();
		int num_children = top_go->children.size;

		if (num_children > 0)
		{
			for (int i = num_children; i > 0; --i)
				go_stack.push(top_go->children[i]);
		}
		else
		{
			if (top_go->id == id_to_search)
				return top_go;
			else
				go_stack.pop();
		}		
	}

	return nullptr;

}