#include "ComponentMaterial.h"

#include "Application.h"
#include "ModuleTextureLoader.h"

#include "Assimp\include\material.h"

ComponentMaterial::ComponentMaterial()
{
	type = COMPONENT_TYPE::MATERIAL;
	name = GetNameByType(type);
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::SetComponent(aiMaterial *ai_material)
{
	unsigned int numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
	if (numTextures != 0)
	{
		aiString path;
		ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		App->tex_loader->LoadTexture(path.C_Str(), tex_buffer);
	}
	else
		tex_buffer = 0;
}

void ComponentMaterial::ShowEditorInfo()
{
	//ImGui::Text(name);
}



