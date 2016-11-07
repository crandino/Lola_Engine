#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

#include "JSONParser.h"

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
	ComponentTransform				*transform = nullptr;
	bool							 transform_applied = false;

	std::vector<Component*>			 components;
	std::vector<GameObject*>         children;
	uint						     id;
	long unsigned int				 UUID;

	bool						     active;
	bool					         selected;
	bool							 to_delete;

	GameObject(uint id, const char* name, GameObject *parent);
	~GameObject();

	const Component *GetComponentByType(COMPONENT_TYPE type) const;
	const Component *AddComponent(COMPONENT_TYPE type);

	bool GetAABB(math::AABB &aabb) const;
	bool GetFrustum(math::Frustum &frustum) const;
	bool HasMesh() const;

	void ChangeActiveState(bool *state, GameObject *go = nullptr);
	
	void Enable();
	void Disable();
	bool IsActive() const;
	const char *GetName() const;

	bool Save(JSONParser &game_objects);
	
};

#endif //!__GAMEOBJECT_H__
