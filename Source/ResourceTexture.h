#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

class ResourceTexture : public Resource
{

public:

	char tex_path[SHORT_STRING];
	unsigned int tex_buffer;
	char *texture_data;
	uint texture_size;

	math::float3  color_diffuse;
	math::float3  color_specular;
	math::float3  color_ambient;
	math::float3  color_emissive;
	math::float3  color_transparent;
	float opacity;
	
	ResourceTexture(ID id, int timestamp) : Resource()
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::TEXTURES;

		tex_buffer = 0;
		texture_data = nullptr;
		texture_size = 0;

		color_diffuse = math::float3::zero;
		color_specular = math::float3::zero;
		color_ambient = math::float3::zero;
		color_emissive = math::float3::zero;
		color_transparent = math::float3::zero;

		opacity = 0;
	}

	~ResourceTexture()
	{
		//RELEASE_ARRAY(texture_data);  // Freeing texture data
		if(texture_data != nullptr)
			delete[] texture_data;
	}

	bool ResourceTexture::LoadToMemory()
	{
		if (!loaded_in_memory)
		{
			loaded_in_memory = true;

			ilutRenderer(ILUT_OPENGL);
			ilGenImages(1, &tex_buffer);
			ilBindImage(tex_buffer);
			ilLoadL(IL_TYPE_UNKNOWN, texture_data, texture_size);
			tex_buffer = ilutGLBindTexImage();
		}

		return loaded_in_memory;
	}

	bool ResourceTexture::UnloadFromMemory()
	{
		if (loaded_in_memory)
		{			
			loaded_in_memory = false;

			ilDeleteImages(1, &tex_buffer);
			tex_buffer = 0;
			texture_size = 0;
		}

		return !loaded_in_memory;
	}
};


#endif //!__RESOURCE_MATERIAL_H__
