#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Mesh.h"

#include "imgui\imgui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_video.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

struct Mesh;

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

	bool LoadMeshBuffer(const Mesh *mesh);
	void DrawMesh(const Mesh *mesh);

	void DrawDirectMode();

public:

	//ImVec4 clear_color;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float3x3 NormalMatrix;
	math::float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:

	uint checker_id;
	unsigned char check_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	void GenerateChecker(uint *buffer);
};

#endif // !__MODULERENDERER3D_H__