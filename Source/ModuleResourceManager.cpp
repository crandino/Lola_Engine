#include "ModuleResourceManager.h"

#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleGameObjectManager.h"
#include "ModuleInput.h"

//Importers
#include "MaterialImporter.h"
#include "MeshImporter.h"
#include "SceneImporter.h"

// Resources
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceScene.h"

//Components
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Resource Manager");

	check_timer.Start();
	check_interval = 2.0f;
}

ModuleResourceManager::~ModuleResourceManager()
{ }

UPDATE_STATUS ModuleResourceManager::PreUpdate(float dt)
{
	if (check_timer.ReadSec() > check_interval)
	{
		std::vector<std::string> files, dirs;
		App->file_system->ExploreFiles("Assets", files, dirs, true);

		// Getting new files or overwritten files
		for (uint i = 0; i < files.size(); ++i)
		{
			ID id = Find(files[i]);
			if (id == 0)
				ImportFile(files[i]);
			else if (IsUpdated(id))
			{
				ID id_removed = DeleteImportedFile(files[i]);
				ImportFile(files[i], id_removed);
			}
		}

		check_timer.Start(); // Resetting timer;
	}	
	
	// Loading FBXs
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		LoadFile("cube.fbx");
		//LoadFile("primitives_with_parent.fbx");
		//LoadFile("Models/aabb_test.fbx");
		//LoadFile("Models/Street environment_V01.fbx");
		//LoadFile("Models/QuadTree_test3.fbx");
		//LoadFile("Models/color_cubes.fbx");
		//LoadFile("Models/conflict_octree.fbx");
		//LoadFile("Models/a_lot_of_balls2.fbx");
		App->gameobject_manager->UpdateOcTree();
	}


	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	for (std::map<ID, Resource*>::iterator it = resources.begin(); it != resources.end();++it)
		RELEASE((*it).second);
	
	resources.clear();

	return true;
}

Resource *ModuleResourceManager::CreateNewResource(RESOURCE_TYPE type, ID id, int timestamp)
{
	Resource *res = nullptr;

	switch (type)
	{
	case (RESOURCE_TYPE::TEXTURES): res = (Resource*) new ResourceTexture(id, timestamp); break;
	case (RESOURCE_TYPE::SCENES): res = (Resource*) new ResourceScene(id, timestamp); break;
	case (RESOURCE_TYPE::MESHES): res = (Resource*) new ResourceMesh(id, timestamp); break;
	
	/*case Resource::mesh: ret = (Resource*) new ResourceMesh(uid); break;
	case Resource::audio: ret = (Resource*) new ResourceAudio(uid); break;
	case Resource::scene: ret = (Resource*) new ResourceScene(uid); break;
	case Resource::bone: ret = (Resource*) new ResourceBone(uid); break;
	case Resource::animation: ret = (Resource*) new ResourceAnimation(uid); break;*/
	}

	if (res != nullptr)
		resources[id] = res;

	return res;
}

ID ModuleResourceManager::ImportFile(std::string &new_asset_file, ID force_id)
{
	ID res_id = force_id == 0 ? GenerateID() : force_id;
	int timestamp = -1;

	std::vector<std::string> imported_files, asset_files;
	asset_files.push_back(new_asset_file);

	std::vector<ID> IDs;
	IDs.push_back(res_id);

	std::vector<RESOURCE_TYPE> types;

	bool successful_import = false;

	RESOURCE_TYPE type = GetTypeOfFile(new_asset_file);
	types.push_back(type);

	switch (type)
	{
		case(RESOURCE_TYPE::TEXTURES):
		{			
			MaterialImporter mat_import;
			successful_import = mat_import.Import(asset_files, imported_files, IDs);
			if (successful_import) timestamp = App->file_system->GetLastTimeMod(new_asset_file.c_str(), "Textures/");
			break;
		}
		case(RESOURCE_TYPE::SCENES):
		{			
			SceneImporter scene_import;
			successful_import = scene_import.Import(asset_files, imported_files, IDs, types);
			if (successful_import) timestamp = App->file_system->GetLastTimeMod(new_asset_file.c_str(), "Models/");
			break;
		}
		case(RESOURCE_TYPE::NONE):
			DEBUG("Unknwon type for file %s", new_asset_file.c_str());
			break;
	}

	if (successful_import)
	{
		for (uint i = 0; i < asset_files.size(); ++i)
		{
			Resource *res = CreateNewResource(types[i], IDs[i], timestamp);
			res->file = asset_files[i];
			res->imported_file = imported_files[i];
		}		
	}

	return res_id;
}

ID ModuleResourceManager::DeleteImportedFile(std::string &asset_to_delete)
{
	ID id_removed = Find(asset_to_delete);

	Resource *res = Get(id_removed);
	App->file_system->RemoveFile(res->imported_file.c_str());
	RELEASE(res);
	DeleteEntry(id_removed);

	return id_removed;
}

ID ModuleResourceManager::GenerateID()
{
	return id_generator.Int();
}

