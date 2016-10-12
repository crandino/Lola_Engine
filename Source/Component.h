#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

enum COMPONENT_TYPE
{
	UNKNOWN,
	TRANSFORM,
	MATERIAL,
	CAMERA,
	MESH,
	AUDIO,
	ANIMATION	
};

class Component
{

protected:

	const char				 *name;
	COMPONENT_TYPE            type;
	
	unsigned int				id;

public:

	bool					active;

	GameObject *game_object = nullptr;

	Component();
	virtual ~Component();

	virtual bool Update();
	virtual void ShowEditorInfo() = 0;

	COMPONENT_TYPE &GetType();
	const char *GetNameByType(COMPONENT_TYPE type);

};





#endif //!__COMPONENT_H__
