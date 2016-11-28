#include "ModuleResourceManager.h"

#include "Application.h"
#include "ModuleFileSystem.h"

//Importers
#include "MaterialImporter.h"
#include "MeshImporter.h"
#include "SceneImporter.h"

// Resources
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceScene.h"

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
	case (RESOURCE_TYPE::TEXTURE): res = (Resource*) new ResourceTexture(id, timestamp); break;
	case (RESOURCE_TYPE::SCENE): res = (Resource*) new ResourceScene(id, timestamp); break;
	case (RESOURCE_TYPE::MESH): res = (Resource*) new ResourceMesh(id, timestamp); break;
	
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
		case(RESOURCE_TYPE::TEXTURE):
		{			
			MaterialImporter mat_import;
			successful_import = mat_import.Import(asset_files, imported_files, IDs);
			if (successful_import) timestamp = App->file_system->GetLastTimeMod(new_asset_file.c_str(), LIBRARY_TEXTURE);
			break;
		}
		case(RESOURCE_TYPE::SCENE):
		{			
			SceneImporter scene_import;
			successful_import = scene_import.Import(asset_files, imported_files, IDs, types);
			if (successful_import) timestamp = App->file_system->GetLastTimeMod(new_asset_file.c_str(), LIBRARY_SCENES);
			break;
		}
		case(RESOURCE_TYPE::UNKNOWN):
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

	RESOURCE_TYPE type = RESOURCE_TYPE::UNKNOWN;

	if (dot_pos != std::string::npos)  // Checking dot extension existence
	{
		std::string ext(&file[dot_pos + 1]);

		// Checking textures extensions...
		for (uint i = 0; i < sizeof(tex_extensions) / sizeof(std::string); ++i)
		{
			if (tex_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::TEXTURE;
				break;
			}
		}

		// Checking scene extensions...
		for (uint i = 0; i < sizeof(mesh_extensions) / sizeof(std::string); ++i)
		{
			if (mesh_extensions[i] == ext)
			{
				type = RESOURCE_TYPE::SCENE;
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
	case(RESOURCE_TYPE::TEXTURE): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), LIBRARY_TEXTURE); break;
	case(RESOURCE_TYPE::MESH): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), LIBRARY_MESH); break;
	case(RESOURCE_TYPE::SCENE): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), LIBRARY_SCENES); break;
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