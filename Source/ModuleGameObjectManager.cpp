#include "ModuleGameObjectManager.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCameraEditor.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
//#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "PerfTimer.h"

#include <stack>
#include <algorithm> 

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

	// First gameobject of the scene (UUID = 0)
	CreateRoot();

	fake_camera = App->gameobject_manager->CreateGameObject("Fake_camera", nullptr);
	fake_camera->AddComponent(COMPONENT_TYPE::TRANSFORM);
	ComponentCamera *c = (ComponentCamera*)fake_camera->AddComponent(COMPONENT_TYPE::CAMERA);
	c->SetComponent();

	// Initializing OcTree	
	oc_tree_boundaries.SetFromCenterAndSize({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 100.0f });
	oc_tree.SetBoundaries(oc_tree_boundaries);

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
		
	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		
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
	math::Frustum frustum = ((ComponentCamera*)fake_camera->GetComponentByType(COMPONENT_TYPE::CAMERA))->cam_frustum;
	FrustumCulling(frustum);
	draw_debug.DrawOcTree(oc_tree, frustum);

	for (uint i = 0; i < list_of_gos_to_draw.size(); ++i)
	{
		const GameObject *curr_go = list_of_gos_to_draw[i];
		if(curr_go->active)
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

	// Raycasts draw for testing!!
	for (uint i = 0; i < ray_casts.size(); ++i)
		draw_debug.DrawLineSegment(ray_casts[i]);

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

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name)
{
	GameObject *new_go = nullptr;
	new_go = new GameObject(name);
	list_of_gos.push_back(new_go);

	return new_go;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name, GameObject *parent)
{
	GameObject *new_go = CreateGameObject(name);
	
	if (parent != nullptr) // With not nullptr pointer parent, argument parent will be used.
	{
		new_go->parent = parent;
		new_go->parent_UUID = parent->UUID;
		parent->children.push_back(new_go);
	}
	else // With nullptr pointer parent, root will be the parent.
	{
		new_go->parent = root;
		new_go->parent_UUID = 0;
		root->children.push_back(new_go);
	}

	return new_go;
}

void ModuleGameObjectManager::CreateRoot()
{
	root = new GameObject("Root");
	root->UUID = 0;
	list_of_gos.push_back(root);
}

GameObject *ModuleGameObjectManager::GetGameObject(long unsigned int UUID_to_search, const std::vector<GameObject*> &list_to_check) const
{
	GameObject *go = nullptr;

	for (uint i = 0; i < list_to_check.size(); ++i)
	{
		if (list_to_check[i]->UUID == UUID_to_search)
			return list_to_check[i];
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

bool ModuleGameObjectManager::DeleteGameObject(GameObject *go_to_delete)
{
	bool ret = false;

	if (go_to_delete != nullptr)
	{
		// Removing child (go_to_delete) from the children parent's list.
		if (go_to_delete->parent != nullptr)
		{
			for (std::vector<GameObject*>::iterator it = go_to_delete->parent->children.begin(); it != go_to_delete->parent->children.end(); ++it)
			{
				if ((*it) == go_to_delete)
				{
					go_to_delete->parent->children.erase(it);
					break;
				}
			}
		}

		// Removing (go_to_delete) as parent from its children.
		for (uint i = 0; i < go_to_delete->children.size(); ++i)
			go_to_delete->children[i]->parent = nullptr;

		RELEASE(go_to_delete);
		ret = true;		
	}	

	return ret;
}

bool ModuleGameObjectManager::DeleteGameObject(long unsigned int id_to_delete)
{
	return DeleteGameObject(GetGameObject(id_to_delete, list_of_gos));
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
	oc_tree.CollectCandidates(list_of_gos_to_draw, frustum);
	
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
	std::vector<GameObject*> new_gos;

	for (int i = 0; i < module.GetArrayCount("Game Objects"); ++i)
	{
		JSONParser go = module.GetArray("Game Objects", i);
		GameObject *new_go = CreateGameObject(go.GetString("Name"));

		new_go->Load(go);
		new_gos.push_back(new_go);
	}

	// Now, linking gameobjects.
	for (uint i = 0; i < new_gos.size(); ++i)
	{
		GameObject *parent = GetGameObject(new_gos[i]->parent_UUID, new_gos);
		if (parent)
		{
			new_gos[i]->parent = parent;   // Assigning parent
			parent->children.push_back(new_gos[i]);  // Assigning as child
		}
		else
		{
			new_gos[i]->parent = root;   // Assigning root as parent
			root->children.push_back(new_gos[i]);  // Assigning a child on root
		}			
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

void ModuleGameObjectManager::RayCast(const math::LineSegment &ray_cast)
{
	PerfTimer timer;

	ray_casts.clear();

	GameObject *curr_go = nullptr;
	GameObject *selection_canditate = nullptr;
	float min_dist = 1.0f;

	std::vector<GameObject*> selection;
	oc_tree.CollectCandidates(selection, ray_cast);
	DEBUG("Tests with Octree %d", selection.size());
	DEBUG("Tests without Octree %d", list_of_gos.size());

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		
		math::AABB bbox; curr_go->GetAABB(bbox);
		math::Triangle tri;		

		Mesh mesh;
		
		if (curr_go->GetMesh(mesh) && ray_cast.Intersects(bbox))
		{
			//math::LineSegment ray_cast_transform = ray_cast;
			//ray_cast_transform.Transform(App->renderer3D->view_matrix);
			//ray_casts.push_back(ray_cast_transform);

			for (uint j = 0; j < mesh.num_indices; j = j + 3)
			{
				tri.a = mesh.vertices[mesh.indices[j]];
				tri.b = mesh.vertices[mesh.indices[j + 1]],
				tri.c = mesh.vertices[mesh.indices[j + 2]];								   
				tri.Transform(curr_go->transform->world_transform);				
				
				float hit_dist;
				math::vec hit_point;

				if (ray_cast.Intersects(tri, &hit_dist, &hit_point) && hit_dist < min_dist)
				{
					selection_canditate = curr_go;
					min_dist = hit_dist;
				}
			}
		}
	}

	App->editor->ChangeSelectedGameObject(selection_canditate);
	DEBUG("Ray Cast took %f", timer.ReadMs());
}

void ModuleGameObjectManager::UpdateOcTree()
{
	oc_tree.Clear();
	oc_tree.SetBoundaries(oc_tree_boundaries);

	GameObject *curr_go = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		if (curr_go->bstatic && curr_go->active)
			oc_tree.Insert(curr_go);
	}
}