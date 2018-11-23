#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Button : public UI_Element {
public:

	Button(SDL_Rect pos);
	virtual ~Button();
	bool PreUpdate();
	bool PostUpdate();
	bool Update(float dt);
	void Draw(float dt);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	bool CleanUp();
	



public:
	
	
};

#endif
