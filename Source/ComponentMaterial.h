#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"

struct aiMaterial;

class ComponentMaterial : public Component
{

public:
	
	char tex_path[SHORT_STRING];
	char *texture;
	unsigned int tex_buffer;

	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	void SetComponent(aiMaterial *ai_material);

	void ShowEditorInfo();
	
};

#endif __COMPONENTMATERIAL_H__

