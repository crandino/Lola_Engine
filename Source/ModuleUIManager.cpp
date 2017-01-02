#include "ModuleUIManager.h"

#include "Application.h"
#include "ModuleInput.h"

#include "UI_Element.h"
#include "UI_Image.h"
#include "UI_Label.h"
#include "UI_Button.h"

#include "Component.h"

#include "SDL\include\SDL_render.h"

ModuleUIManager::ModuleUIManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "UI_Manager");
	//debug = false;
}

// Destructor
ModuleUIManager::~ModuleUIManager()
{}

// Called before render is available
bool ModuleUIManager::Awake(JSONParser &config)
{
	DEBUG("Loading GUI atlas");
	bool ret = true;

	//atlas_file_name = conf.child("atlas").attribute("file").as_string("");*/

	return ret;
}

// Called before the first frame
bool ModuleUIManager::Start()
{
	//atlas = app->tex->loadTexture(atlas_file_name.GetString());

	canvas = App->gameobject_manager->CreateGameObject("Canvas", nullptr);
	canvas->AddComponent(COMPONENT_TYPE::TRANSFORM_2D);

	screen = new UI_Element();
	screen->interactable = false;
	focus = screen;
	previous_UIelement = screen;

	return true;
}

// Update all guis
bool ModuleUIManager::PreUpdate()
{
	math::float2 p = math::float2(App->input->GetMouseX(), App->input->GetMouseY());
	current_UIelement = whichUIelemOnMouse();

	std::vector<Module*>::iterator it = current_UIelement->mod_listeners.end();
	while (it != current_UIelement->mod_listeners.begin())
	{
		if (current_UIelement != previous_UIelement)
			(*it)->OnGui(MOUSE_LEAVE, previous_UIelement);

		if (current_UIelement->interactable)
		{
			if (!current_UIelement->is_inside && current_UIelement->isMouseIn(p))
				(*it)->OnGui(MOUSE_ENTER, current_UIelement);

			if (App->input->GetMouseButton(0) == KEY_REPEAT)
				(*it)->OnGui(MOUSE_REPEAT_LEFT, current_UIelement);

			// Behaviour for left mouse button
			if (App->input->GetMouseButton(0) == KEY_DOWN)
			{
				(*it)->OnGui(MOUSE_CLICK_LEFT, current_UIelement);
				if (focus != current_UIelement && current_UIelement->can_be_focused)
				{
					(*it)->OnGui(LOST_FOCUS, focus);
					(*it)->OnGui(GAIN_FOCUS, current_UIelement);
				}
			}
			else if (App->input->GetMouseButton(0) == KEY_UP)
				(*it)->OnGui(MOUSE_LEAVE_LEFT, current_UIelement);

			// Behaviour for right mouse button
			if (App->input->GetMouseButton(1) == KEY_DOWN)
				(*it)->OnGui(MOUSE_CLICK_RIGHT, current_UIelement);
			else if (App->input->GetMouseButton(1) == KEY_UP)
				(*it)->OnGui(MOUSE_LEAVE_RIGHT, current_UIelement);
		}

		--it;  // Previous listener
	}

	for (std::vector<UI_Element*>::iterator it = UIelement_list.begin(); it != UIelement_list.end(); ++it)
		(*it)->preUpdate();
	
	previous_UIelement = current_UIelement;
	return true;
}

// Called after all Updates
bool ModuleUIManager::PostUpdate()
{
	/*if (app->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;*/

	for (std::vector<UI_Element*>::iterator it = UIelement_list.begin(); it != UIelement_list.end(); ++it)
	{
		/*if (debug)
			item->data->drawDebug();
		item->data->draw();*/
	}
	return true;
}

// Called before quitting
bool ModuleUIManager::CleanUp()
{
	DEBUG("Freeing GUI");

	for (std::vector<UI_Element*>::iterator it = UIelement_list.begin(); it != UIelement_list.end(); ++it)
		delete (*it);
	
	UIelement_list.clear();

	// Deleting screen root for tree
	delete screen;

	return true;
}

// const getter for atlas
const SDL_Texture* ModuleUIManager::getAtlas() const
{
	return atlas;
}

const UI_Element* ModuleUIManager::whichFocus() const
{
	return focus;
}

// Factories for class Gui ---------------------------------------------------
UI_Label* ModuleUIManager::CreateLabel(math::float2 p, const char *string, _TTF_Font *font, Module *mod, UI_Element *parent)
{
	UI_Label *l = new UI_Label();
	//l->init(p, string, font, mod, parent);
	UIelement_list.push_back(l);
	return l;
}

