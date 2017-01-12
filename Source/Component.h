#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSONParser.h"

#include "Application.h"
#include "ModuleResourceManager.h"

class GameObject;
class Resource;

enum COMPONENT_TYPE
{
	UNKNOWN,

	// Generic components
	TRANSFORM, 
	MATERIAL,
	MESH,
	CAMERA,

	// UI components
	TRANSFORM_2D,
	UI_IMAGE,
	UI_LABEL, 
	UI_BUTTON
};

class Component
{

protected:

	const char				 *name;
	COMPONENT_TYPE            type;
	bool					active;	

public:
	
	GameObject *game_object = nullptr;
	long unsigned int	      UUID;

	Component();
	virtual ~Component();

	virtual bool Update();
	virtual void ShowEditorInfo();

	COMPONENT_TYPE &GetType();
	const char *GetNameByType(COMPONENT_TYPE type);
	bool IsActive() const;

	virtual void AddResource(Resource *res)
	{ }

	virtual bool Save(JSONParser &go);
	virtual bool Load(JSONParser &component);
};

#endif //!__COMPONENT_H__
