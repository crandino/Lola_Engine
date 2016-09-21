#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"
#include "Console.h"

class ModuleEditor : public Module
{
	public:

		ModuleEditor(Application* app, bool start_enabled = true);
		~ModuleEditor();

		bool Init();
		UPDATE_STATUS PreUpdate(float dt);
		UPDATE_STATUS Update(float dt);
		UPDATE_STATUS PostUpdate(float dt);
		bool CleanUp();	

	private:
		
		Console console;

		//Windows variables
		bool about_menu = false;
		bool conf_menu = false;
		bool console_menu = false;

		// Windows methods
		void ShowMenuBar();
		void ShowMenuFile();
		void ShowAboutMenu();
		void ShowConfMenu();
		void ShowConsole();
};

#endif // !__MODULEIMGUI_H__

