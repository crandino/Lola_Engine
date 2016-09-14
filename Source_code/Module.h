#pragma once
#include "Globals.h"
#include <string>

class Application;
struct PhysBody3D;

class Module
{
private :

	bool enabled;
	
protected:
	
	std::string name;

public:

	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UPDATE_STATUS PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UPDATE_STATUS Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual UPDATE_STATUS PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}
};