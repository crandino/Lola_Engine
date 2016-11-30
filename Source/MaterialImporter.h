#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"

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

class MaterialImporter : public Importer
{

public:

	uint static Save()
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

	bool static Import(std::vector<std::string> &asset_to_import, std::vector<std::string> &imported_file, std::vector<ID> &res_id)
	{
		bool success = false;

		std::string asset_folder = "Textures/";
		std::string lib_folder = LIBRARY_TEXTURE;		

		char *data = nullptr;
		uint size = App->file_system->Load((asset_folder + asset_to_import.back()).c_str(), &data);		

		if (size > 0)
		{
			ilLoadL(IL_TYPE_UNKNOWN, data, size);
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
			uint dds_size = ilSaveL(IL_DDS, NULL, 0);

			char *dds_data = new char[dds_size]; // allocate data buffer
			if (ilSaveL(IL_DDS, dds_data, dds_size) > 0) // Save to buffer with the ilSaveIL function
			{
				char imported_filename[100];
				sprintf_s(imported_filename, sizeof(imported_filename), "%lu%s", res_id.back(), ".dds");
				imported_file.push_back(imported_filename);
				if (App->file_system->Save((lib_folder + imported_file.back()).c_str(), dds_data, dds_size) != 0) success = true;
			}
			RELEASE_ARRAY(dds_data);
		}

		RELEASE(data);
		return success;		
	}

	uint static Load(const std::string &imported_file, ResourceTexture *res_mat)
	{
		char *data;
		std::string lib_folder = LIBRARY_TEXTURE;
		uint size = App->file_system->Load((lib_folder + imported_file).c_str(), &data);

		ilutRenderer(ILUT_OPENGL);
		ilGenImages(1, &res_mat->tex_buffer);
		ilBindImage(res_mat->tex_buffer);
		ilLoadL(IL_TYPE_UNKNOWN, data, size);
		res_mat->tex_buffer = ilutGLBindTexImage();

		sprintf_s(res_mat->tex_path, SHORT_STRING, "%s%s", "Textures/", res_mat->file.c_str());

		return size;
	}
};

#endif // !__MATERIALIMPORTER_H__
