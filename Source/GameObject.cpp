#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(uint id, const char* name, GameObject *parent)
{
	active = true;
	selected = false;
	this->id = id;
	sprintf_s(this->name, SHORT_STRING, name);

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

void GameObject::AddComponent(Component *comp)
{
	comp->game_object = this;
	components.push_back(comp);

	if (comp->GetType() == COMPONENT_TYPE::TRANSFORM && transform == nullptr)
		transform = (ComponentTransform*)comp;
}

const Component *GameObject::GetComponentByType(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			return components[i];
	}

	return nullptr;
}

void GameObject::Enable() { active = true; }
void GameObject::Disable() { active = false; }
bool GameObject::IsActive() const { return active; }
const char *GameObject::GetName() const { return name; }
