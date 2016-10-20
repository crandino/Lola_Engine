#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "imgui\imgui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_video.h"

#include "DrawDebug.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 512
#define CHECKERS_WIDTH 512

class ComponentMesh;
class GameObject;

class ModuleRenderer3D : public Module
{
public:

	char gl_version[SHORT_STRING], glew_version[SHORT_STRING];

	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	bool LoadMeshBuffer(const ComponentMesh *mesh);
	void ShowGameObject(GameObject *go);

	void DrawDirectMode();

public:

	//ImVec4 clear_color;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float3x3 NormalMatrix;
	math::float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:

	DrawDebug debug;

	unsigned int checker_id;
	unsigned char check_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	void GenerateChecker(unsigned int*buffer);
};

#endif // !__MODULERENDERER3D_H__