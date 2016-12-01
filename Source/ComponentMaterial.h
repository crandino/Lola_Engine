#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "Color.h"

#include "ResourceTexture.h"

#include "Assimp\include\types.h"

struct aiMaterial;

class ComponentMaterial : public Component
{

public:
	
	ResourceTexture *resource;
	
	//math::float3  color_diffuse;
	//math::float3  color_specular;
	//math::float3  color_ambient;
	//math::float3  color_emissive;
	//math::float3  color_transparent;
	//float opacity;

	ComponentMaterial();
	~ComponentMaterial();

	bool Update();

	void SetComponent(aiMaterial *ai_material);
	void AddResource(const Resource *res);

	void ShowEditorInfo();
	
	bool Save(JSONParser &go);
	bool Load(JSONParser &comp);
};

#endif __COMPONENTMATERIAL_H__

