#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Module.h"

#include "Performance.h"
#include "Console.h"

#include <list>

class ModuleWindow;
class ModuleGeometryLoader;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class ModuleEditor;
class ModuleRenderer3D;

class Application
{
public:

	Console					console;
	Performance				perf_info;

	ModuleWindow			*window = nullptr;
	ModuleGeometryLoader    *geo_loader = nullptr;
	ModuleInput				*input = nullptr;
	ModuleAudio				*audio = nullptr;
	ModuleSceneIntro		*scene_intro = nullptr;
	ModuleRenderer3D		*renderer3D = nullptr;
	ModuleCamera3D			*camera = nullptr;
	ModulePhysics3D			*physics = nullptr;
	ModuleEditor			*editor = nullptr;

	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

	void CloseApp();
	void RequestBrowser(const char *web_adress);

	const char *GetAppName();
	const char *GetOrganization();
		
private:

	char app_name[SHORT_STRING];
	char organization[SHORT_STRING];

	bool					app_marked_for_closing = false;
	std::list<Module*>		list_modules;
	
	void PrepareUpdate();
	void FinishUpdate();	

	void AddModule(Module* mod);

};

#endif //!__APPLICATION_H__