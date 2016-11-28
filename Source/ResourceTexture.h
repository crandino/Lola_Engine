#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{

public:
	
	ResourceTexture(ID id, int timestamp)
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::TEXTURE;
	}
};


#endif //!__RESOURCE_MATERIAL_H__
