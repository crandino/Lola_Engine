#ifndef __MODULEGEOMETRYLOADER_H__
#define __MODULEGEOMETRYLOADER_H__

#include "Module.h"
#include "MathGeoLib\MathGeoLib.h"

struct Mesh;

class ModuleGeometryLoader : public Module
{
public:

	std::vector<const Mesh*> meshes;

	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	~ModuleGeometryLoader();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();	

private:

	void LoadGeometry(const char *file_name);
};

#endif // !__MODULEGEOMETRYLOADER_H__

