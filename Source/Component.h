#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum COMPONENT_TYPE
{
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

	bool					active;
	COMPONENT_TYPE            type;
	unsigned int				id;

public:

	Component()
	{
		active = true;
	}

	virtual bool Update()
	{
		return true;
	}

	COMPONENT_TYPE GetType()
	{
		return type;
	}

};





#endif //!__COMPONENT_H__
