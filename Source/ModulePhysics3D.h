#ifndef __MODULEPHYSICS_H__
#define __MODULEPHYSICS_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "Bullet\include\btBulletDynamicsCommon.h"

#include <list>

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysBody3D;

class ModulePhysics3D : public Module
{
public:
	
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f, bool sensor = false);
	PhysBody3D* AddBody(const Cube& cube, Module* listener, float mass = 1.0f, bool sensor = false);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f, bool sensor = false);

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const math::vec& anchorA, const math::vec& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const math::vec& anchorA, const math::vec& anchorB, const math::vec& axisS, const math::vec& axisB, bool disable_collision = false);

private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	DebugDrawer*						debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<PhysBody3D*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;
};

class DebugDrawer : public btIDebugDraw
{
public:

	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};

#endif //!_MODULEPHYSICS_H__