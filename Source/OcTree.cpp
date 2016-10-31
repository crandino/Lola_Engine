#include "OcTree.h"

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

bool ContainsAllChildren(const GameObject *a, const OcTreeNode * b)
{
	bool contains_all = true;

	math::AABB a_bbox;
	if (a->GetAABB(a_bbox) && b->childs[0] != nullptr)
	{
		for (uint i = 0; i < 8; ++i)
		{
			if (!a_bbox.Contains(b->childs[i]->box))
			{
				contains_all = false;
				break;
			}
		}

		return contains_all;
	}

	return false;
}

bool IntersectsAllChildren(const GameObject *a, const OcTreeNode *b)
{
	bool intersects_all = true;

	math::AABB a_bbox;
	if (a->GetAABB(a_bbox) && b->childs[0] != nullptr)
	{
		for (uint i = 0; i < 8; ++i)
		{
			if (!a_bbox.Intersects(b->childs[i]->box))
			{
				intersects_all = false;
				break;
			}
		}

		return intersects_all;
	}

	return false;
}

// ---- OCTREE NODE ----

OcTreeNode::OcTreeNode(math::AABB bbox) : box(bbox)
{
	for (int i = 0; i < 8; ++i)
		childs[i] = nullptr;
}

OcTreeNode::~OcTreeNode()
{
	for (int i = 0; i < 8; ++i)
		if (childs[i] != nullptr) delete childs[i];
}

void OcTreeNode::Insert(GameObject* go)
{
	math::AABB bounding_box;
	go->GetAABB(bounding_box);

	// Each new item will go deep to the set of children, if they exists.
	if (childs[0] != nullptr)
	{
		if (!(IntersectsAllChildren(go, this)))
		{
			for (int i = 0; i < 8; ++i)
				childs[i]->Insert(go);
		}
		else
			objects.push_back(go);					// No limit for items that spread across all children.
	}
	else if (Intersects(box, go))
	{
		if (objects.size() < OCTREE_MAX_ITEMS)	    // Limit of OCTREE_MAX_ITEMS for items that spread across all children.
			objects.push_back(go);
		else
		{
			if (childs[0] == nullptr)
				SubdivideNode();

			// Possible redistribution of objects on this node on splitting
			for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
			{
				if (!IntersectsAllChildren(go, this))
				{
					for (int i = 0; i < 8; ++i)
						childs[i]->Insert(*it);

					it = objects.erase(it);
				}
				else
				{
					++it;
				}
			}

			for (int i = 0; i < 8; ++i)
				childs[i]->Insert(go);
		}
	}
}

int OcTreeNode::CollectCandidates(std::vector<GameObject*> &nodes, const math::AABB& bbox) const
{
	int tests = 0;

	for (std::list<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it)
		nodes.push_back(*it);

	for (int i = 0; i < 8; ++i)
	{
		if(childs[i]->box.Intersects(bbox))
			tests += CollectCandidates(nodes, childs[i]->box);
		++tests;			
	}

	return tests;
}

void OcTreeNode::CollectRects(std::vector<OcTreeNode*> &nodes)
{
	nodes.push_back(this);

	for (int i = 0; i < 8; ++i)
		if (childs[i] != nullptr) childs[i]->CollectRects(nodes);
}

void OcTreeNode::SubdivideNode()
{
	math::vec half_size = (box.maxPoint - box.minPoint) / 2.0f;
	math::AABB initial_bbox = math::AABB(box.minPoint, { box.minPoint.x + half_size.x, box.minPoint.y + half_size.y, box.minPoint.z + half_size.z });

	uint index = 0;

	for (int dir_x = 0; dir_x < 2; ++dir_x)
	{
		for (int dir_y = 0; dir_y < 2; ++dir_y)
		{
			for (int dir_z = 0; dir_z < 2; ++dir_z)
			{
				math::AABB sub_cube = initial_bbox;
				sub_cube.Translate({ half_size.x * dir_x, half_size.y * dir_y, half_size.z * dir_z });
				childs[index++] = new OcTreeNode(sub_cube);
			}
		}
	}
}

// ---- OCTREE ----

OcTree::OcTree() : root(nullptr)
{}

OcTree::~OcTree()
{
	Clear();
}

void OcTree::SetBoundaries(const math::AABB &r)
{
	if (root != nullptr)
		delete root;

	root = new OcTreeNode(r);
}

void OcTree::Insert(GameObject* go)
{
	if (root != nullptr)
	{
		math::AABB bbox;

		if (go->GetAABB(bbox) && root->box.Intersects(bbox))
			root->Insert(go);
	}
}

void OcTree::Clear()
{
	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}
}

int OcTree::CollectCandidates(std::vector<GameObject*> &nodes, const math::AABB& bbox) const
{
	int tests = 1;
	if(root != nullptr && root->box.Intersects(bbox))
		tests += root->CollectCandidates(nodes, bbox);
	return tests;
}

void OcTree::CollectRects(std::vector<OcTreeNode*> &nodes) const
{
	if (root != nullptr)
		root->CollectRects(nodes);
}