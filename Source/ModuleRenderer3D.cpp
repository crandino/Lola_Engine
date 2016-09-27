#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Source/Glew/libx86/glew32.lib")   /* link Glew lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Renderer3D");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->console.AddLOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		App->console.AddLOG("[error] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	sprintf_s(gl_version, SHORT_STRING, "%s", glGetString(GL_VERSION));

	GLenum gl_enum = glewInit();

	if (gl_enum != GLEW_OK)
		App->console.AddLOG("[error] Glew hasn't been initialized!");

	sprintf_s(glew_version, SHORT_STRING, "%s", glewGetString(GLEW_VERSION));
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->console.AddLOG("[error] Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console.AddLOG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console.AddLOG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//clear_color = ImColor(114, 144, 154);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console.AddLOG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	DrawDirectMode();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleRenderer3D::PostUpdate(float dt)
{
	// Rendering
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);	

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->console.AddLOG("Destroying 3D Renderer");
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float coty = 1.0f / tan(60.0f * (float)math::pi / 360.0f);

	ProjectionMatrix[0][0] = coty / ((float)width / (float)height);
	ProjectionMatrix[1][1] = coty;
	ProjectionMatrix[2][2] = (0.125f + 512.0f) / (0.125f - 512.0f);
	ProjectionMatrix[2][3] = -1.0f;
	ProjectionMatrix[3][2] = 2.0f * 0.125f * 512.0f / (0.125f - 512.0f);
	ProjectionMatrix[3][3] = 0.0f;	

	/*Perspective.M[0] = coty / ((float)width / (float)height);
	Perspective.M[5] = coty;
	Perspective.M[10] = (0.125f + 512.0f) / (0.125f - 512.0f);
	Perspective.M[11] = -1.0f;
	Perspective.M[14] = 2.0f * 0.125f * 512.0f / (0.125f - 512.0f);
	Perspective.M[15] = 0.0f;

	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);*/
	glLoadMatrixf(&ProjectionMatrix.v[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawDirectMode()
{
	// --- Array Vertexs --- drawn by index: see Index Array
	const int num_vertices = 8;
	math::vec *vertices = new math::vec[num_vertices];
	int i = 0;

	/*       6-------7
	       / �     / |
		  2 -�--- 3  |
		  |  4----|- 5                      
          | /     | /
		  0 ----- 1	
	*/

	vertices[i++] = { 0.0f, 0.0f, 0.0f };  // 0
	vertices[i++] = { 0.0f, 0.0f, 2.0f };  // 1
	vertices[i++] = { 0.0f, 2.0f, 0.0f };  // 2
	vertices[i++] = { 0.0f, 2.0f, 2.0f };  // 3
	vertices[i++] = { 2.0f, 0.0f, 0.0f };  // 4
	vertices[i++] = { 2.0f, 0.0f, 2.0f };  // 5
	vertices[i++] = { 2.0f, 2.0f, 0.0f };  // 6
	vertices[i++] = { 2.0f, 2.0f, 2.0f };  // 7

	uint my_id = 0;
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glDrawArrays(GL_TRIANGLES, 0, num_vertices * 3);

	// --- Index Array ----
	const int num_indices = 36;
	uint *indices = new uint[num_indices];
	i = 0;

	// Front
	indices[i++] = 0;
	indices[i++] = 1;
	indices[i++] = 2;
	indices[i++] = 1;
	indices[i++] = 3;
	indices[i++] = 2;

	// Right
	indices[i++] = 3;
	indices[i++] = 1;
	indices[i++] = 5;
	indices[i++] = 3;
	indices[i++] = 5;
	indices[i++] = 7;

	// Left
	indices[i++] = 4;
	indices[i++] = 0;
	indices[i++] = 2;
	indices[i++] = 2;
	indices[i++] = 6;
	indices[i++] = 4;

	// Rear
	indices[i++] = 6;
	indices[i++] = 5;
	indices[i++] = 4;
	indices[i++] = 6;
	indices[i++] = 7;
	indices[i++] = 5;

	// Top
	indices[i++] = 6;
	indices[i++] = 2;
	indices[i++] = 3;
	indices[i++] = 6;
	indices[i++] = 3;
	indices[i++] = 7;

	// Bottom
	indices[i++] = 0;
	indices[i++] = 4;
	indices[i++] = 1;
	indices[i++] = 4;
	indices[i++] = 5;
	indices[i++] = 1;

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glIndexPointer(GL_UNSIGNED_INT, 0, NULL);
	glDrawElements(GL_TRIANGLES, sizeof(uint)*num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

}
