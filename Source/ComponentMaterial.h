#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"

#include "ModuleTextureLoader.h"

#include "Assimp\include\Material.h"

class ComponentMaterial : public Component
{

public:

	unsigned int tex_buffer;

	ComponentMaterial()
	{
		type = COMPONENT_TYPE::MATERIAL;
		name = GetNameByType(type);
	}

	bool Update()
	{
		return true;
	}

	void ShowEditorInfo()
	{
		//ImGui::Text(name);
	}

	void SetComponent(aiMaterial *ai_material)
	{
		uint numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTextures != 0)
		{
			aiString path;
			ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			App->tex_loader->LoadTexture(path.C_Str(), tex_buffer);
		}
		else
			tex_buffer = 0;
		
		
	}
};

#endif __COMPONENTMATERIAL_H__

