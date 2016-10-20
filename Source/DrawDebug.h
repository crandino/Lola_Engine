#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "MathGeoLib\MathGeoLib.h"

class DrawDebug
{
private:

	math::float3 *edges;

public:

	DrawDebug();
	~DrawDebug();

	void DrawAABB(const math::AABB &bbox);
	void DrawOBB(const math::OBB &bbox);
};

#endif // __DRAWDEBUG_H__
