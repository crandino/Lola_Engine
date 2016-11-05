#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleFileSystem.h"
#include "ModuleCameraEditor.h"
//#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleTextureLoader.h"
#include "ModuleGameObjectManager.h"
#include "ModuleRenderer3D.h"

#include "JSONParser.h"

Application::Application()
{
	window = new ModuleWindow(this, true);
	tex_loader = new ModuleTextureLoader(this, true);
	file_system = new ModuleFileSystem(this, true);
	input = new ModuleInput(this, true);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this, true);
	renderer3D = new ModuleRenderer3D(this, true);
	camera = new ModuleCameraEditor(this, true);
	//physics = new ModulePhysics3D(this, true);
	gameobject_manager = new ModuleGameObjectManager(this, true);
	editor = new ModuleEditor(this, true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);		
	AddModule(input);
	AddModule(audio);
	//AddModule(physics);

	// Loaders	
	AddModule(file_system);
	AddModule(tex_loader);
	AddModule(gameobject_manager);
	
	// Scenes
	AddModule(camera);
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

	LoadConfig();

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
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		SaveGame("Test.json");
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		LoadGame("Test.json");

	perf_info.PreUpdate();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (want_to_load == true) LoadGameNow();
	if (want_to_save == true) SaveGameNow();

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

// *************** Load / Save ******************
void Application::LoadConfig() const
{
	// --- load config file ---
	/*char* buf;
	int size = App->file_system->Load("config.json", &buf);*/

	/*JSON_Value *user_data = json_parse_file("config.json");

	if (user_data != nullptr)
	{
		JSON_Object *object = json_value_get_object(user_data);
		json_object_get_value(object, "Framerate");
		json_value
	}*/

	
	/*pugi::xml_parse_result result = config_file.
	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}

	return config_file.child("config");*/
}

void Application::LoadGame(const char *file)
{
	want_to_load = true;
	sprintf_s(load_game, SHORT_STRING, "%s%s", App->file_system->GetSaveDirectory(), file);
}

void Application::SaveGame(const char *file)
{
	want_to_save = true;
	sprintf_s(save_game, SHORT_STRING, "%s", file);
}

bool Application::SaveGameNow()
{
	bool ret = true;

	//http://kgabis.github.io/parson/

	DEBUG("Saving Game State on %s...", save_game);

	JSONParser parser;
	
	JSONParser module = parser.AddNode("Modules");
	
	std::list<Module*>::iterator it = list_modules.begin();
	for (; it != list_modules.end() && ret != false; ++it)
	{	
		ret = (*it)->Save(module.AddNode((*it)->GetModuleName()));
	}	

	char *serialized_string;
	parser.Save(&serialized_string);	
	App->file_system->Save(save_game, serialized_string, strlen(serialized_string));
	parser.FreeBuffer(serialized_string);

	want_to_save = false;

	return ret;
}

bool Application::LoadGameNow()
{
	bool ret = true;

	char* buf;
	uint size = App->file_system->Load(load_game, &buf);

	if (size > 0)
	{
		JSONParser parser(buf);
		JSONParser modules = parser.GetNode("Modules");
		
		std::list<Module*>::iterator it = list_modules.begin();
		for (; it != list_modules.end() && ret != false; ++it)			
			(*it)->Load(modules.GetNode((*it)->GetModuleName()));

		parser.FreeBuffer(buf);
	}
	else
		DEBUG("Could not load game state xml file %s", load_game);

	want_to_load = false;
	return ret;
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