#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include <vector>

class Component;

class GameObject
{

private:

	uint						     id;
	bool						 active;
	char			 name[SHORT_STRING];
	std::vector<Component*>  components;
	std::vector<GameObject*>   children;

public:

	GameObject(uint id, const char* name, GameObject *parent)
	{
		active = true;
		this->id = id;
		sprintf_s(this->name, SHORT_STRING, name);

		if (parent != nullptr) 	
			parent->children.push_back(this);
	
	}
	
};





#endif //!__GAMEOBJECT_H__
