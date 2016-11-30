#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSONParser.h"

class GameObject;
class Resource;

enum COMPONENT_TYPE
{
	UNKNOWN,
	TRANSFORM,
	MATERIAL,
	MESH,
	CAMERA
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

	virtual void AddResource(const Resource *res) = 0;

	virtual bool Save(JSONParser &go);
	virtual bool Load(JSONParser &go);
};

#endif //!__COMPONENT_H__
