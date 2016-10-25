#ifndef __ModuleCameraEditor_H__
#define __ModuleCameraEditor_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class GameObject;

class ModuleCameraEditor : public Module
{
public:
	ModuleCameraEditor(Application* app, bool start_enabled = true);
	~ModuleCameraEditor();

	bool Init();
	bool Start();
	UPDATE_STATUS Update(float dt);
	bool CleanUp();

	void Look(const math::vec &Position, const math::vec &Reference, bool RotateAroundReference = false);
	void LookAt(const math::vec &Spot);
	void Move(const math::vec &Movement);
	float* GetViewMatrix();

private:

	GameObject *main_camera;
	void CalculateViewMatrix();
	void CreateEditorCamera();

public:
	
	math::vec X, Y, Z, Position, Reference;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // !__ModuleCameraEditor_H__