#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

#include "Resource.h"

#include "Timer.h"

#include "MathGeoLib\MathGeoLib.h"

typedef long unsigned int ID;

class ModuleResourceManager : public Module
{

public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	UPDATE_STATUS PreUpdate(float dt);

	bool CleanUp();

	Resource* CreateNewResource(RESOURCE_TYPE type, ID id, int timestamp);
	ID ImportFile(std::string &new_asset_file, ID force_id = 0);
	ID DeleteImportedFile(std::string &asset_to_delete);
	ID GenerateID();

private:

	Timer check_timer;
	float check_interval;

	math::LCG id_generator;

	std::map<ID, Resource*> resources;

	ID Find(const std::string &asset_to_find) const;
	void DeleteEntry(ID id);
	bool IsUpdated(ID id) const;
	Resource* Get(ID uid);

	RESOURCE_TYPE GetTypeOfFile(const std::string &file) const;
};

#endif // !__MODULERESOURCEMANAGER_H__
