#include "MaterialImporter.h"

bool MaterialImporter::Import(std::string &asset_to_import, std::string &imported_file, ID &res_id, const aiMaterial *ai_material)
{
	ilInit();

	bool success = false;

	std::string asset_folder = "Textures/";
	std::string lib_folder = LIBRARY_TEXTURE;

	char *data;
	uint size = App->file_system->Load((asset_folder + asset_to_import).c_str(), &data);

	if (size > 0)
	{
		ilLoadL(IL_TYPE_UNKNOWN, data, size);
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
		uint dds_size = ilSaveL(IL_DDS, NULL, 0);

		// Allocate data buffer for DDS image information, colors and opacity
		uint color_size = sizeof(math::float3) * 5 + sizeof(float);
		char *dds_data = new char[dds_size + color_size]; // allocate data buffer for DDS and colors

		// Save to buffer with the ilSaveIL function
		if (ilSaveL(IL_DDS, dds_data, dds_size) > 0)
		{
			char *cursor = dds_data; cursor += dds_size;

			if (ai_material != nullptr)
			{
				aiColor3D ai_color;
				math::float3 color;
				float opacity;

				// We append color and opacity information				
				ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
				color.Set(ai_color.r, ai_color.g, ai_color.b);
				memcpy(cursor, &color, sizeof(math::float3));
				cursor += sizeof(math::float3);

				ai_material->Get(AI_MATKEY_COLOR_SPECULAR, ai_color);
				color.Set(ai_color.r, ai_color.g, ai_color.b);
				memcpy(cursor, &color, sizeof(math::float3));
				cursor += sizeof(math::float3);

				ai_material->Get(AI_MATKEY_COLOR_AMBIENT, ai_color);
				color.Set(ai_color.r, ai_color.g, ai_color.b);
				memcpy(cursor, &color, sizeof(math::float3));
				cursor += sizeof(math::float3);

				ai_material->Get(AI_MATKEY_COLOR_TRANSPARENT, ai_color);
				color.Set(ai_color.r, ai_color.g, ai_color.b);
				memcpy(cursor, &color, sizeof(math::float3));
				cursor += sizeof(math::float3);

				ai_material->Get(AI_MATKEY_COLOR_EMISSIVE, ai_color);
				color.Set(ai_color.r, ai_color.g, ai_color.b);
				memcpy(cursor, &color, sizeof(math::float3));
				cursor += sizeof(math::float3);

				ai_material->Get(AI_MATKEY_OPACITY, opacity);
				memcpy(cursor, &opacity, sizeof(float));
			}
			else
			{
				memset(cursor, 0, sizeof(math::float3) * 5 + sizeof(float));
			}

			char imported_filename[100];
			sprintf_s(imported_filename, sizeof(imported_filename), "%lu%s", res_id, ".dds");
			imported_file = imported_filename;
			if (App->file_system->Save((lib_folder + imported_file).c_str(), dds_data, dds_size + color_size) != 0) success = true;
		}
		//RELEASE_ARRAY(dds_data);
		delete[] dds_data;
		delete data;
	}
	//RELEASE(data);	

	ilShutDown();
	return success;
}

uint MaterialImporter::Load(const std::string &imported_file, ResourceTexture *res_mat)
{
	char *data;
	std::string lib_folder = LIBRARY_TEXTURE;
	uint data_size = App->file_system->Load((lib_folder + imported_file).c_str(), &data);

	if (data_size > 0)
	{
		sprintf_s(res_mat->tex_path, SHORT_STRING, "%s%s", "Textures/", res_mat->file.c_str());

		uint color_size = sizeof(math::float3) * 5 + sizeof(float); // Size for colors and opacity
		char *cursor = data;

		res_mat->texture_size = data_size - color_size;
		res_mat->texture_data = new char[res_mat->texture_size];
		memcpy(res_mat->texture_data, cursor, res_mat->texture_size);
		cursor += res_mat->texture_size;

		memcpy(&res_mat->color_diffuse, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_specular, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_ambient, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_transparent, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->color_emissive, cursor, sizeof(math::float3));
		cursor += sizeof(math::float3);

		memcpy(&res_mat->opacity, cursor, sizeof(float));

		//RELEASE_ARRAY(data);
		delete[] data;
	}

	return data_size;
}

