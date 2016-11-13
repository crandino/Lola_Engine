#ifndef __MODULEGAMEOBJECTMANAGER_H__
#define __MODULEGAMEOBJECTMANAGER_H__

#include "Module.h"
#include "MathGeoLib\MathGeoLib.h"
#include "DrawDebug.h"

#include "OcTree.h"
#include <vector>

class GameObject;
class ComponentCamera;

class ModuleGameObjectManager : public Module
{
public:

	std::vector<math::LineSegment> ray_casts;

	ModuleGameObjectManager(Application* app, bool start_enabled = true);
	~ModuleGameObjectManager();

	bool Awake(JSONParser &config);
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	const GameObject *GetRoot() const;

	GameObject *CreateGameObject(const char *name, GameObject *parent = nullptr);
	void CreateRoot();

	void CreateCamera();
	bool DeleteGameObject(GameObject *go_to_delete);
	void MarkChildToDelete(GameObject *go);

	void GenerateUUID(GameObject *go);
	void GenerateUUID(Component *comp);

	void SetEditorCamera(const ComponentCamera *comp_cam);
	void RayCast(const math::LineSegment &ray_cast);

	void UpdateOcTree();

private:

	OcTree								oc_tree;
	math::AABB							oc_tree_boundaries;

	GameObject							*fake_camera;

	DrawDebug							draw_debug;

	math::LCG							UUID_generator;

	GameObject							*root = nullptr;
	std::vector<GameObject*>			list_of_gos;
	std::vector<GameObject*>		    list_of_gos_to_draw;

	GameObject	*GetGameObject(long unsigned int UUID_to_search) const;

	bool DeleteGameObject(long unsigned int UUID_to_delete);

	int FrustumCulling(const math::Frustum &frustum);

	bool Save(JSONParser &module);
	bool Load(JSONParser &module);
	
};

#endif // !__MODULEGAMEOBJECTMANAGER_H__


