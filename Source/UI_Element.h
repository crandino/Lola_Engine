#ifndef __UIELEMENTS__
#define __UIELEMENTS__

#define MAX_STRING_UI 10

#include "MathGeoLib\MathGeoLib.h"

#include "SDL\include\SDL_rect.h"
#include "SDL\include\SDL_render.h"

enum UI_TYPE
{
	UI_LABEL,
	UI_IMAGE,
	UI_BUTTON,
	UI_INPUTBOX,
	UI_HORIZONTALSCROLLBAR
};

struct StateImage
{
	SDL_Texture		*img;
	SDL_Rect		section;

	StateImage() { img = nullptr; }
};

class Module;

class UI_Element
{

private:
	
	math::float2			pos;
	int 					width, height;
	
public:

	const UI_Element*		parent;
	bool					interactable;
	bool					is_inside;
	bool					can_be_focused = false;
	UI_TYPE					type;
	std::vector<Module*>	mod_listeners;			// Listener's module list

	UI_Element() {}
	virtual ~UI_Element() {}

	virtual bool draw() { return true; }
	virtual bool preUpdate() { return true; }
	virtual void drawDebug();

	bool isMouseIn(const math::float2 &pos);
	virtual void dragElement();

	math::float2 getScreenPos() const;
	void getScreenRect(int &w, int &h) const;
	math::float2 getLocalPos() const;
	void setLocalPos(math::float2 local_pos);

	void addListener(Module *module);
	void setDimensions(const int &width, const int &height);
};

//#define CURSOR_WIDTH 2

//class UIinputBox : public UI_Element
//{
//
//public:
//	UI_Image			frame;
//	UI_Label			text;
//	math::float2	offset;
//	int				cursor_pos;
//	uint			cursor_index;
//	int				cursor_height;
//	int				cursor_width;
//
//	bool			active;
//	const char*		default_string;
//
//	UIinputBox();
//	~UIinputBox();
//
//	void init(math::float2 pos, math::float2 text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
//		_TTF_Font *font, Module *module, UIelement *parent);
//	bool draw();
//	bool preUpdate();
//	void drawDebug();
//
//	void moveCursor();
//};

//// EXERCISE 1 and 6
//class UIHorizontalScrollBar : public UIelement
//{
//
//public:
//
//	UIimage			bar;
//	UIimage			thumb;
//	math::float2	thumb_pos;
//	int				left_limit;
//	int				right_limit;
//
//	UIHorizontalScrollBar();
//	~UIHorizontalScrollBar();
//
//	void init(math::float2 pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
//			  SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent);
//	bool draw();
//	// EXERCISE 3
//	bool preUpdate();
//	void drawDebug();
//	// EXERCISE 2
//	void dragElement();
//	// EXERCISE 5
//	float calculateValue();
//	// EXERCISE 2 and 3
//	bool checkLimits(int new_x_pos);
//	
//};


#endif //__UIELEMENTS__
