#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "ResourceScene.h"

#include <stack>

#include "JSONParser.h"

typedef long unsigned int ID;
enum RESOURCE_TYPE;

class SceneImporter : public Importer
{

public:

	bool static Import(std::vector<std::string> &assets_to_import, std::vector<std::string> &imported_files, std::vector<ID> &IDs, std::vector<RESOURCE_TYPE> &types)
	{
		// Creating JSON scene information
		char imported_filename[SHORT_STRING];
		sprintf_s(imported_filename, SHORT_STRING, "%lu%s", IDs.back(), ".scene");

		JSONParser json_scene;
		json_scene.AddUUID("ID", IDs.back());
		json_scene.AddString("File", assets_to_import.back().c_str());		
		json_scene.AddString("Imported File", imported_filename);
		imported_files.push_back(imported_filename);

		std::string models = "Models/";
		std::vector<std::string> unique_textures;
		
		const aiScene* scene = aiImportFileEx((models + assets_to_import.back()).c_str(), aiProcessPreset_TargetRealtime_MaxQuality, App->file_system->GetAssimpIO());

		if (scene != nullptr)
		{
			if (scene->HasMeshes())
			{
				json_scene.AddArray(assets_to_import.back().c_str());

				std::stack<aiNode*> nodes_stack;
				nodes_stack.push(scene->mRootNode);

				aiNode *curr_node;

				while (!nodes_stack.empty())
				{
					curr_node = nodes_stack.top();
					int num_children = curr_node->mNumChildren;

					if (num_children > 0)
					{
						nodes_stack.pop();  // Node checked is eliminated.

						for (int i = 0; i < num_children; ++i)
						{
							aiNode *node_to_add = curr_node->mChildren[i];

							nodes_stack.push(node_to_add);
							char *name = node_to_add->mName.data;

							// I don't like! This part eliminates the strange names on Ricard's FBX,
							// but it is exclusive for his FBX, so a better and generic option has to be implemented.						
							for (uint i = 0; i < node_to_add->mName.length; ++i)
							{
								if (name[i] == '$')
								{
									name[i - 1] = '\0';
									break;
								}
							}

							// --- MESH ---	
							for (uint j = 0; j < node_to_add->mNumMeshes; ++j)
							{
								aiMesh *ai_mesh = scene->mMeshes[node_to_add->mMeshes[j]];

								bool accepted = true;
								// Checking for correct number of indices per face. If not, discarded.
								for (uint k = 0; k < ai_mesh->mNumFaces; ++k)
								{
									if (ai_mesh->mFaces[k].mNumIndices != 3)
									{
										DEBUG("WARNING, geometry face with != 3 indices!");
										DEBUG("%s %d %s %s", "Mesh", j, "on", node_to_add->mName.C_Str(), "will not be loaded");
										accepted = false;
									}
								}

								if (accepted)
								{
									// Properties
									types.push_back(RESOURCE_TYPE::MESHES);  // Type
									ID next_id = IDs.back() + 1;			 // Resource ID
									IDs.push_back(next_id);
									assets_to_import.push_back(name);		 // Asset filename	
									imported_files.push_back(std::string()); // Imported filename
									MeshImporter::Import(ai_mesh, imported_files.back(), IDs.back());  

									// JSON entry for this mesh
									JSONParser mesh;									
									mesh.AddInt("Type", RESOURCE_TYPE::MESHES);
									mesh.AddUUID("ID", next_id);
									mesh.AddString("File", name);
									mesh.AddString("Imported File", imported_files.back().c_str());
									json_scene.AddArray(mesh);																									

									// ------  MATERIAL -------
									aiMaterial *ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
									if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
									{
										// Where is this texture?
										aiString path; ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
										std::string new_texture = App->file_system->GetFileFromDirPath(path.C_Str());

										//Adding new texture if there isn't something similar...
										if (std::find(unique_textures.begin(), unique_textures.end(), new_texture) == unique_textures.end())
										{
											unique_textures.push_back(new_texture);

											// Properties
											types.push_back(RESOURCE_TYPE::TEXTURES);  // Type
											ID next_id = IDs.back() + 1;			   // Resource ID
											IDs.push_back(next_id);
											assets_to_import.push_back(new_texture);   // Asset filename	
											imported_files.push_back(std::string());   // Imported filename
											MaterialImporter::Import(ai_material, assets_to_import.back(), imported_files.back(), IDs.back());		

											// JSON entry for this texture
											JSONParser material;
											material.AddInt("Type", RESOURCE_TYPE::TEXTURES);
											material.AddUUID("ID", next_id);
											material.AddString("File", assets_to_import.back().c_str());
											material.AddString("Imported File", imported_files.back().c_str());
											json_scene.AddArray(material);
										}											
									}									
								}
							}
						}
					}
					else
						nodes_stack.pop();
				}

				char *serialized_string;
				json_scene.Save(&serialized_string);
				std::string lib_folder = LIBRARY_SCENES;
				App->file_system->Save((lib_folder + imported_files.front()).c_str(), serialized_string, strlen(serialized_string));
				json_scene.FreeBuffer(serialized_string);
			}

			aiReleaseImport(scene);
			return true;
		}	
		return false;
	}

	bool static Load(const std::string &imported_file, ResourceScene *scene)
	{
		char *buf;
		if (App->file_system->Load(imported_file.c_str(), &buf) != 0)
		{
			JSONParser scene(buf);
		}
	}
};


#endif //__SCENEIMPORTER_H__
