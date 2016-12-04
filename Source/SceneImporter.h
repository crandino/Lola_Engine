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

class SceneImporter
{
public:

	bool static Import(std::vector<std::string> &assets_to_import, std::vector<std::string> &imported_files, std::vector<ID> &IDs, std::vector<RESOURCE_TYPE> &types);
	//bool static Load(const std::string &imported_file, ResourceScene *scene);
};


#endif //__SCENEIMPORTER_H__
