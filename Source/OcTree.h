#ifndef __OcTree_H__
#define __OcTree_H__

#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

#include <list>

#define OCTREE_MAX_ITEMS 2

class OcTreeNode;

// Helper function to check if one rectangle complately contains another
bool Contains(const GameObject *a, const GameObject *b);
bool Intersects(const GameObject *a, const GameObject *b);

bool Contains(const math::AABB &a, const GameObject *b);
bool Intersects(const math::AABB &a, const GameObject *b);

bool Contains(const GameObject *a, const math::AABB &b);
bool Intersects(const GameObject *a, const math::AABB &b);

bool ContainsAll(const GameObject *a, const OcTreeNode *b);
bool IntersectsAll(const GameObject *a, const OcTreeNode *b);

// ----- TreeNode for OcTree -----
class OcTreeNode
{

public:

	math::AABB					rect;
	std::list<GameObject*>	    objects;
	OcTreeNode*					childs[8];

public:

	OcTreeNode(math::AABB bbox);
	~OcTreeNode();
	 
	void Insert(GameObject* go);
	//int CollectCandidates(DynArray<Collider*>& nodes, const SDL_Rect& r) const;
	void CollectRects(std::vector<OcTreeNode*> &nodes);
	
private:

	void SubdivideNode();
};

// ----- OcTree -----
class OcTree
{
public:

	OcTreeNode*	root;

	OcTree();
	~OcTree();

	void SetBoundaries(const math::AABB &r);

	void Insert(GameObject* go);
	void Clear();

	//int CollectCandidates(DynArray<Collider*>& nodes, const SDL_Rect& r) const;
	void CollectRects(std::vector<OcTreeNode*> &nodes) const;
};

#endif // __OcTree_H__