#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"

#include "Assimp\include\types.h"

struct aiMaterial;

class ComponentMaterial : public Component
{

public:
	
	char tex_path[SHORT_STRING];	// Virtual system texture path
	unsigned int tex_buffer;

	aiColor3D color_diffuse;
	aiColor3D color_specular;
	aiColor3D color_ambient;
	aiColor3D color_emissive;
	aiColor3D color_transparent;
	float opacity;

	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	void SetComponent(aiMaterial *ai_material);

	void ShowEditorInfo();
	
};

#endif __COMPONENTMATERIAL_H__

