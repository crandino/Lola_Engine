#ifndef __COMPONENT_H__
#define __COMPONENT_H__

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
	long unsigned int		  UUID;

public:
	
	GameObject *game_object = nullptr;

	Component();
	virtual ~Component();

	virtual bool Update();
	virtual void ShowEditorInfo();

	COMPONENT_TYPE &GetType();
	const char *GetNameByType(COMPONENT_TYPE type);
	bool IsActive() const;

};





#endif //!__COMPONENT_H__
