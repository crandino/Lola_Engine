#ifndef __MODULEFONTMANAGER_H__
#define __MODULEFONTMANAGER_H__

#include "Module.h"

// FreeType Headers
#include "FreeType2\include\ft2build.h"
#include "FreeType2\include\freetype\freetype.h"

//#include "FreeType\freetype2\freetype\ftglyph.h"
//#include "FreeType\freetype2\freetype\ftoutln.h"
//#include "FreeType\freetype2\freetype\fttrigon.h"

#pragma comment (lib, "Source/FreeType2/libx86/freetype.lib")

class ModuleFontManager : public Module
{
public:

	ModuleFontManager(Application* app, bool start_enabled = true);
	~ModuleFontManager();

	bool Awake(JSONParser &config);

	bool Start();

	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	bool CleanUp();

private:

};

#endif // !__MODULEFONTMANAGER_H__
