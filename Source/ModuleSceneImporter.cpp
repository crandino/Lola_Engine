#include "ModuleSceneImporter.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "ModuleGameObjectManager.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include <stack>

#pragma comment (lib, "Source/Assimp/libx86/assimp.lib")

ModuleSceneImporter::ModuleSceneImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Scene Importer");
}

ModuleSceneImporter::~ModuleSceneImporter()
{ }

bool ModuleSceneImporter::Awake(JSONParser &config)
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

UPDATE_STATUS ModuleSceneImporter::PreUpdate(float dt)
{
	static bool load_model = true;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		//ImportModel("Models/primitives_with_parent.fbx");  //Street environment_V01.fbx
		//ImportModel("Models/aabb_test.fbx");
		//ImportModel("Models/Street environment_V01.fbx");
	}

	if (load_model)
	{
		ImportModel("Models/QuadTree_test3.fbx");  //Street environment_V01.fbx
												   //ImportModel("Models/color_cubes.fbx");  //Street environment_V01.fbx
		load_model = false;
	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneImporter::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleSceneImporter::ImportModel(const char *file_name, bool use_fs)
{
	const aiScene* scene;

	if (use_fs)
		scene = aiImportFileEx(file_name, aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());
	else
		scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr)
	{
		if (scene->HasMeshes())
		{
			std::stack<aiNode*> nodes_stack;
			std::stack<GameObject*> go_stack;

			nodes_stack.push(scene->mRootNode);
			go_stack.push((GameObject*)App->gameobject_manager->GetRoot());

			aiNode *curr_node;
			GameObject *parent;

			while (!nodes_stack.empty())
			{
				curr_node = nodes_stack.top();
				parent = go_stack.top();

				int num_children = curr_node->mNumChildren;

				if (num_children > 0)
				{
					nodes_stack.pop();  // Node checked is eliminated.
					go_stack.pop();

					for (int i = 0; i < num_children; ++i)
					{
						aiNode *node_to_add = curr_node->mChildren[i];

						nodes_stack.push(node_to_add);
						char *go_name = node_to_add->mName.data;

						// I don't like! This part eliminates the strange names on Ricard's FBX,
						// but it is exclusive for his FBX, so a better and generic option has to be implemented.						
						for (uint i = 0; i < node_to_add->mName.length; ++i)
						{
							if (go_name[i] == '$')
							{
								go_name[i - 1] = '\0';
								break;
							}
						}

						GameObject *new_go = App->gameobject_manager->CreateGameObject(go_name, parent);
						go_stack.push(new_go);

						// --- TRANSFORM ---						
						ComponentTransform *comp_trans = (ComponentTransform*)new_go->AddComponent(COMPONENT_TYPE::TRANSFORM);
						comp_trans->SetComponent(node_to_add);

						// --- MESH ---						
						if (node_to_add->mNumMeshes != 0)
						{
							aiMesh *ai_mesh = scene->mMeshes[*node_to_add->mMeshes];
							ComponentMesh *comp_mesh = (ComponentMesh*)new_go->AddComponent(COMPONENT_TYPE::MESH);
							comp_mesh->SetComponent(ai_mesh);

							MeshImporter mesh_importer;
							char save_filename[SHORT_STRING]; sprintf_s(save_filename, SHORT_STRING, "%lu%s", comp_mesh->UUID, ".msh");
							char *buf;
							unsigned int size = mesh_importer.Save(&buf, &comp_mesh->mesh);
							App->file_system->Save(save_filename, buf, size);
							RELEASE_ARRAY(buf);

							App->renderer3D->LoadMeshBuffer(&comp_mesh->mesh);

							// --- MATERIAL ---
							aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
							ComponentMaterial *comp_mat = (ComponentMaterial*)new_go->AddComponent(COMPONENT_TYPE::MATERIAL);
							comp_mat->SetComponent(ai_material);
						}
					}
				}
				else
				{
					go_stack.pop();
					nodes_stack.pop();
				}
			}
		}

		aiReleaseImport(scene);
	}
	else
	{
		DEBUG("Error loading scene %s: %s", file_name, aiGetErrorString());
	}
}