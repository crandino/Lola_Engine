#ifndef __MODULETEXTURELOADER_H__
#define __MODULETEXTURELOADER_H__

#include "Module.h"

class ComponentMaterial;

class ModuleTextureLoader : public Module
{
public:

	unsigned int lenna, lenna_gl;

	ModuleTextureLoader(Application* app, bool start_enabled = true);
	~ModuleTextureLoader();

	bool Awake(JSONParser &config);
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

public:

	void LoadTexture(const char *file_name, unsigned int &buffer, ComponentMaterial *comp_mat);
};

#endif // !__MODULETEXTURELOADER_H__

