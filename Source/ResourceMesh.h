#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"

class ResourceMesh : public Resource
{

public:

	ResourceMesh(ID id, int timestamp)
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::MESH;
	}
};


#endif //!__RESOURCE_MESH_H__

