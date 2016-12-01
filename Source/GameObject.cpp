#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "Application.h"
#include "ModuleGameObjectManager.h"

GameObject::GameObject(const char* name)
{
	active = true;
	selected = false;
	to_delete = false;
	bstatic = true;

	sprintf_s(this->name, SHORT_STRING, name);

	App->gameobject_manager->GenerateUUID(this);		
}

GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); ++i)
		RELEASE(components[i]);

	components.clear();
	children.clear();
}

const Component* GameObject::AddComponent(COMPONENT_TYPE type)
{
	Component *comp = nullptr;

	switch (type)
	{
	case(COMPONENT_TYPE::TRANSFORM):
		comp = new ComponentTransform();
		if(transform == nullptr)
			transform = (ComponentTransform*)comp;
		break;
	case(COMPONENT_TYPE::MESH):
		comp = new ComponentMesh();
		break;
	case(COMPONENT_TYPE::MATERIAL):
		comp = new ComponentMaterial();
		break;
	case(COMPONENT_TYPE::CAMERA):
		comp = new ComponentCamera();
		break;
	}

	comp->game_object = this;
	components.push_back(comp);
	return comp;
}

const Component *GameObject::GetComponentByType(COMPONENT_TYPE type) const
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			return components[i];
	}

	return nullptr;
}

void GameObject::ChangeActiveState(bool *state, GameObject *go)
{
	if (go == nullptr)
		go = this;

	go->active = *state;

	for (uint i = 0; i < go->children.size(); ++i)
		ChangeActiveState(state, go->children[i]);

}

void GameObject::Enable() { active = true; }
void GameObject::Disable() { active = false; }
bool GameObject::IsActive() const { return active; }
const char *GameObject::GetName() const { return name; }

bool GameObject::GetAABB(math::AABB &aabb) const
{
	bool ret = false;
	aabb.SetNegativeInfinity();
	const ComponentMesh *mesh = (ComponentMesh*)GetComponentByType(COMPONENT_TYPE::MESH);
	if (mesh)
	{
		ret = true;
		aabb = mesh->bounding_box;
	}		

	return ret;
}

bool GameObject::GetFrustum(math::Frustum &frustum) const
{
	bool ret = false;
	const ComponentCamera *camera = (ComponentCamera*)GetComponentByType(COMPONENT_TYPE::CAMERA);
	if (camera)
	{
		ret = true;
		frustum = camera->cam_frustum;
	}

	return ret;
}

bool GameObject::GetMesh(const Mesh *mesh) const
{
	if (HasMesh())
		mesh = ((ComponentMesh*)GetComponentByType(COMPONENT_TYPE::MESH))->resource->mesh_data;	

	return mesh != nullptr ? true : false;
}

 bool GameObject::HasMesh() const
 {
	 return (GetComponentByType(COMPONENT_TYPE::MESH) != nullptr  ? true : false);
 }

 bool GameObject::Save(JSONParser &game_objects)
 {
	 if (UUID != 0) // Root will not be saved!
	 {
		 JSONParser go;

		 go.AddString("Name", name);

		 go.AddBoolean("Active", active);
		 go.AddBoolean("Selected", selected);
		 go.AddBoolean("ToDelete", to_delete);
		 go.AddBoolean("Static", bstatic);

		 go.AddUUID("UUID", UUID);
		 go.AddUUID("Parent UUID", parent->UUID);

		 go.AddArray("Components");

		 for (uint i = 0; i < components.size(); ++i)
			 components[i]->Save(go);

		 game_objects.AddArray(go);
	 }	

	 return true;
 }

 bool GameObject::Load(JSONParser &go)
 {
	 active = go.GetBoolean("Active");
	 selected = go.GetBoolean("Selected");
	 to_delete = go.GetBoolean("ToDelete");
	 bstatic = go.GetBoolean("Static");

	 UUID = go.GetUUID("UUID");
	 parent_UUID = go.GetUUID("Parent UUID");
	 
	 for (int i = 0; i < go.GetArrayCount("Components"); ++i)
	 {
		 JSONParser component = go.GetArray("Components", i);
		 Component *comp = nullptr;

		 switch ((COMPONENT_TYPE)component.GetInt("Type"))
		 {
			 case COMPONENT_TYPE::TRANSFORM:
			 {
				 comp = (Component*)AddComponent(COMPONENT_TYPE::TRANSFORM);
				 comp->Load(component);
				 break;
			 }			 
			 case COMPONENT_TYPE::MESH:
			 {
				 comp = (Component*)AddComponent(COMPONENT_TYPE::MESH);
				 comp->Load(component);
				 break;
			 }
			 case COMPONENT_TYPE::MATERIAL:
			 {
				 comp = (Component*)AddComponent(COMPONENT_TYPE::MATERIAL);
				 comp->Load(component);
				 break;
			 }
			 case COMPONENT_TYPE::CAMERA:
			 {
				 comp = (Component*)AddComponent(COMPONENT_TYPE::CAMERA);
				 comp->Load(component);
				 break;
			 }
		 }
	 }

	 return true;
 }
