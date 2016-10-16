#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"

#include <vector>

enum COMPONENT_TYPE;

class Component;
class ComponentTransform;

class GameObject
{

private:	

	char			 name[SHORT_STRING];	

public:

	// Unique components
	ComponentTransform		 *transform = nullptr;

	std::vector<Component*>			 components;
	std::vector<GameObject*>         children;
	uint						     id;
	bool						     active;
	bool					         selected;

	GameObject(uint id, const char* name, GameObject *parent);
	~GameObject();

	const Component *GetComponentByType(COMPONENT_TYPE type);
	void AddComponent(Component *comp);

	void ChangeActiveState(bool *state, GameObject *go = nullptr);
	
	void Enable();
	void Disable();
	bool IsActive() const;
	const char *GetName() const;
	
};

#endif //!__GAMEOBJECT_H__
