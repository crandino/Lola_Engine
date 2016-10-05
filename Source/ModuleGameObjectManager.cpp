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

	// With nullptr pointer parent...
	if (parent == nullptr && id != 0)
		parent = root;
	
	new_go = new GameObject(id++, name, parent);

	return new_go;
}

const GameObject *ModuleGameObjectManager::GetRoot()
{
	return root;
}