#include "ModuleRenderer3D.h"

#include "Application.h"
#include "ModuleCameraEditor.h"
#include "ModuleWindow.h"
#include "ModuleTextureLoader.h"
#include "Globals.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "GameObject.h"

#include "openGL.h"

#include "Devil\include\ilut.h"

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
	DEBUG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		DEBUG("[error] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	sprintf_s(gl_version, SHORT_STRING, "%s", glGetString(GL_VERSION));

	GLenum gl_enum = glewInit();

	if (gl_enum != GLEW_OK)
		DEBUG("[error] Glew hasn't been initialized!");

	sprintf_s(glew_version, SHORT_STRING, "%s", glewGetString(GLEW_VERSION));
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			DEBUG("[error] Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
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
			DEBUG("[error] Error initializing OpenGL! %s\n", gluErrorString(error));
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

	// Default texture
	GenerateChecker(&checker_id);

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

	return UPDATE_CONTINUE;
}

// Update present buffer to screen
UPDATE_STATUS ModuleRenderer3D::Update(float dt)
{
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
	DEBUG("Destroying 3D Renderer");
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (App->camera->main_camera == nullptr)
	{
		float coty = 1.0f / tan(60.0f * (float)math::pi / 360.0f);

		ProjectionMatrix[0][0] = coty / ((float)width / (float)height);
		ProjectionMatrix[1][1] = coty;
		ProjectionMatrix[2][2] = (0.125f + 512.0f) / (0.125f - 512.0f);
		ProjectionMatrix[2][3] = -1.0f;
		ProjectionMatrix[3][2] = 2.0f * 0.125f * 512.0f / (0.125f - 512.0f);
		ProjectionMatrix[3][3] = 0.0f;

		glLoadMatrixf(&ProjectionMatrix.v[0][0]);
	}		
	else
	{
		ComponentCamera *c = (ComponentCamera*)App->camera->main_camera->GetComponentByType(COMPONENT_TYPE::CAMERA);
		math::float4x4 proj = c->cam_frustum.ComputeProjectionMatrix();
		glLoadMatrixf(*proj.Transposed().v);
	}	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::GenerateChecker(uint *buffer)
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0)^(((j & 0x8)) == 0))) * 255;
			check_image[i][j][0] = (GLubyte)c;
			check_image[i][j][1] = (GLubyte)c;
			check_image[i][j][2] = (GLubyte)c;
			check_image[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, buffer);
	glBindTexture(GL_TEXTURE_2D, *buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
	0, GL_RGBA, GL_UNSIGNED_BYTE, check_image);
}

void ModuleRenderer3D::DrawDirectMode()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// --- Array Vertexs --- drawn by index: see Index Array
	const int num_vertices = 8;
	math::vec *vertices = new math::vec[num_vertices];
	int i = 0;

	/*       6-------7
	       / �     / |
		  2 -�--- 3  |   Y     X
		  |  4----|- 5   |   /                 
          | /     | /    | /
		  0 ----- 1	     O __ __ Z
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
	
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

bool ModuleRenderer3D::LoadMeshBuffer(const ComponentMesh *mesh)
{
	bool ret = true;

	// Vertices
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	if (mesh->id_vertices == 0)
	{
		DEBUG("[error] Vertices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);
	}

	// Normals
	glGenBuffers(1, (GLuint*) &(mesh->id_normals));
	if (mesh->id_normals == 0)
	{
		DEBUG("[error] Normals buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);
	}

	// Texture coordinates
	glGenBuffers(1, (GLuint*) &(mesh->id_tex_coord));
	if (mesh->id_tex_coord == 0)
	{
		DEBUG("[error] Texture coordinates buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_tex_coord * 2, mesh->tex_coord, GL_STATIC_DRAW);
	}
	
	// Indices
	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	if (mesh->id_indices == 0)
	{
		DEBUG("[error] Indices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	}

	return ret;
}

void ModuleRenderer3D::ShowGameObject(GameObject *go)
{
	const ComponentMesh *mesh = nullptr;
	const ComponentMaterial *mat = nullptr;
	const ComponentCamera *cam = nullptr;

	mesh = (ComponentMesh*)go->GetComponentByType(COMPONENT_TYPE::MESH);
	mat = (ComponentMaterial*)go->GetComponentByType(COMPONENT_TYPE::MATERIAL);
	cam = (ComponentCamera*)go->GetComponentByType(COMPONENT_TYPE::CAMERA);

	if (cam) debug.DrawFrustum(cam->cam_frustum);

	// Rendering
	if (mesh != nullptr && mesh->IsActive())
	{
		// Transformation 
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(*go->transform->world_transform.Transposed().v);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);		

		if (mesh->wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (mat != nullptr && mat->IsActive())
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0); // Cleanning bind buffer;
				glBindTexture(GL_TEXTURE_2D, mat->tex_buffer);		
				glColor3f(mat->color_diffuse.r, mat->color_diffuse.g, mat->color_diffuse.b);
			}
		}			

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Is this GO selected?
		/*if (go->selected)
		{
			glLineWidth(1.5f);
			glColor3f(255.0f, 255.0f, 0.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
			glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(1.0f);
		}*/

		glPopMatrix();

		// Drawing AABB 
		if (mesh->bounding_box.IsFinite())
		{
			debug.DrawAABB(mesh->bounding_box);
			debug.DrawOBB(mesh->test);
		}			
		
		glDisable(GL_TEXTURE_2D);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}		
}