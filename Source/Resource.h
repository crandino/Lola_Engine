#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <map>

enum RESOURCE_TYPE
{
	NONE,
	TEXTURES, 
	MESHES, 
	SCENES
};

typedef long unsigned int ID;

class Resource
{

protected:

	bool loaded_in_memory;
	unsigned int times_referenced;

public:

	RESOURCE_TYPE type;
	ID id;
	std::string file;
	std::string imported_file;
	int timestamp;

	Resource()
	{
		loaded_in_memory = false;
		times_referenced = 0;
		timestamp = 0;
		id = 0;
		type = RESOURCE_TYPE::NONE;
	}

	virtual bool LoadToMemory()
	{
		return true;
	}

	bool LoadedInMemory() const
	{
		return loaded_in_memory;
	}

	/*Resource(UID uid, Resource::Type type);
	virtual ~Resource();
	Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	
	uint CountReferences() const;

	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;*/



};

#endif __RESOURCES_H__
