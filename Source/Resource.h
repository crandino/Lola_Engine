#ifndef __RESOURCES_H__
#define __RESOURCES_H__

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
	int times_referenced;

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

	virtual ~Resource()
	{
	}

	virtual bool LoadToMemory()
	{
		return true;
	}

	virtual bool UnloadFromMemory()
	{
		return true;
	}

	bool LoadedInMemory() const
	{
		return loaded_in_memory;
	}

	int GetNumReferences() const
	{
		return times_referenced;
	}

	void SetNumReferences(int times)
	{
		times_referenced = times;
	}

	void IncrReferences()
	{
		++times_referenced;
	}

	void DecrReferences()
	{
		if(times_referenced > 0) --times_referenced;
	}
};

#endif __RESOURCES_H__
