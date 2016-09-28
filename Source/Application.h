#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleGeometryLoader.h"

#include "Performance.h"
#include "Console.h"

#include <list>

class Application
{
public:

	Console					console;
	Performance				perf_info;

	ModuleWindow			*window;
	ModuleGeometryLoader    *geo_loader;
	ModuleInput				*input;
	ModuleAudio				*audio;
	ModuleSceneIntro		*scene_intro;
	ModuleRenderer3D		*renderer3D;
	ModuleCamera3D			*camera;
	ModulePhysics3D			*physics;
	ModuleEditor			*editor;	

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

	char app_name[SHORT_STRING], organization[SHORT_STRING];

	bool					app_marked_for_closing = false;
	std::list<Module*>		list_modules;
	
	void PrepareUpdate();
	void FinishUpdate();	

	void AddModule(Module* mod);

};

#endif //!__APPLICATION_H__