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
private:

	bool			active;
	COMPONENT_TYPE    type;
	uint				id;

public:

	COMPONENT_TYPE GetType();

};





#endif //!__COMPONENT_H__
