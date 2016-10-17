#include "ModuleGameObjectManager.h"

#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include <stack>

#pragma comment (lib, "Source/Assimp/libx86/assimp.lib")

ModuleGameObjectManager::ModuleGameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("GameObject Manager");
}

// Destructor
ModuleGameObjectManager::~ModuleGameObjectManager()
{ }

// Called before render is available
bool ModuleGameObjectManager::Init()
{
	bool ret = true;

	// First gameobject of the scene (id = 0)
	root = CreateGameObject("Root", nullptr);

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		ImportModel("Models/primitives_with_parent.fbx");  // primitives_with_parent2.fbx Street environment_V01.fbx

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
			if(curr_go->GetComponentByType(COMPONENT_TYPE::MESH))
				App->renderer3D->ShowGameObject(curr_go);
		}
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGameObjectManager::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < list_of_gos.size(); ++i)
		RELEASE(list_of_gos[i]);

	list_of_gos.clear();

	// detach log stream
	aiDetachAllLogStreams();
	
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

bool ModuleGameObjectManager::DeleteGameObject(const GameObject *go_to_delete)
{
	bool ret = false;

	if (go_to_delete != nullptr)
	{
		std::vector<GameObject*>::iterator it_gos = list_of_gos.begin();
		
		for (;it_gos != list_of_gos.end();)
		{
			if ((*it_gos) == go_to_delete)
			{	
				for (uint i = 0; i < (*it_gos)->children.size(); ++i)
					DeleteChildrenGameObject((*it_gos)->children[i]);

				RemoveChildFromChildren(go_to_delete);
				it_gos = list_of_gos.erase(it_gos);
				RELEASE(go_to_delete);	
				ret = true;
			}
			else
				++it_gos;
		}
	}	

	return ret;
}

bool ModuleGameObjectManager::DeleteChildrenGameObject(const GameObject *go_to_delete)
{
	bool ret = false;

	if (go_to_delete != nullptr)
	{
		std::vector<GameObject*>::iterator it_gos = list_of_gos.begin();

		for (;it_gos != list_of_gos.end();)
		{
			if ((*it_gos) == go_to_delete)
			{
				for (uint i = 0; i < (*it_gos)->children.size(); ++i)
					DeleteChildrenGameObject((*it_gos)->children[i]);

				it_gos = list_of_gos.erase(it_gos);
				RELEASE(go_to_delete);
				ret = true;
			}
			else
				++it_gos;
		}
	}

	return ret;
}

bool ModuleGameObjectManager::DeleteGameObject(unsigned int id_to_delete)
{
	return DeleteGameObject(GetGameObject(id_to_delete));
}

bool ModuleGameObjectManager::RemoveChildFromChildren(const GameObject *go_child_to_remove)
{
	if (go_child_to_remove)
	{
		std::vector<GameObject*>::iterator it_gos = list_of_gos.begin();
		std::vector<GameObject*>::iterator it_child;

		for (; it_gos != list_of_gos.end(); ++it_gos)
		{
			for(it_child = (*it_gos)->children.begin(); it_child != (*it_gos)->children.end(); ++it_child)
			{
				if ((*it_child) == go_child_to_remove)
				{
					(*it_gos)->children.erase(it_child);
					return true;
				}				
			}
		}
	}
	return false;
	
}

/* ImportModel seperates each FBX sections into different GameObject components (transform, mesh, material,...).
A GameObject is created for every individual mesh available on the FBX */
void ModuleGameObjectManager::ImportModel(const char *file_name, bool use_fs)
{
	const aiScene* scene;

	if (use_fs)
		scene = aiImportFileEx(file_name, aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());
	else
		scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr)
	{
		if (scene->HasMeshes())
		{
			std::stack<aiNode*> nodes_stack;
			std::stack<GameObject*> go_stack;

			nodes_stack.push(scene->mRootNode);
			go_stack.push(root);

			aiNode *curr_node;
			GameObject *parent;

			while (!nodes_stack.empty())
			{
				curr_node = nodes_stack.top();
				parent = go_stack.top();

				int num_children = curr_node->mNumChildren;

				if (num_children > 0)
				{
					nodes_stack.pop();  // Node checked is eliminated.
					go_stack.pop();

					for (int i = 0; i < num_children; ++i)
					{
						aiNode *node_to_add = curr_node->mChildren[i];												

						nodes_stack.push(node_to_add);
						char *go_name = node_to_add->mName.data;

						// I don't like! This part eliminates the strange names on Ricard's FBX,
						// but it is exclusive for his FBX, so a better and generic option has to be implemented.						
						for (uint i = 0; i < node_to_add->mName.length; ++i)
						{
							if (go_name[i] == '$')
							{
								go_name[i-1] = '\0';
								break;
							}								
						}							

						GameObject *new_go = CreateGameObject(go_name, parent);	
						go_stack.push(new_go);						

						// --- TRANSFORM ---						
						ComponentTransform *comp_trans = new ComponentTransform();
						comp_trans->SetComponent(node_to_add);
						new_go->AddComponent(comp_trans);
									
						// --- MESH ---						
						if (node_to_add->mNumMeshes != 0)
						{
							ComponentMesh *comp_mesh = new ComponentMesh();
							aiMesh *ai_mesh = scene->mMeshes[*node_to_add->mMeshes];
							comp_mesh->SetComponent(ai_mesh);							

							App->renderer3D->LoadMeshBuffer(comp_mesh);
							new_go->AddComponent(comp_mesh);

							// --- MATERIAL ---
							ComponentMaterial *comp_mat = new ComponentMaterial();
							aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
							comp_mat->SetComponent(ai_material);
							new_go->AddComponent(comp_mat);
						}					
					}
				}
				else
				{
					go_stack.pop();
					nodes_stack.pop();					
				}
			}
		}
		aiReleaseImport(scene);
	}
	else
	{
		DEBUG("Error loading scene %s: %s", file_name, aiGetErrorString());
	}	
}