#include "ModuleGameObjectManager.h"

#include "Application.h"
//#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "ModuleCameraEditor.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
//#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include <stack>

ModuleGameObjectManager::ModuleGameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "GameObject Manager");
}

// Destructor
ModuleGameObjectManager::~ModuleGameObjectManager()
{ }

// Called before render is available
bool ModuleGameObjectManager::Awake(JSONParser &config)
{
	bool ret = true;

	// First gameobject of the scene (id = 0)
	root = CreateGameObject("Root", nullptr);

	fake_camera = App->gameobject_manager->CreateGameObject("Fake_camera", nullptr);
	fake_camera->AddComponent(COMPONENT_TYPE::TRANSFORM);
	ComponentCamera *c = (ComponentCamera*)fake_camera->AddComponent(COMPONENT_TYPE::CAMERA);
	c->SetComponent();

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	// Looking GameObject to delete...
	std::vector<GameObject*>::iterator it;
	
	for(it = list_of_gos.begin(); it != list_of_gos.end(); ++it)
	{
		if ((*it) != nullptr && (*it)->to_delete)
		{
			DeleteGameObject((*it));
			(*it) = nullptr;
		}
	}

	// Deleting the erased GameObjects form the list of GOs
	for (it = list_of_gos.begin(); it != list_of_gos.end();)
	{
		if ((*it) == nullptr)
			it = list_of_gos.erase(it);
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	GameObject *curr_go = nullptr;
	oc_tree.Clear();
	math::AABB boundaries = math::AABB({ -10.0f, -10.0f, -10.0f }, { 10.0f, 10.0f, 10.0f });
	oc_tree.SetBoundaries(boundaries);
	
	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		oc_tree.Insert(curr_go);

		if (curr_go->IsActive())
		{
			for (uint i = 0; i < curr_go->components.size(); ++i)
			{
				curr_go->components[i]->Update();
				switch (curr_go->components[i]->GetType())
				{
					case(COMPONENT_TYPE::CAMERA):
						draw_debug.DrawFrustum(((ComponentCamera*)curr_go->components[i])->cam_frustum);
						break;
				}
			}					

			curr_go->transform_applied = false;
		}
	}

	// Frustum culling
	const GameObject *camera = App->camera->GetEditorCamera();
	math::Frustum frustum = ((ComponentCamera*)camera->GetComponentByType(COMPONENT_TYPE::CAMERA))->cam_frustum;
	FrustumCulling(frustum);
	draw_debug.DrawOcTree(oc_tree);

	for (uint i = 0; i < list_of_gos_to_draw.size(); ++i)
	{
		const GameObject *curr_go = list_of_gos_to_draw[i];
		App->renderer3D->ShowGameObject(curr_go);

		for (uint i = 0; i < curr_go->components.size(); ++i)
		{
			switch (curr_go->components[i]->GetType())
			{
			case(COMPONENT_TYPE::MESH):
				draw_debug.DrawAABB(((ComponentMesh*)curr_go->components[i])->bounding_box);
				break;
			}
		}			
	}		

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::PostUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_6))
	{
		for (uint i = 0; i < list_of_gos.size(); ++i)
		{
			const GameObject *curr_go = list_of_gos[i];

			for (uint i = 0; i < curr_go->components.size(); ++i)
			{
				switch (curr_go->components[i]->GetType())
				{
				case(COMPONENT_TYPE::MESH):
					((ComponentMesh*)curr_go->components[i])->Load();
					break;
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGameObjectManager::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < list_of_gos.size(); ++i)
		RELEASE(list_of_gos[i]);

	list_of_gos.clear();
	list_of_gos_to_draw.clear();
	
	return ret;
}

const GameObject *ModuleGameObjectManager::GetRoot() const
{
	return root;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name, GameObject *parent)
{
	GameObject *new_go = nullptr;

	// With nullptr pointer parent, root will be the parent if it's not the first gameobject on the game.
	if (parent == nullptr && id_to_assign != 0)
		parent = root;				
	
	new_go = new GameObject(id_to_assign++, name, parent);
	list_of_gos.push_back(new_go);

	return new_go;
}

GameObject *ModuleGameObjectManager::GetGameObject(uint id_to_search) const
{
	GameObject *go = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		if (list_of_gos[i]->id == id_to_search)
			return list_of_gos[i];
	}

	return nullptr;
}

GameObject *ModuleGameObjectManager::FindParent(const GameObject* go) const
{
	std::stack<GameObject*> go_stack;
	go_stack.push(root);

	GameObject *curr_go = nullptr;

	while (!go_stack.empty())
	{
		curr_go = go_stack.top();
		go_stack.pop();

		if (curr_go != nullptr)
		{
			for (uint i = 0; i < curr_go->children.size(); ++i)
			{
				if (curr_go->children[i] == go)
					return curr_go;

				go_stack.push(curr_go->children[i]);
			}
		}		
	}

	return nullptr;
}

void ModuleGameObjectManager::MarkChildToDelete(GameObject *go)
{
	std::stack<GameObject*> go_stack;
	go_stack.push(go);

	GameObject *curr_go = nullptr;

	while (!go_stack.empty())
	{
		curr_go = go_stack.top();
		go_stack.pop();

		curr_go->to_delete = true;

		for (uint i = 0; i < curr_go->children.size(); ++i)
			go_stack.push(curr_go->children[i]);
	}
}

bool ModuleGameObjectManager::DeleteGameObject(const GameObject *go_to_delete)
{
	bool ret = false;

	if (go_to_delete != nullptr)
	{
		GameObject *parent = FindParent(go_to_delete);

		if (parent != nullptr)
		{
			for(std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it)
			{
				if ((*it) == go_to_delete)
				{
					parent->children.erase(it);
					break;
				}				
			}
		}

		RELEASE(go_to_delete);
		ret = true;		
	}	

	return ret;
}

bool ModuleGameObjectManager::DeleteGameObject(unsigned int id_to_delete)
{
	return DeleteGameObject(GetGameObject(id_to_delete));
}

void ModuleGameObjectManager::SetEditorCamera(const ComponentCamera *comp_cam)
{
	ComponentCamera *cam = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{				
		cam = (ComponentCamera*)list_of_gos[i]->GetComponentByType(COMPONENT_TYPE::CAMERA);
		
		if (cam && cam != comp_cam)
			cam->editor_camera = false;
	}

	App->camera->SetAsEditorCamera(comp_cam->game_object);
}

void ModuleGameObjectManager::CreateCamera()
{
	GameObject *go = CreateGameObject("Camera");
	go->AddComponent(COMPONENT_TYPE::TRANSFORM);
	go->AddComponent(COMPONENT_TYPE::CAMERA);
}

int ModuleGameObjectManager::FrustumCulling(const math::Frustum &frustum)
{
	list_of_gos_to_draw.clear();

	const uint num_planes = 6;
	const uint num_corners = 8;

	const GameObject *curr_go;

	math::Plane planes[num_planes];
	frustum.GetPlanes(planes);

	math::vec corners[num_corners];	

	std::vector<GameObject*> nodes;
	oc_tree.CollectCandidates(nodes, frustum);

	// For every gameobject
	for (uint i = 1; i < nodes.size(); ++i)
	{
		bool next_go = false;
		curr_go = nodes[i];

		if (!curr_go->HasMesh())
			continue;

		math::AABB bounding_box;
		curr_go->GetAABB(bounding_box);
		bounding_box.GetCornerPoints(corners);

		bool go_accepted = true;

		for (uint j = 0; j < num_planes; ++j)
		{		
			if (go_accepted)
			{
				uint corners_outside_frustum = 0;
				for (uint k = 0; k < num_corners; ++k)
				{
					if (planes[j].IsOnPositiveSide(corners[k]))
						++corners_outside_frustum;
				}

				if (corners_outside_frustum == num_corners)
				{
					go_accepted = false;
					break;
				}
			}
		}

		if (go_accepted) list_of_gos_to_draw.push_back(curr_go);
	}
	
	return list_of_gos_to_draw.size();
}

bool ModuleGameObjectManager::Save(JSONParser &module)
{
	module.AddArray("Game Objects");
	for (uint i = 0; i < list_of_gos.size(); ++i)
		list_of_gos[i]->Save(module);

	return true;
}

bool ModuleGameObjectManager::Load(JSONParser &module)
{
	for (uint i = 0; i < module.GetArrayCount("Game Objects"); ++i)
	{
		JSONParser go = module.GetArray("Game Objects", i);
		go.GetBoolean("Active");
		go.GetBoolean("Selected");
	}
		
	return true;
}

void ModuleGameObjectManager::GenerateUUID(GameObject *go)
{
	go->UUID = UUID_generator.Int();
}

void ModuleGameObjectManager::GenerateUUID(Component *comp)
{
	comp->UUID = UUID_generator.Int();
}

void ModuleGameObjectManager::RayCast(const math::LineSegment &ray_cast) const
{
	GameObject *curr_go = nullptr;
	GameObject *selection_canditate = nullptr;
	float min_dist = 1.0f;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		Mesh* mesh = curr_go->GetMesh();
		
		if (mesh)
		{
			for (uint j = 0; j < mesh->num_indices; j = j + 3)
			{
				math::Triangle tri(mesh->vertices[mesh->indices[j]],
								   mesh->vertices[mesh->indices[j + 1]],
								   mesh->vertices[mesh->indices[j + 2]]);
				tri.Transform(curr_go->transform->world_transform);
				
				float hit_dist;
				math::vec hit_point;

				if (ray_cast.Intersects(tri, &hit_dist, &hit_point) && hit_dist < min_dist)
				{
					selection_canditate = curr_go;
					min_dist = hit_dist;
					DEBUG("%s %s %s %f", "Hit on", curr_go->GetName(), "at", hit_dist);
				}
			}
		}
	}

	App->editor->ChangeSelectedGameObject(selection_canditate);
}