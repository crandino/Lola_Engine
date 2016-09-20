#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"

#include <list>

class Application
{
public:
	ModuleWindow		*window;
	ModuleInput			*input;
	ModuleAudio			*audio;
	ModuleSceneIntro	*scene_intro;
	ModuleRenderer3D	*renderer3D;
	ModuleCamera3D		*camera;
	ModulePhysics3D		*physics;
	ModuleEditor	    *editor;

private:

	bool					app_marked_for_closing = false;
	Timer					ms_timer;
	float					dt;
	std::list<Module*>		list_modules;

public:

	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

	void CloseApp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};