UI_Image* ModuleUIManager::CreateImage(math::float2 p, SDL_Texture *tex, SDL_Rect &section, Module *mod, UI_Element *parent)
{
	UI_Image *i = new UI_Image();
	//i->init(p, tex, section, mod, parent);
	UIelement_list.push_back(i);
	return i;
}

UI_Button *ModuleUIManager::CreateButton(math::float2 p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
								  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
								  Module *mod, UI_Element *parent)
{
	UI_Button *b = new UI_Button();
	//b->init(p, tex_idle, section_idle, tex_hover, section_hover, tex_clicked, section_clicked, mod, parent);
	UIelement_list.push_back(b);

	return b;	
}

//UIinputBox *Gui::createInputBox(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
//									  _TTF_Font *font, Module *module, UIelement *parent)
//{
//	UIinputBox *i = new UIinputBox();
//	i->init(pos, text_offset, frame_tex, frame_section, initial_text, font, module, parent);
//	UIelement_list.add(i);
//	return i;
//}
//
//UIHorizontalScrollBar *Gui::createHorizontalScrollBar(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
//	SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent)
//{
//	UIHorizontalScrollBar *h = new UIHorizontalScrollBar();
//	h->init(pos, bar_offset, thumb_vert_offset, bar_tex, section_bar, thumb_tex, section_thumb, module, parent);
//	UIelement_list.add(h);
//	return h;
//}

void ModuleUIManager::OnGui(GUI_EVENTS mouse_event, UI_Element *trigger)
{
	switch (trigger->type)
	{
	case UI_BUTTON:
	{
		UI_Button *b = (UI_Button*)trigger;
		switch (mouse_event)
		{
		case MOUSE_ENTER:
			b->setHoverState();
			b->is_inside = true;	
			break;
		case MOUSE_LEAVE:
			b->setIdleState();
			b->is_inside = false;
			break;
		case MOUSE_CLICK_LEFT:
		case MOUSE_CLICK_RIGHT:
			b->setClickedState();
			break;
		case MOUSE_REPEAT_LEFT:
			b->dragElement();
			break;
		case MOUSE_LEAVE_LEFT:
		case MOUSE_LEAVE_RIGHT:
			b->setHoverState();
			break;
		case GAIN_FOCUS:
			if (focus != trigger)
			{
				focus = trigger;
				//app->input->stopTextInput();
			}
			break;
		}
		break;
	}

	case UI_IMAGE:
	{
		UI_Image *i = (UI_Image*)trigger;
		switch (mouse_event)
		{
		case MOUSE_REPEAT_LEFT:
			i->dragElement();
			break;
		}
		break;
	}

	/*case UI_INPUTBOX:
	{
		UIinputBox *i = (UIinputBox*)trigger;
		switch (mouse_event)
		{
		case GAIN_FOCUS:
			if (focus != trigger)
			{
				focus = trigger;
				app->input->startTextInput(i);

				if (!i->active && strcmp(i->text.text.GetString(), i->default_string) == 0)
					i->text.setText("");
				i->active = true;
			}
			break;
		case LOST_FOCUS:
				i->active = false;
			break;
		case MOUSE_REPEAT_LEFT:
			i->dragElement();
			break;
		case TEXT_CHANGED:
			i->moveCursor();
			break;
		}
		break;
	}*/

	//// EXERCISE 2
	//case UI_HORIZONTALSCROLLBAR:
	//{
	//	UIHorizontalScrollBar *h = (UIHorizontalScrollBar*)trigger;
	//	switch (mouse_event)
	//	{
	//	case GAIN_FOCUS:
	//		if (focus != trigger)
	//			focus = trigger;
	//		break;
	//	case MOUSE_REPEAT_LEFT:
	//		iPoint p;
	//		app->input->getMousePosition(p);
	//		if (focus == trigger && h->thumb.isMouseIn(p))
	//			h->dragElement();
	//		break;
	//	}
	//	break;
	//}
	}
}

UI_Element *ModuleUIManager::whichUIelemOnMouse() const
{
	math::float2 p = math::float2(App->input->GetMouseX(), App->input->GetMouseY());
	
	std::vector<UI_Element*>::const_iterator it = UIelement_list.end();
	while (it != UIelement_list.begin())
	{
		if ((*it)->isMouseIn(p) && (*it)->interactable)
			return (*it);
		--it;
	}

	return screen;
}
