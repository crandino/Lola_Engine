#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "Color.h"

#include "ResourceTexture.h"

class ComponentMaterial : public Component
{

public:
	
	ResourceTexture *resource;
	
	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	void AddResource(Resource *res);

	void ShowEditorInfo();
	
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTMATERIAL_H__

