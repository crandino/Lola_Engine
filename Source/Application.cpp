#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);

	// Strings
	sprintf_s(app_name, SHORT_STRING, "LOLA Engine");
	sprintf_s(organization, SHORT_STRING, "CITM");
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete (*item);
		++item;
	}
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
	console.AddLOG("-------------- Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
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
		ret = (*item)->PreUpdate(perf_info.getSecDt());
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(perf_info.getSecDt());
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(perf_info.getSecDt());
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