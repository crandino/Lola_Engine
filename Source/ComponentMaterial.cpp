#include "ComponentMaterial.h"

#include "Application.h"

#include "Assimp\include\material.h"
#include "imgui\imgui.h"

ComponentMaterial::ComponentMaterial() : Component()
{
	/*color_diffuse.Set(0.0f, 0.0f, 0.0f);
	color_specular.Set(0.0f, 0.0f, 0.0f);
	color_ambient.Set(0.0f, 0.0f, 0.0f);
	color_emissive.Set(0.0f, 0.0f, 0.0f);
	color_transparent.Set(0.0f, 0.0f, 0.0f);

	opacity = 1.0f;*/

	type = COMPONENT_TYPE::MATERIAL;
	name = GetNameByType(type);

	resource = nullptr;
}

ComponentMaterial::~ComponentMaterial()
{ 
	//App->tex_loader->DeleteBuffer(tex_buffer);
}

bool ComponentMaterial::Update()
{
	return true;
}

void ComponentMaterial::SetComponent(aiMaterial *ai_material)
{
	unsigned int numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);

	//if (numTextures != 0)
	//{
	//	// Loading texture to tex_buffer
	//	aiString path;
	//	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	//	App->tex_loader->LoadTexture(path.C_Str(), tex_buffer, this);
	//}
	//else
	//	tex_buffer = 0;

	/*aiColor3D color;
	
	ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	color_diffuse.Set(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	color_specular.Set(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	color_ambient.Set(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	color_emissive.Set(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
	color_transparent.Set(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_OPACITY, opacity);*/

}

void ComponentMaterial::AddResource(const Resource *res)
{
	resource = (ResourceTexture*)res;
}

void ComponentMaterial::ShowEditorInfo()
{
	ImGui::TextColored(ImVec4(1.0f, 0.5, 0.0f, 1.0f), "Component: "); ImGui::SameLine();
	ImGui::Text(name);

	ImGui::Checkbox("Active##Mat", &active);
	if (resource->tex_buffer != 0)
	{
		// Showing texture image on a 200x200 frame
		ImGui::Image((void*)resource->tex_buffer, ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
		ImGui::Text("%s%s", "Path: ", resource->tex_path);
	}	

	ImGui::Separator();
}

bool ComponentMaterial::Save(JSONParser &go)
{
	JSONParser component;

	component.AddInt("Type", type);
	component.AddInt("Opacity", resource->opacity);

	// Colors and opacity is loaded into Resource, so if the resource exists, it will have these information
	component.AddPoints("Color diffuse", resource->color_diffuse, 3);
	component.AddPoints("Color specular", resource->color_specular, 3);
	component.AddPoints("Color ambient", resource->color_ambient, 3);
	component.AddPoints("Color emissive", resource->color_emissive, 3);
	component.AddPoints("Color transparent", resource->color_transparent, 3);

	component.AddUUID("Resource ID", resource->id);

	go.AddArray(component);

	return true;
}

bool ComponentMaterial::Load(JSONParser &comp)
{
	resource->opacity = comp.GetInt("Opacity");

	comp.GetPoints("Color diffuse", resource->color_diffuse, 3);
	comp.GetPoints("Color specular", resource->color_specular, 3);
	comp.GetPoints("Color ambient", resource->color_ambient, 3);
	comp.GetPoints("Color emissive", resource->color_emissive, 3);
	comp.GetPoints("Color transparent", resource->color_transparent, 3);
	
	AddResource(App->resource_manager->Get(comp.GetUUID("Resource ID")));

	return true;
}


