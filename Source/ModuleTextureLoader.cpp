#include "ModuleTextureLoader.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "ComponentMaterial.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Source/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Source/Devil/libx86/ILU.lib")
#pragma comment (lib, "Source/Devil/libx86/ILUT.lib")

ModuleTextureLoader::ModuleTextureLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Texture Loader");
}

// Destructor
ModuleTextureLoader::~ModuleTextureLoader()
{}

// Called before render is available
bool ModuleTextureLoader::Init()
{
	bool ret = true;

	ilInit();

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleTextureLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleTextureLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleTextureLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTextureLoader::CleanUp()
{
	bool ret = true;
	return ret;
}

void ModuleTextureLoader::LoadTexture(const char *full_path, unsigned int &buffer, ComponentMaterial *comp_mat)
{
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &buffer);
	ilBindImage(buffer);

	sprintf_s(comp_mat->tex_path, SHORT_STRING, "%s%s", "Textures/", App->file_system->GetFileFromDirPath(full_path));

	char *tmp;
	uint size = App->file_system->Load(comp_mat->tex_path, &tmp);
	ilLoadL(IL_TYPE_UNKNOWN, tmp, size);

	buffer = ilutGLBindTexImage();
}