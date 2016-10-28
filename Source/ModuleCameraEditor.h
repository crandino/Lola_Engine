#ifndef __ModuleCameraEditor_H__
#define __ModuleCameraEditor_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class GameObject;

class ModuleCameraEditor : public Module
{
public:

	GameObject *camera = nullptr;

	ModuleCameraEditor(Application* app, bool start_enabled = true);
	~ModuleCameraEditor();

	bool Init();
	bool Start();
	UPDATE_STATUS Update(float dt);
	bool CleanUp();

	void SetAsEditorCamera(GameObject *camera);
};

#endif // !__ModuleCameraEditor_H__