#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"

#include "Assimp\include\cfileio.h"
#include "Devil\include\il.h"

#define LIBRARY_TEXTURE "Library/Textures"
#define LIBRARY_MESH "Library/Mesh"
#define SCENES "Scene"

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:
	
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Awake(JSONParser &config);
	bool CleanUp();

	bool AddSearchPath(const char *path_or_zip, const char *mount_point = NULL);
	bool RemoveAllSearchPaths();
	bool RemovePath(const char *path_or_zip);

	uint Load(const char* file, char **buffer) const;
	SDL_RWops *Load(const char* file) const;
	uint Save(const char *file, const char *buffer, uint size) const;

	const char *GetFileFromDirPath(const char *path) const;
	const char *GetSaveDirectory() const;
	const char *GetRealDirectory(const char* file) const;
	bool IsDirectory(const char *dir) const;
	bool Exists(const char *file) const;

	void SetWriteDirectory();

	aiFileIO * ModuleFileSystem::GetAssimpIO();

private:

	aiFileIO *AssimpIO = nullptr;	
	void ModuleFileSystem::CreateAssimpIO();
	void ModuleFileSystem::SetDevilIO();	
};


#endif //!__MODULEFILESYSTEM_H__