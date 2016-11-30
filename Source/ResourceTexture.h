#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{

public:

	char tex_path[SHORT_STRING];
	unsigned int tex_buffer;
	
	ResourceTexture(ID id, int timestamp) : Resource()
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::TEXTURES;
	}
};


#endif //!__RESOURCE_MATERIAL_H__
