#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "MathGeoLib\MathGeoLib.h"
#include "QuadTree.h"

class DrawDebug
{
private:

	math::float3 *edges;
	math::vec *corners;

public:

	DrawDebug();
	~DrawDebug();

	void DrawAABB(const math::AABB &bbox) const;
	void DrawOBB(const math::OBB &bbox) const;
	void DrawFrustum(const math::Frustum &frustrum) const;
	void DrawQuadTree(const QuadTree &quad) const;
};

#endif // __DRAWDEBUG_H__
