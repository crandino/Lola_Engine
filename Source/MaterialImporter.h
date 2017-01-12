#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include "Assimp\include\material.h"

#include "ResourceTexture.h"

#pragma comment (lib, "Source/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Source/Devil/libx86/ILU.lib")
#pragma comment (lib, "Source/Devil/libx86/ILUT.lib")

typedef unsigned int uint;

class MaterialImporter
{

public:

	bool static Import(std::string &asset_to_import, std::string &imported_file, ID &res_id, const aiMaterial *ai_material = nullptr);
	uint static Load(const std::string &imported_file, ResourceTexture *res_mat);
	uint static Save(unsigned char **data, uint size);

};

#endif //__MATERIALIMPORTER_H__

