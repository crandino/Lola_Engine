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

	// First gameobject of the scene
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
		ImportModel("Models/primitives_with_parent2.fbx");  // primitives_with_parent2.fbx Street environment_V01.fbx

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	GameObject *curr_go = nullptr;

	for (uint i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		if (curr_go->isActive())
		{
			if(curr_go->GetComponentByType(COMPONENT_TYPE::MESH))
				App->renderer3D->DrawMesh(curr_go);

			/*for (uint j = 0; j < curr_go->components.size(); ++j)
			{
				curr_go->components[j]->Update();
			}*/
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

	// detach log stream
	aiDetachAllLogStreams();
	
	return ret;
}

GameObject *ModuleGameObjectManager::CreateGameObject(const char *name, GameObject *parent)
{
	GameObject *new_go = nullptr;

	// With nullptr pointer parent, root will be the parent if it's not the first gameobject on the game.
	if (parent == nullptr && id != 0)
		parent = root;				
	
	new_go = new GameObject(id++, name, parent);
	list_of_gos.push_back(new_go);

	return new_go;
}

GameObject *ModuleGameObjectManager::GetGameObject(uint id_to_search)
{
	std::stack<GameObject*> go_stack;
	go_stack.push(root);
	
	while (!go_stack.empty())
	{
		GameObject *top_go = go_stack.top();
		int num_children = top_go->children.size();

		if (num_children > 0)
		{
			for (int i = num_children; i > 0; --i)
				go_stack.push(top_go->children[i]);
		}
		else
		{
			if (top_go->id == id_to_search)
				return top_go;
			else
				go_stack.pop();
		}		
	}

	return nullptr;
}

void ModuleGameObjectManager::ImportModel(const char *file_name)
{
	const aiScene* scene = aiImportFileEx(file_name, aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());

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