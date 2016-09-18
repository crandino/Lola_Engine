#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
//#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	UPDATE_STATUS Update(float dt);
	bool CleanUp();

	void Look(const math::vec &Position, const math::vec &Reference, bool RotateAroundReference = false);
	void LookAt(const math::vec &Spot);
	void Move(const math::vec &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	math::vec X, Y, Z, Position, Reference;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // !__MODULECAMERA3D_H__