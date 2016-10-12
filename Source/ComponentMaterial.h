#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"

struct aiMaterial;

class ComponentMaterial : public Component
{
public:

	unsigned int tex_buffer;

	ComponentMaterial();

	bool Update();

	void SetComponent(aiMaterial *ai_material);

	void ShowEditorInfo();
	
};

#endif __COMPONENTMATERIAL_H__

