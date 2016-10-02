#include "ModuleGeometryLoader.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "Mesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Source/Assimp/libx86/assimp.lib")

ModuleGeometryLoader::ModuleGeometryLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.assign("Geometry Loader");
}

// Destructor
ModuleGeometryLoader::~ModuleGeometryLoader()
{}

// Called before render is available
bool ModuleGeometryLoader::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleGeometryLoader::PreUpdate(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		LoadGeometry("C:/Users/Carlos/Desktop/tank2.fbx");

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGeometryLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleGeometryLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGeometryLoader::CleanUp()
{
	bool ret = true;

	// detach log stream
	aiDetachAllLogStreams();

	return ret;
}

void ModuleGeometryLoader::LoadGeometry(const char *full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// For each mesh...
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh *mesh = new Mesh();
			aiMesh *ai_mesh = scene->mMeshes[i];

			// Copying vertices...
			mesh->num_vertices = ai_mesh->mNumVertices;
			mesh->vertices = new math::float3[mesh->num_vertices];
			memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(math::float3) * mesh->num_vertices);
			DEBUG("New mesh with %d vertices", mesh->num_vertices);

			// Copying normals...
			mesh->num_normals = ai_mesh->mNumVertices;
			mesh->normals = new math::float3[mesh->num_normals];
			memcpy(mesh->normals, ai_mesh->mNormals, sizeof(math::float3) * mesh->num_vertices);
			DEBUG("New mesh with %d normals", mesh->num_vertices);

			// Copying indicies (faces on Assimp)
			if (ai_mesh->HasFaces())
			{
				mesh->num_indices = ai_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
					{
						DEBUG("WARNING, geometry face with != 3 indices!");
					}						
					else
						memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}

			App->renderer3D->LoadMeshBuffer(mesh);
			meshes.push_back(mesh);
		}

		aiReleaseImport(scene);
	}
	else
		DEBUG("Error loading scene %s", full_path);
}