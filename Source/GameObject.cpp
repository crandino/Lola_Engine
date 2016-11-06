#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "Application.h"
#include "ModuleGameObjectManager.h"

GameObject::GameObject(uint id, const char* name, GameObject *parent)
{
	active = true;
	selected = false;
	to_delete = false;

	sprintf_s(this->name, SHORT_STRING, name);

	this->id = id;
	App->gameobject_manager->GenerateUUID(this);

	if (parent != nullptr)
		parent->children.push_back(this);
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

 bool GameObject::HasMesh() const
 {
	 return (GetComponentByType(COMPONENT_TYPE::MESH) != nullptr  ? true : false);
 }

 bool GameObject::Save(JSONParser &game_objects)
 {
	 JSONParser go;

	 go.AddBoolean("Active", active);
	 go.AddBoolean("Selected", selected);	

	 go.AddArray("Components");

	 for (uint i = 0; i < components.size(); ++i)
	 {
		 components[i]->Save(go);
	 }

	 game_objects.AddArray(go);
	
	 return true;
 }
