#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Globals.h"

class ModuleEditor : public Module
{
	private:

	public:

		ModuleEditor(Application* app, bool start_enabled = true);
		~ModuleEditor();

		bool Init();
		UPDATE_STATUS PreUpdate(float dt);
		UPDATE_STATUS Update(float dt);
		UPDATE_STATUS PostUpdate(float dt);
		bool CleanUp();		
};

#endif // !__MODULEIMGUI_H__

