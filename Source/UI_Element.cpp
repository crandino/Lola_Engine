//#include "Gui.h"
//#include "UIelements.h"
//#include "Point2d.h"
//#include "DList.h"
//#include "App.h"
//#include "Input.h"
//#include "Fonts.h"
//#include "Render.h"
//#include "p2Log.h"
//#include "SDL\include\SDL_rect.h"
//#include "SDL\include\SDL_keycode.h"

#include "UI_Element.h"

#include "Application.h"
#include "ModuleInput.h"

// ---- UI Element -----
bool UI_Element::isMouseIn(const math::float2 &p)
{
	math::float2 p_elem = getScreenPos();

	return (p.x >= p_elem.x &&
		p.x <= (p_elem.x + width) &&
		p.y >= p_elem.y &&
		p.y <= (p_elem.y + height));
}

math::float2 UI_Element::getScreenPos() const
{
	math::float2 p(0, 0);
	const UI_Element *item = this;
	while (item != NULL)
	{
		p += { item->pos.x, item->pos.y };
		item = item->parent;
	}
	return p;
}

void UI_Element::getScreenRect(int &w, int &h) const
{
	w = width;
	h = height;
}

void UI_Element::setLocalPos(math::float2 local_pos)
{
	pos.x = local_pos.x;
	pos.y = local_pos.y;
}

math::float2 UI_Element::getLocalPos() const
{
	return pos;
}

void UI_Element::addListener(Module *module)
{
	if (module != NULL)
		mod_listeners.push_back(module);
}

void UI_Element::setDimensions(const int &width, const int &height)
{
	this->width = width;
	this->height = height;
}

//GetLocalRect()

void UI_Element::drawDebug()
{
	math::float2 p = getScreenPos();
	/*SDL_Rect r = { p.x, p.y, width, height };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);*/
}

