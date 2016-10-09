#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"

class GameObject;

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

		int node_flags, leaf_flags;
		int item_selected_by_id;
		const GameObject *go_selected = nullptr;
		
		//Windows variables
		bool about_menu = false;
		bool conf_menu = false;
		bool console_menu = false;
		bool hierarchy_menu = false;
		bool component_menu = false;

		// Windows methods
		void ShowMenuBar();
		void ShowMenuFile();
		void ShowAboutMenu();
		void ShowConfMenu();
		void ShowConsole();
		void ShowHierarchy();
		void ShowComponentInfo();

		void ExpandTree(const GameObject* go_to_expand);
};

#endif // !__MODULEIMGUI_H__

