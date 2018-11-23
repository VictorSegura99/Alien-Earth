#ifndef __UI_Element_H__
#define __UI_Element_H__


#include "p2Point.h"
#include "p2Animation.h"
#include "p2DynArray.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"

struct SDL_Texture;
enum UI_Elements
{
	NON_TYPE,

	BUTTON,
};


class UI_Element
{
public:

	UI_Element();
	UI_Element(int x, int y);
	virtual ~UI_Element();
	

	
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual void Draw(float dt);
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };



public:

	SDL_Rect position;
	SDL_Rect png_pos = { 0,0,0,0 };
	UI_Elements type = UI_Elements::NON_TYPE;
	//

	Animation* current_animation = nullptr;
	SDL_Texture* texture;

	bool debug_draw = false;

	bool pressed = false;

};

#endif

