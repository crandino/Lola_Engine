#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"

class ResourceScene : public Resource
{

public:

	ResourceScene(ID id, int timestamp)
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::SCENE;
	}
};


#endif //!__RESOURCE_SCENE_H__


