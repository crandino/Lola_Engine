#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

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

	bool Import(std::string asset_to_import, std::string &imported_file, ID res_id)
	{
		std::string asset_folder = "Textures/";
		std::string lib_folder = LIBRARY_TEXTURE;

		char *data;
		uint size = App->file_system->Load((asset_folder + asset_to_import).c_str(), &data);
		ilLoadL(IL_TYPE_UNKNOWN, data, size);

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		//size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size > 0)
		{
			char *dds_data = new char[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, dds_data, size) > 0) // Save to buffer with the ilSaveIL function
			{
				char buff[100];
				sprintf_s(buff, sizeof(buff), "%lu%s", res_id, ".dds");
				imported_file = buff;
				App->file_system->Save((lib_folder + imported_file).c_str(), data, size);
			}				

			RELEASE_ARRAY(data);
			return true;
		}
		return false;		
	}

	uint Load(char **data)
	{
		return 0;
	}
};

#endif // !__MATERIALIMPORTER_H__
