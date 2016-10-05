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

	root = CreateGameObject("Root", nullptr);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGameObjectManager::PreUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		ImportModel("Models/primitives.fbx");

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGameObjectManager::Update(float dt)
{
	GameObject *curr_go = nullptr;

	for (int i = 0; i < list_of_gos.size(); ++i)
	{
		curr_go = list_of_gos[i];
		if (curr_go->isActive())
		{
			for (int j = 0; j < curr_go->components.size(); ++j)
			{
				curr_go->components[j]->Update();
			}
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
		GameObject *new_go = CreateGameObject(file_name, nullptr);

		if (scene->HasMeshes())
		{
			// For each mesh...
			for (uint i = 0; i < scene->mNumMeshes; ++i)
			{
				ComponentMesh *comp_mesh = new ComponentMesh();
				aiMesh *ai_mesh = scene->mMeshes[i];
				DEBUG("Creating new mesh %s", ai_mesh->mName);

				// Copying vertices...
				comp_mesh->num_vertices = ai_mesh->mNumVertices;
				comp_mesh->vertices = new math::float3[comp_mesh->num_vertices];
				memcpy(comp_mesh->vertices, ai_mesh->mVertices, sizeof(math::float3) * comp_mesh->num_vertices);
				DEBUG("  -> %d vertices", comp_mesh->num_vertices);

				// Copying normals...
				comp_mesh->num_normals = ai_mesh->mNumVertices;
				comp_mesh->normals = new math::float3[comp_mesh->num_normals];
				memcpy(comp_mesh->normals, ai_mesh->mNormals, sizeof(math::float3) * comp_mesh->num_normals);
				DEBUG("  -> %d normals", comp_mesh->num_normals);

				// Copying texture coordinates...
				uint UV_index = 0;
				if (ai_mesh->HasTextureCoords(UV_index))
				{
					comp_mesh->num_tex_coord = ai_mesh->mNumVertices;
					comp_mesh->tex_coord = new math::float2[comp_mesh->num_tex_coord];
					for (int l = 0; l < comp_mesh->num_tex_coord; ++l)
					{
						comp_mesh->tex_coord[l].x = ai_mesh->mTextureCoords[UV_index][l].x;
						comp_mesh->tex_coord[l].y = ai_mesh->mTextureCoords[UV_index][l].y;
					}
					DEBUG("  -> %d texture coordinates", comp_mesh->num_tex_coord);				
				}

				// Copying indicies (faces on Assimp)
				if (ai_mesh->HasFaces())
				{
					comp_mesh->num_indices = ai_mesh->mNumFaces * 3;
					comp_mesh->indices = new uint[comp_mesh->num_indices]; // assume each face is a triangle
					DEBUG("  -> %d indices", comp_mesh->num_indices);
					for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
					{
						if (ai_mesh->mFaces[j].mNumIndices != 3)
							DEBUG("WARNING, geometry face with != 3 indices!");
						else
						{
							memcpy(&comp_mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
						}
					}
				}

				App->renderer3D->LoadMeshBuffer(comp_mesh);							
				new_go->components.push_back(comp_mesh);
			}			
		}

		aiReleaseImport(scene);
	}
	else
	{
		DEBUG("Error loading scene %s: %s", file_name, aiGetErrorString());
	}	
}