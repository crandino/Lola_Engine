#include "ModuleTextureLoader.h"

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

void ModuleTextureLoader::LoadTexture(const char *full_path, unsigned int &buffer)
{
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &buffer);
	ilBindImage(buffer);
	char c[SHORT_STRING];
	//sprintf_s(c, SHORT_STRING, "C:/Users/Carlos/Documents/GitHub/Lola_Engine/Game/Assets/Models/%s", full_path );
	sprintf_s(c, SHORT_STRING, "Assets/Models/%s", full_path );
	ilLoadImage(c);
	buffer = ilutGLBindTexImage();
}