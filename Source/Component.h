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
	bool					active;		
	unsigned int				id;

public:

	GameObject *game_object = nullptr;

	Component()
	{
		type = COMPONENT_TYPE::UNKNOWN;
		active = true;
	}

	virtual bool Update() {  return true;	}
	virtual void ShowEditorInfo() = 0;

	COMPONENT_TYPE &GetType()
	{
		return type;
	}

	const char *GetNameByType(COMPONENT_TYPE type)
	{
		const char *name;
		switch (type)
		{
		case(COMPONENT_TYPE::TRANSFORM):
			name = "Transform";
			break;
		case(COMPONENT_TYPE::MESH):
			name = "Mesh";
			break;
		default:
			name = "Unknown";
		}

		return name;
	}

};





#endif //!__COMPONENT_H__
