#ifndef __OcTree_H__
#define __OcTree_H__

#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

#include <list>

#define OCTREE_MAX_ITEMS 2

class OcTreeNode;

// Helper function to check if one rectangle complately contains another
//bool Contains(const GameObject *a, const GameObject *b);
//bool Intersects(const GameObject *a, const GameObject *b);
//
//bool Contains(const math::AABB &a, const GameObject *b);
//bool Intersects(const math::AABB &a, const GameObject *b);
//
//bool ContainsAllChildren(const GameObject *a, const OcTreeNode *b);
//bool IntersectsAllChildren(const GameObject *a, const OcTreeNode *b);

// ----- TreeNode for OcTree -----
class OcTreeNode
{

public:

	math::AABB					box;
	std::list<GameObject*>	    objects;
	OcTreeNode*					childs[8];

public:
	
	OcTreeNode(math::AABB bbox);
	~OcTreeNode();
	 
	bool Insert(GameObject* go);
	bool SharedByMoreThanXChild(GameObject *go, unsigned int X) const;
	template <class PRIMITIVE>
	int CollectCandidates(std::vector<GameObject*> &nodes, const PRIMITIVE &primitive) const;
	void CollectRects(std::vector<OcTreeNode*> &nodes);

	bool HasChildren() const;
	
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

	bool Insert(GameObject* go);
	void Clear();
	
	template<class PRIMITIVE>
	int CollectCandidates(std::vector<GameObject*> &nodes, const PRIMITIVE &primitive) const;
	void CollectRects(std::vector<OcTreeNode*> &nodes) const;
};

template <class PRIMITIVE>
int OcTreeNode::CollectCandidates(std::vector<GameObject*> &nodes, const PRIMITIVE &primitive) const
{
	int tests = 0;

	for (std::list<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
		nodes.push_back(*it);

	if (HasChildren())
	{
		for (int i = 0; i < 8; ++i)
		{
			if (childs[i]->box.Intersects(primitive))
				tests += childs[i]->CollectCandidates(nodes, childs[i]->box);
			++tests;
		}
	}	

	return tests;
}

template <class PRIMITVE>
int OcTree::CollectCandidates(std::vector<GameObject*> &nodes, const PRIMITVE &primitive) const
{
	int tests = 1;
	if (root != nullptr && root->box.Intersects(primitive))
		tests += root->CollectCandidates(nodes, primitive);
	return tests;
}

#endif // __OcTree_H__