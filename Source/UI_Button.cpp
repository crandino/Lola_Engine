#include "UI_Button.h"

// Constructor:
UI_Button::UI_Button()
{ }

// Destructor:
UI_Button::~UI_Button()
{
	/*SDL_DestroyTexture(idle.img);
	SDL_DestroyTexture(hover.img);
	SDL_DestroyTexture(clicked.img);*/
}

// Draw:
//void UI_Button::init(math::float2 pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover,
//	SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UI_Element *parent)
//{
//	setLocalPos(pos);
//	//this->parent = parent != NULL ? parent : app->gui->screen;
//	// Listeners.
//	addListener(app->gui);
//	addListener(module);
//
//	interactable = true;
//	is_inside = false;
//	can_be_focused = true;
//	type = UI_BUTTON;
//	idle.img = tex_idle != NULL ? tex_idle : (SDL_Texture*)app->gui->getAtlas();
//	hover.img = tex_hover != NULL ? tex_hover : (SDL_Texture*)app->gui->getAtlas();
//	clicked.img = tex_clicked != NULL ? tex_clicked : (SDL_Texture*)app->gui->getAtlas();
//	idle.section = section_idle;
//	hover.section = section_hover;
//	clicked.section = section_clicked;
//	current_state = &idle;
//	setDimensions(idle.section.w, idle.section.h);
//}

// Draw:
bool UI_Button::draw()
{
	//math::float2 p(App->input->GetMouseX(), App->input->GetMouseY());
	//app->render->blit(current_state->img, p.x, p.y, &current_state->section);
	return true;
}

// State Button methods:
void UI_Button::setIdleState()
{
	current_state = &idle;
}

void UI_Button::setHoverState()
{
	current_state = &hover;
}

void UI_Button::setClickedState()
{
	current_state = &clicked;
}
