#include "ComponentMaterial.h"

#include "Application.h"
#include "ModuleTextureLoader.h"

#include "Assimp\include\material.h"
#include "imgui\imgui.h"

ComponentMaterial::ComponentMaterial() : Component()
{
	type = COMPONENT_TYPE::MATERIAL;
	name = GetNameByType(type);
}

ComponentMaterial::~ComponentMaterial()
{ }

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

		App->tex_loader->LoadTexture(path.C_Str(), tex_buffer, this);
	}
	else
		tex_buffer = 0;
}

void ComponentMaterial::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mat", &active);

	ImGui::Image((void*)tex_buffer, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1,1,1,1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
	ImGui::Text("%s%s", "Path: ", tex_path);

	ImGui::Separator();
}



