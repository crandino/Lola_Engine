#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI_Element.h"

class UI_Button : public UI_Element
{

public:

	StateImage		idle;
	StateImage		hover;
	StateImage		clicked;
	StateImage      *current_state;

	UI_Button();
	~UI_Button();

	//void init(math::float2 pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover,
		//SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UI_Element *parent);
	bool draw();

	void setIdleState();
	void setHoverState();
	void setClickedState();
};

#endif // !__UI_BUTTON_H__
