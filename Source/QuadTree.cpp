#include "QuadTree.h"

bool Contains(const GameObject *a, const GameObject *b)
{
	math::AABB a_bbox, b_bbox;

	if (a->GetAABB(a_bbox) && b->GetAABB(b_bbox))
		return a_bbox.Contains(b_bbox);
	
	return false;
}

bool Intersects(const GameObject *a, const GameObject *b)
{
	math::AABB a_bbox, b_bbox;

	if (a->GetAABB(a_bbox) && b->GetAABB(b_bbox))
		return a_bbox.Intersects(b_bbox);

	return false;
}

bool Contains(const math::AABB &a_bbox, const GameObject *b)
{
	math::AABB b_bbox;

	if (b->GetAABB(b_bbox))
		return a_bbox.Contains(b_bbox);

	return false;
}

bool Intersects(const math::AABB &a_bbox, const GameObject *b)
{
	math::AABB b_bbox;

	if (b->GetAABB(b_bbox))
		return a_bbox.Intersects(b_bbox);

	return false;
}

bool Contains(const GameObject *a, const math::AABB &b_bbox)
{
	math::AABB a_bbox;
	if (a->GetAABB(a_bbox))
		return a_bbox.Contains(b_bbox);

	return false;
}

bool Intersects(const GameObject *a, const math::AABB &b_bbox)
{
	math::AABB a_bbox;
	if (a->GetAABB(a_bbox))
		return a_bbox.Intersects(b_bbox);

	return false;
}