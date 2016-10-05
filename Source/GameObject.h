#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include <vector>

#include "Component.h"

class ComponentTransform;

class GameObject
{

private:	
	
	bool						 active;
	char			 name[SHORT_STRING];	

public:

	// Unique components
	ComponentTransform		 *transform = nullptr;


	std::vector<Component*>  components;
	std::vector<GameObject*>   children;
	uint						     id;

	GameObject(uint id, const char* name, GameObject *parent)
	{
		active = true;
		this->id = id;
		sprintf_s(this->name, SHORT_STRING, name);

		if (parent != nullptr) 	
			parent->children.push_back(this);	
	}

	void AddComponent(Component *comp)
	{
		comp->game_object = this;
		components.push_back(comp);

		if (comp->GetType() == COMPONENT_TYPE::TRANSFORM && transform == nullptr)
			transform = (ComponentTransform*)comp;
	}

	void Enable()	{ active = true;  }
	void Disable()  { active = false;  }
	bool isActive() const { return active; }
	
};

#endif //!__GAMEOBJECT_H__
