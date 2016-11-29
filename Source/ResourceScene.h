#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"

class ResourceScene : public Resource
{

public:

	ResourceScene(ID id, int timestamp) : Resource()
	{
		this->id = id;
		this->timestamp = timestamp;
		type = RESOURCE_TYPE::SCENES;
	}
};


#endif //!__RESOURCE_SCENE_H__


