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

	COMPONENT_TYPE            type;
	bool					active;		
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

	COMPONENT_TYPE &GetType()
	{
		return type;
	}

};





#endif //!__COMPONENT_H__
