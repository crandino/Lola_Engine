#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include "Assimp\include\material.h"

#pragma comment (lib, "Source/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Source/Devil/libx86/ILU.lib")
#pragma comment (lib, "Source/Devil/libx86/ILUT.lib")

typedef unsigned int uint;

class MaterialImporter : public Importer
{

public:

	uint Save()
	{
		ILuint size;
		ILubyte *data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size > 0) {

			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				App->file_system->Save("Image_Devil", (char*)data, size);

			RELEASE_ARRAY(data);
		}
	}

	bool Import(std::vector<std::string> &asset_to_import, std::vector<std::string> &imported_file, std::vector<ID> &res_id)
	{
		std::string asset_folder = "Textures/";
		asset_to_import.back() = App->file_system->GetFileFromDirPath(asset_to_import.back().c_str());
		std::string lib_folder = LIBRARY_TEXTURE;		

		char *data;
		uint size = App->file_system->Load((asset_folder + asset_to_import.back()).c_str(), &data);
		ilLoadL(IL_TYPE_UNKNOWN, data, size);

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		//size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size > 0)
		{
			char *dds_data = new char[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, dds_data, size) > 0) // Save to buffer with the ilSaveIL function
			{
				char buff[100];
				sprintf_s(buff, sizeof(buff), "%lu%s", res_id.back(), ".dds");
				imported_file.push_back(buff);
				App->file_system->Save((lib_folder + imported_file.back()).c_str(), data, size);
			}				

			RELEASE_ARRAY(data);
			return true;
		}
		return false;		
	}

	//bool Import(aiMaterial *ai_material, std::vector<std::string> &asset_file, std::vector<std::string> &imported_file, std::vector<ID> &res_id)
	//{
	//	// Loading texture to tex_buffer
	//	aiString path;
	//	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	//	asset_file.push_back(path.C_Str());
	//	return Import(asset_file, imported_file, res_id);
	//}
};

#endif // !__MATERIALIMPORTER_H__
