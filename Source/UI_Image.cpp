
#include "UI_Image.h"

UI_Image::UI_Image() { }

UI_Image::~UI_Image() 
{
	SDL_DestroyTexture(image.img);
}

// Init:
//void UI_Image::init(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent)
//{
//	setLocalPos(pos);
//	this->parent = parent != NULL ? parent : app->gui->screen;
//	// Listeners.
//	addListener(app->gui);
//	addListener(module);
//
//	interactable = false;
//	is_inside = false;
//	type = UI_IMAGE;
//	image.img = tex != NULL ? tex : (SDL_Texture*)app->gui->getAtlas();
//	image.section = section;
//	setDimensions(section.w, section.h);
//}

// Draw:
bool UI_Image::draw()
{
	//iPoint p = getScreenPos();
	//app->render->blit(image.img, p.x, p.y, &image.section);
	return true;
}