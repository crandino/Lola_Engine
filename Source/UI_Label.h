#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "UI_Element.h"

class UI_Label : public UI_Element
{
public:

	//p2SString		text;
	StateImage		text_tex;
	//_TTF_Font		*font;

	UI_Label();
	~UI_Label();

	//void Init(math::float2 pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent);
	bool Draw();
	void SetText(const char *t);
};

#endif __UI_LABEL_H__

