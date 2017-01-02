#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI_element.h"

class UI_Image : public UI_Element
{

public:

	StateImage		image;

	UI_Image();
	~UI_Image();

	//void init(math::float2 pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UI_Element *parent);
	bool draw();

};

#endif // !__UI_IMAGE_H__
