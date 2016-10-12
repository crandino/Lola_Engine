#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleFileSystem.h"
#include "ModuleCamera3D.h"
//#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleTextureLoader.h"
#include "ModuleGameObjectManager.h"
#include "ModuleRenderer3D.h"

Application::Application()
{
	window = new ModuleWindow(this, true);
	tex_loader = new ModuleTextureLoader(this, true);
	file_system = new ModuleFileSystem(this, true);
	input = new ModuleInput(this, true);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this, true);
	renderer3D = new ModuleRenderer3D(this, true);
	camera = new ModuleCamera3D(this, true);
	//physics = new ModulePhysics3D(this, true);
	gameobject_manager = new ModuleGameObjectManager(this, true);
	editor = new ModuleEditor(this, true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);	
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	//AddModule(physics);

	// Loaders	
	AddModule(file_system);
	AddModule(tex_loader);
	AddModule(gameobject_manager);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);

	// Strings
	sprintf_s(app_name, SHORT_STRING, "LOLA Engine");
	sprintf_s(organization, SHORT_STRING, "CITM");

	// Console
	console = new Console();
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete (*item);
		++item;
	}

	// Console
	if (console) delete console;
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	DEBUG("-------------- Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->IsEnabled() ? (*item)->Start() : true;
		++item;
	}
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	perf_info.PreUpdate();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	perf_info.PostUpdate();
}

// Call PreUpdate, Update and PostUpdate on all modules
UPDATE_STATUS Application::Update()
{
	UPDATE_STATUS ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->IsEnabled() ? (*item)->PreUpdate(perf_info.getSecDt()) : UPDATE_CONTINUE;
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->IsEnabled() ? (*item)->Update(perf_info.getSecDt()) : UPDATE_CONTINUE;
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->IsEnabled() ? (*item)->PostUpdate(perf_info.getSecDt()) : UPDATE_CONTINUE;
		++item;
	}

	FinishUpdate();

	if (app_marked_for_closing)
		ret = UPDATE_STATUS::UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

// *************** Utilities ********************

void Application::CloseApp()
{
	app_marked_for_closing = true;
}

void Application::RequestBrowser(const char *web_adress)
{
	ShellExecuteA(NULL, "open", web_adress, NULL, NULL, 0);
	/*"C:/Program Files (x86)/Google/Chrome/Application"*/
}

const char *Application::GetAppName() { return app_name; }
const char *Application::GetOrganization() { return organization; }