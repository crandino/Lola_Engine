#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "GameObject.h"

#include "MathGeoLib\MathGeoLib.h"

#include <list>

#define QUADTREE_MAX_ITEMS 2
#define NUM_CHILDS 4

// Helper function to check if one rectangle complately contains another
bool Contains(const GameObject *a, const GameObject *b);
bool Intersects(const GameObject *a, const GameObject *b);

bool Contains(const math::AABB &a, const GameObject *b);
bool Intersects(const math::AABB &a, const GameObject *b);

bool Contains(const GameObject *a, const math::AABB &b);
bool Intersects(const GameObject *a, const math::AABB &b);

// ----- TreeNode ----
class QuadTreeNode
{

public:

	math::AABB					rect;
	std::list<GameObject*>	    objects;
	//QuadTreeNode*				parent;
	QuadTreeNode*				childs[NUM_CHILDS];

public:

	QuadTreeNode(math::AABB r) : rect(r)
	{
		//parent = nullptr;
		for(int i = 0; i < NUM_CHILDS; ++i)
			 childs[i] = nullptr;
	}

	~QuadTreeNode()
	{
		for(int i = 0; i < 4; ++i)
			if(childs[i] != nullptr) delete childs[i];
	}
	 
	void Insert(GameObject* go)
	{
		math::AABB bounding_box;
		go->GetAABB(bounding_box);

		if (childs[0] != nullptr)
		{
			for (int i = 0; i < NUM_CHILDS; ++i)
				childs[i]->Insert(go);
		}			
		else if (Intersects(rect, go))
		{
			if (objects.size() < QUADTREE_MAX_ITEMS)
				objects.push_back(go);
			else
			{
				if (childs[0] == nullptr)
				{
					math::vec size = rect.maxPoint - rect.minPoint;
					math::AABB initial_bbox = math::AABB(rect.minPoint, { rect.minPoint.x + size.x / 2.0f, rect.maxPoint.y, rect.minPoint.z + size.z / 2.0f });

					childs[0] = new QuadTreeNode(initial_bbox);
					initial_bbox.Translate({ size.x / 2.0f, 0.0f, 0.0f });
					childs[1] = new QuadTreeNode(initial_bbox);
					initial_bbox.Translate({0.0f, 0.0f, size.z / 2.0f });
					childs[2] = new QuadTreeNode(initial_bbox);
					initial_bbox.Translate({ -size.x / 2.0f, 0.0f, 0.0f });
					childs[3] = new QuadTreeNode(initial_bbox);
				}

				// Possible redistribution of objects on this node on splitting
				for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
				{
					if (!(Intersects((*it), childs[0]->rect) && Intersects((*it), childs[1]->rect) && Intersects((*it), childs[2]->rect) &&	Intersects((*it), childs[3]->rect)))
					{
						for (int i = 0; i < NUM_CHILDS; ++i)
							childs[i]->Insert(*it);		

						it = objects.erase(it);
					}
					else
					{
						++it;
					}											
				}

				for (int i = 0; i < NUM_CHILDS; ++i)
					childs[i]->Insert(go);
			}
		}

		// TODO: Insertar un nou Collider al quadtree
		// En principi cada node pot enmagatzemar QUADTREE_MAX_ITEMS nodes (encara que podrien ser més)
		// Si es detecten més, el node s'ha de tallar en quatre
		// Si es talla, a de redistribuir tots els seus colliders pels nous nodes (childs) sempre que pugui
		// Nota: un Collider pot estar a més de un node del quadtree
		// Nota: si un Collider intersecciona als quatre childs, deixar-lo al pare
	}

	//int CollectCandidates(DynArray<Collider*>& nodes, const SDL_Rect& r) const
	//{
	//	// TODO: Omplir el array "nodes" amb tots els colliders candidats
	//	// de fer intersecció amb el rectangle r
	//	// retornar el número de intersección calculades en el procés
	//	// Nota: és una funció recursiva
	//	return 0;
	//}

	void CollectRects(std::vector<QuadTreeNode*> &nodes) 
	{
		nodes.push_back(this);

		for(int i = 0; i < 4; ++i)
			if(childs[i] != nullptr) childs[i]->CollectRects(nodes);
	}

};

// Tree class -------------------------------------------------------
class QuadTree
{
public:

	QuadTreeNode*	root;

	// Constructor
	QuadTree() : root(NULL)
	{}

	// Destructor
	virtual ~QuadTree()
	{
		Clear();
	}

	void SetBoundaries(const math::AABB &r)
	{
		if(root != nullptr)
			delete root;

		root = new QuadTreeNode(r);
	}

	void Insert(GameObject* go)
	{
		if(root != NULL)
		{
			math::AABB bbox;
			
			if(go->GetAABB(bbox) && root->rect.Intersects(bbox))
				root->Insert(go);
		}
	}

	void Clear()
	{
		if(root != NULL)
		{
			delete root;
			root = NULL;
		}
	}

	/*int CollectCandidates(DynArray<Collider*>& nodes, const SDL_Rect& r) const
	{
		int tests = 1;
		if(root != NULL && Intersects(root->rect, r))
			tests = root->CollectCandidates(nodes, r);
		return tests;
	}*/

	void CollectRects(std::vector<QuadTreeNode*>& nodes) const
	{
		if(root != nullptr)
			root->CollectRects(nodes);
	}
};

#endif // __QUADTREE_H__