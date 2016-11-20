#include "ModuleResourceManager.h"

#include "Application.h"
#include "ModuleFileSystem.h"

//Importers
#include "MaterialImporter.h"
#include "MeshImporter.h"

// Resources
#include "ResourceTexture.h"

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
	ID res_id = force_id == 0 ? GenerateID() : 0;
	int timestamp = -1;
	std::string imported_file;
	bool successful_import = false;

	RESOURCE_TYPE type = GetTypeOfFile(new_asset_file);

	switch (type)
	{
		case(RESOURCE_TYPE::TEXTURE):
		{
			MaterialImporter mat_import;
			successful_import = mat_import.Import(new_asset_file, imported_file, res_id);
			if (successful_import) timestamp = App->file_system->GetLastTimeMod(new_asset_file.c_str(), "Textures/");
			break;
		}
		case(RESOURCE_TYPE::UNKNOWN):
			DEBUG("Unknwon type for file %s", new_asset_file.c_str());
			break;
	}

	if (successful_import)
	{
		Resource *res = CreateNewResource(type, res_id, timestamp);
		res->file = new_asset_file;
		res->imported_file = imported_file;
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
	case(RESOURCE_TYPE::TEXTURE): timestamp = App->file_system->GetLastTimeMod(res->file.c_str(), "Textures/"); break;
	}

	if (timestamp > res->timestamp)
		return true;
	return false;
}

Resource *ModuleResourceManager::Get(ID id)
{
	if (id != 0)
		return resources.at(id);
	return nullptr;
}