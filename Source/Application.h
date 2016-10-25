#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Module.h"

#include "Performance.h"
#include "Console.h"

#include <list>

class ModuleWindow;
class ModuleTextureLoader;
class ModuleFileSystem;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCameraEditor;
class ModulePhysics3D;
class ModuleEditor;
class ModuleGameObjectManager;
class ModuleRenderer3D;

class Application
{
public:

	Performance				perf_info;
	Console					*console = nullptr;

	ModuleWindow			*window = nullptr;
	ModuleTextureLoader     *tex_loader = nullptr;
	ModuleFileSystem		*file_system = nullptr;
	ModuleInput				*input = nullptr;
	ModuleAudio				*audio = nullptr;
	ModuleSceneIntro		*scene_intro = nullptr;
	ModuleRenderer3D		*renderer3D = nullptr;
	ModuleCameraEditor		*camera = nullptr;
	//ModulePhysics3D			*physics = nullptr;
	ModuleGameObjectManager *gameobject_manager = nullptr;
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

extern Application *App;

#endif //!__APPLICATION_H__