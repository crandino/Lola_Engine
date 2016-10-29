#ifndef __ModuleCameraEditor_H__
#define __ModuleCameraEditor_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class GameObject;

class ModuleCameraEditor : public Module
{
private:
	GameObject *camera = nullptr;

public:	

	ModuleCameraEditor(Application* app, bool start_enabled = true);
	~ModuleCameraEditor();

	bool Init();
	bool Start();
	UPDATE_STATUS Update(float dt);
	bool CleanUp();

	void SetAsEditorCamera(GameObject *camera);
	const math::vec &GetCameraPos() const;
	const GameObject *GetEditorCamera() const;
};

#endif // !__ModuleCameraEditor_H__