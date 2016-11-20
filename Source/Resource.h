#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <map>

enum RESOURCE_TYPE
{
	UNKNOWN,
	TEXTURE, 
	MESH, 
	SCENE
};

typedef long unsigned int ID;

class Resource
{

public:

	RESOURCE_TYPE type;
	ID id = 0;
	std::string file;
	std::string imported_file;
	int timestamp = 0;

	/*Resource(UID uid, Resource::Type type);
	virtual ~Resource();
	Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;

	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;*/



};

#endif __RESOURCES_H__
