#include "ComponentImageUI.h"

#include "ResourceTexture.h"
#include "TextureImporter.h"

#include "imgui\imgui.h"

ComponentImageUI::ComponentImageUI() : Component()
{
	type = COMPONENT_TYPE::COMP_UI_IMAGE;
	name = "UI Image";

	resource = nullptr;
}

ComponentImageUI::~ComponentImageUI()
{
	if (resource->texture_data != nullptr)
	{
		resource->DecrReferences();

		if (resource->GetNumReferences() == 0)
		{
			resource->UnloadFromMemory();
			RELEASE_ARRAY(resource->texture_data);
			resource->texture_data = nullptr;
		}
	}
}

void ComponentImageUI::InitTexture(const char *tex_name)
{
	if (!resource->LoadedInMemory())
		TextureImporter::Load(resource->imported_file, resource);

	resource->LoadToMemory();
}

void ComponentImageUI::AddResource(Resource *res)
{
	res->IncrReferences();
	resource = (ResourceTexture*)res;
}

void ComponentImageUI::ShowEditorInfo()
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

bool ComponentImageUI::Save(JSONParser &go)
{
	//JSONParser component;

	//component.AddInt("Type", type);
	//component.AddBoolean("Active", active);
	//component.AddInt("Opacity", resource->opacity);

	//// Colors and opacity is loaded into Resource, so if the resource exists, it will have these information
	//component.AddPoints("Color diffuse", resource->color_diffuse, 3);
	//component.AddPoints("Color specular", resource->color_specular, 3);
	//component.AddPoints("Color ambient", resource->color_ambient, 3);
	//component.AddPoints("Color emissive", resource->color_emissive, 3);
	//component.AddPoints("Color transparent", resource->color_transparent, 3);

	//component.AddUUID("Resource ID", resource->id);

	//go.AddArray(component);

	return true;
}

bool ComponentImageUI::Load(JSONParser &comp)
{
	/*AddResource(App->resource_manager->Get(comp.GetUUID("Resource ID")));

	active = comp.GetBoolean("Active");
	resource->opacity = comp.GetInt("Opacity");

	comp.GetPoints("Color diffuse", resource->color_diffuse, 3);
	comp.GetPoints("Color specular", resource->color_specular, 3);
	comp.GetPoints("Color ambient", resource->color_ambient, 3);
	comp.GetPoints("Color emissive", resource->color_emissive, 3);
	comp.GetPoints("Color transparent", resource->color_transparent, 3);*/

	return true;
}