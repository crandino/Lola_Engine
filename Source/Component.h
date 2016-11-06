#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSONParser.h"

class GameObject;

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

	virtual bool Save(JSONParser &go);
};

#endif //!__COMPONENT_H__