RESOURCE_TYPE ModuleResourceManager::GetTypeOfFile(const std::string &file) const
{
	std::string tex_extensions[] = { "png", "tga", "jpg", "PNG", "TGA", "JPG"};
	std::string mesh_extensions[] = { "fbx", "FBX" };

	size_t dot_pos = file.find_last_of(".");

	RESOURCE_TYPE type = RESOURCE_TYPE::NONE;

	if (dot_pos != std::string::npos)  // Checking dot extension existence
	{
		std::string ext(&file[dot_pos + 1]);

		// Checking textures extensions...
		for (uint i = 0; i < sizeof(tex_extensions) / sizeof(std::string); ++i)
		{
			if (tex_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::TEXTURES;
				break;
			}
		}

		// Checking scene extensions...
		for (uint i = 0; i < sizeof(mesh_extensions) / sizeof(std::string); ++i)
		{
			if (mesh_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::SCENES;
				break;
			}
		}
	}
	
	return type;
}

ID ModuleResourceManager::Find(const std::string &asset_to_find) const
{
	for (std::map<ID, Resource*>::const_iterator it = resources.begin(); it != resources.end() ;++it)
	{
		if (!(*it).second->file.compare(asset_to_find))
			return (*it).first;
	}

	return 0;
}

void ModuleResourceManager::DeleteEntry(ID id)
{
	std::map<ID,Resource*>::iterator it = resources.find(id);
	resources.erase(it);
}

bool ModuleResourceManager::IsUpdated(ID id) const
{
	Resource *res = resources.at(id);
	int timestamp = 0;

	switch (res->type)
	{
		case(RESOURCE_TYPE::TEXTURES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), "Textures/"); break;
		//case(RESOURCE_TYPE::MESHES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), LIBRARY_MESH); break;
		case(RESOURCE_TYPE::SCENES): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), "Models/"); break;
	}

	if (timestamp > res->timestamp)
		return true;
	return false;
}

Resource *ModuleResourceManager::Get(ID id)
{
	//Resource *res = nullptr;
	return  resources.at(id);	
	//return (res != resources.) ? res : nullptr;
}

bool ModuleResourceManager::LoadFile(const char *file_to_load)
{
	ID id = Find(file_to_load);
	if (id == 0)
		DEBUG("File %s has not been imported!", file_to_load);
	else
	{
		// Loading resources to memory
		Resource *res = Get(Find(file_to_load));
		
		char *buf;
		std::string lib_folder = LIBRARY_SCENES;
		App->file_system->Load((lib_folder + res->imported_file).c_str(), &buf);
		JSONParser json_scene; json_scene.ParseBuffer(buf);

		for (int i = 0; i < json_scene.GetArrayCount(file_to_load); ++i)
		{
			JSONParser item = json_scene.GetArray(file_to_load, i);
			
			switch ((RESOURCE_TYPE)item.GetInt("Type"))
			{
			case(RESOURCE_TYPE::MESHES):
				{
					ResourceMesh *res_mesh = (ResourceMesh*)resources[item.GetUUID("ID")];
					if (!res_mesh->LoadedInMemory())
					{
						MeshImporter mesh_importer;
						mesh_importer.Load(item.GetString("Imported File"), res_mesh);
						res_mesh->LoadToMemory();
					}					
					break;
				}
			case(RESOURCE_TYPE::TEXTURES):
				{
					MaterialImporter material_importer;
					break;
				}
			}
		}

		// Creating GameObjects and their corresponding compoments
		lib_folder = "Models/";
		const aiScene* scene = aiImportFileEx((lib_folder + res->file).c_str(), aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());
		
		if (scene != nullptr)
		{
			if (scene->HasMeshes())
			{
				std::stack<aiNode*> nodes_stack;
				std::stack<GameObject*> go_stack;

				nodes_stack.push(scene->mRootNode);
				go_stack.push((GameObject*)App->gameobject_manager->GetRoot());

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
									go_name[i - 1] = '\0';
									break;
								}
							}

							GameObject *new_go = App->gameobject_manager->CreateGameObject(go_name, parent);
							go_stack.push(new_go);

							// --- TRANSFORM ---						
							ComponentTransform *comp_trans = (ComponentTransform*)new_go->AddComponent(COMPONENT_TYPE::TRANSFORM);
							comp_trans->SetComponent(node_to_add);

							// --- MESH ---	
							for (uint j = 0; j < node_to_add->mNumMeshes; ++j)
							{
								aiMesh *ai_mesh = scene->mMeshes[node_to_add->mMeshes[j]];
							
								ComponentMesh *comp_mesh = (ComponentMesh*)new_go->AddComponent(COMPONENT_TYPE::MESH);
								comp_mesh->AddResource((ResourceMesh*)Get(FindFileIdJSON(json_scene, file_to_load, go_name)));									

									//// --- MATERIAL ---
									//aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
									//ComponentMaterial *comp_mat = (ComponentMaterial*)new_go->AddComponent(COMPONENT_TYPE::MATERIAL);
									//comp_mat->SetComponent(ai_material);
				
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
			DEBUG("Error loading scene %s: %s", file_to_load, aiGetErrorString());
		}		
	}	

	return true;
}

ID ModuleResourceManager::FindFileIdJSON(const JSONParser &json, const char *scene_name, const char *file) const
{
	for (int i = 0; i < json.GetArrayCount(scene_name); ++i)
	{
		JSONParser item = json.GetArray(scene_name, i);

		if (strcmp(item.GetString("File"), file) == 0)
			return item.GetUUID("ID");
	}

	return 0;
}