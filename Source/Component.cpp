#include "Component.h"

#include "MathGeoLib\Algorithm\Random\LCG.h"
#include "JSONParser.h"

Component::Component()
{
	type = COMPONENT_TYPE::UNKNOWN;
	active = true;

	math::LCG random;
	UUID = random.Int();
}

Component::~Component()
{ }

bool Component::Update()
{ 
	return true;
}

void Component::ShowEditorInfo()
{ }

COMPONENT_TYPE &Component::GetType()
{
	return type;
}

const char *Component::GetNameByType(COMPONENT_TYPE type)
{
	const char *name;
	switch (type)
	{
	case(COMPONENT_TYPE::TRANSFORM):
		name = "Transform";
		break;
	case(COMPONENT_TYPE::MESH):
		name = "Mesh";
		break;
	case(COMPONENT_TYPE::MATERIAL):
		name = "Material";
		break;
	case(COMPONENT_TYPE::CAMERA):
		name = "Camera";
		break;
	default:
		name = "Unknown";
	}

	return name;
}

bool Component::IsActive() const
{
	return active;
}

bool Component::Save(JSONParser &go)
{
	return true;
}