// DragElement:
void UI_Element::dragElement()
{
	math::float2 p(App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
	setLocalPos(pos + p);
}

//// ----- UIinputBox -----
//// Constructor:
//UIinputBox::UIinputBox()
//{ }
//
//// Destructor:
//UIinputBox::~UIinputBox()
//{
//	SDL_DestroyTexture(frame.image.img);
//	SDL_DestroyTexture(text.text_tex.img);
//}
//
//// Init:
//void UIinputBox::init(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
//	_TTF_Font *font, Module *module, UIelement *parent)
//{
//	setLocalPos(pos);
//	this->parent = parent != NULL ? parent : app->gui->screen;
//	// Listeners.
//	addListener(app->gui);
//	addListener(module);
//
//	frame.init({ 0, 0 }, frame_tex, frame_section, module, this);
//	text.init(text_offset, initial_text, font, module, this);
//	offset = text_offset;
//	default_string = initial_text;
//	cursor_pos = 0;
//	cursor_index = 0;
//	interactable = true;
//	is_inside = false;
//	active = false;
//	can_be_focused = true;
//	type = UI_INPUTBOX;
//	setDimensions(frame_section.w, frame_section.h);
//
//	// Cursor
//	cursor_width = CURSOR_WIDTH;
//	int dummy;
//	app->fonts->calcSize("X", dummy, cursor_height, font);
//}
//
//// Draw:
//bool UIinputBox::draw()
//{
//	iPoint p = getScreenPos();
//	// Frame
//	app->render->blit(frame.image.img, p.x, p.y, &frame.image.section);	
//	// Label
//	if(strcmp(text.text.GetString() ,"") != 0)		
//		app->render->blit(text.text_tex.img, p.x + offset.x, p.y + offset.y);	
//	// Cursor
//	if(active)							
//		app->render->DrawQuad({ p.x + offset.x + cursor_pos, p.y + offset.y, cursor_width, cursor_height }, 255, 255, 255);   
//	return true;
//}
//
//// preUpdate:
//bool UIinputBox::preUpdate()
//{
//	if (active)
//	{
//		uint length_string = text.text.Length();
//		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
//		{
//			if(cursor_index > 0) cursor_index--;
//			moveCursor();
//		}
//		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
//		{
//			if (cursor_index < length_string) cursor_index++;
//			moveCursor();
//		}
//	}
//
//	return true;
//}
//
//void UIinputBox::moveCursor()
//{
//	
//	if (cursor_index == 0)
//		cursor_pos = 0;
//	else
//	{
//		const char *t = text.text.GetString();
//		char c[MAX_STRING_UI + 1 ];
//		memset(c, '\0', MAX_STRING_UI + 1);
//
//		for (int i = 0; i < cursor_index; i++)
//			c[i] = t[i];
//
//		int dummy;
//		app->fonts->calcSize(c, cursor_pos, dummy, text.font);
//			
//	}
//}
//
//void UIinputBox::drawDebug()
//{
//	// Frame
//	iPoint p = frame.getScreenPos();
//	int w, h; frame.getScreenRect(w, h);
//	SDL_Rect r = { p.x, p.y, w,h };
//	app->render->DrawQuad(r, 255, 125, 0, 255, false);
//
//	// Label
//	p = text.getScreenPos();
//	text.getScreenRect(w, h);
//	r = { p.x, p.y, w, h };
//	app->render->DrawQuad(r, 255, 125, 0, 255, false);
//}
//
//// ----- UIHorizontalScrollBar -----
//
//UIHorizontalScrollBar::UIHorizontalScrollBar()
//{ }
//
//UIHorizontalScrollBar::~UIHorizontalScrollBar()
//{
//	SDL_DestroyTexture(bar.image.img);
//	SDL_DestroyTexture(thumb.image.img);
//}
//
//// EXERCISE 1 and 6
//void UIHorizontalScrollBar::init(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
//	SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent)
//{
//	setLocalPos(pos);
//	this->parent = parent != NULL ? parent : app->gui->screen;
//	// Listeners.
//	addListener(app->gui);
//	addListener(module);
//
//	bar.init({ 0, 0 }, bar_tex, section_bar, module, this);
//	thumb_pos.x = bar_offset;
//	thumb_pos.y = thumb_vert_offset;
//	thumb.init( thumb_pos, thumb_tex, section_thumb, module, this);
//	left_limit = bar_offset;
//	right_limit = section_bar.w - bar_offset;
//	
//	interactable = true;
//	can_be_focused = true;
//	type = UI_HORIZONTALSCROLLBAR;
//	setDimensions(section_bar.w, section_bar.h);
//
//}
//
//// Draw:
//bool UIHorizontalScrollBar::draw()
//{
//	iPoint p = getScreenPos();
//	// Bar
//	app->render->blit(bar.image.img, p.x, p.y, &bar.image.section);
//	// Thumb
//	app->render->blit(thumb.image.img, p.x + thumb_pos.x, p.y + thumb_pos.y, &thumb.image.section);
//	
//	return true;
//}
//
//// Draw:
//// EXERCISE 5
//float UIHorizontalScrollBar::calculateValue()
//{
//	return ((float)(thumb_pos.x - left_limit) / ((right_limit - thumb.image.section.w)  - left_limit));
//}
//
//// Draw:
//// EXERCISE 2
//void UIHorizontalScrollBar::dragElement()
//{
//	iPoint p = app->input->getMouseMotion();
//	int final_pos = thumb_pos.x + p.x;
//	if (checkLimits(final_pos))
//	{
//		thumb_pos.x = final_pos;
//		iPoint new_pos = { final_pos, thumb_pos.y };
//		thumb.setLocalPos(new_pos);
//
//		for (doubleNode<Module*> *item = mod_listeners.getLast(); item != NULL; item = item->previous)
//			item->data->onGui(DRAGGED, this);
//	}
//}
//
//// EXERCISE 2 and 3
//bool UIHorizontalScrollBar::checkLimits(int new_x_pos)
//{
//	return (new_x_pos >= left_limit && new_x_pos + thumb.image.section.w <= right_limit);
//}
//
//void UIHorizontalScrollBar::drawDebug()
//{
//	// Bar
//	iPoint p = bar.getScreenPos();
//	int w, h; bar.getScreenRect(w, h);
//	SDL_Rect r = { p.x, p.y, w, h };
//	app->render->DrawQuad(r, 255, 125, 0, 255, false);
//
//	// Thumb
//	p = thumb.getScreenPos();
//	thumb.getScreenRect(w, h);
//	r = { p.x, p.y, w, h };
//	app->render->DrawQuad(r, 255, 125, 0, 255, false);
//}
//
//// EXERCISE 3
//bool UIHorizontalScrollBar::preUpdate()
//{
//	if (this == app->gui->whichFocus())
//	{
//		int speed = 1;
//		iPoint new_pos;
//
//		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && checkLimits(thumb_pos.x - speed))
//		{
//			thumb_pos.x -= speed;
//			thumb.setLocalPos(thumb_pos);			
//		}
//		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && checkLimits(thumb_pos.x + speed))
//		{
//			thumb_pos.x += speed;
//			thumb.setLocalPos(thumb_pos);
//		}
//
//		for (doubleNode<Module*> *item = mod_listeners.getLast(); item != NULL; item = item->previous)
//			item->data->onGui(DRAGGED, this);
//	}
//
//	return true;
//}