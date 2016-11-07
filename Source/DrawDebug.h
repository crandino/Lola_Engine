#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "MathGeoLib\MathGeoLib.h"
#include "OcTree.h"

#define LINE_WIDTH 3
#define DEFAULT_LINE_WIDTH 1

class DrawDebug
{
private:

	math::float3 *edges;
	math::vec *corners;	

public:	

	static bool show_aabb;
	static bool show_frustum;
	static bool show_octree;

	DrawDebug();
	~DrawDebug();

	void DrawAABB(const math::AABB &bbox) const;
	void DrawOBB(const math::OBB &bbox) const;
	void DrawFrustum(const math::Frustum &frustrum) const;
	void DrawOcTree(const OcTree &quad) const;
	void DrawLineSegment(const math::LineSegment &line) const;
};

#endif // __DRAWDEBUG_H__
