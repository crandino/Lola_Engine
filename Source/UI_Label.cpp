#include "UI_Label.h"

// ---- UI Label -----
// Constructor:
UI_Label::UI_Label()
{ }

// Destructor:
UI_Label::~UI_Label()
{
	//SDL_DestroyTexture(text_tex.img);
}

//void UI_Label::Init(iPoint pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent)
//{
//	setLocalPos(pos);
//	this->parent = parent != NULL ? parent : app->gui->screen;
//	// Listeners.
//	addListener(app->gui);
//	addListener(module);
//
//	interactable = false;
//	is_inside = false;
//	type = UI_LABEL;
//	text.create(string);
//	font = f != NULL ? f : app->fonts->default;
//	SDL_Color white = { 255, 255, 255, 255 };
//	text_tex.img = app->fonts->print(text.GetString(), white, font);
//	text_tex.section.x = 0;
//	text_tex.section.y = 0;
//	app->fonts->calcSize(text.GetString(), text_tex.section.w, text_tex.section.h, font);
//	setDimensions(text_tex.section.w, text_tex.section.h);
//}

// Draw:
bool UI_Label::Draw()
{
	//math::float2 p(App->input->GetMouseX(), App->input->GetMouseY());
	//app->render->blit(text_tex.img, p.x, p.y, &text_tex.section);
	return true;
}

void UI_Label::SetText(const char *t)
{
	/*text.create(t);
	SDL_DestroyTexture(text_tex.img);
	text_tex.img = app->fonts->print(text.GetString(), { 255, 255, 255, 255 }, font);*/
}