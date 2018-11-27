#ifndef __UI_Element_H__
#define __UI_Element_H__


#include "p2Point.h"
#include "p2Animation.h"
#include "p2DynArray.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"

struct SDL_Texture;
enum UI_Elements
{
	NON_TYPE,

	BUTTON,
	CHECKBOX,
};


class UI_Element
{
public:

	UI_Element();
	virtual ~UI_Element();
	
	virtual bool Update(float dt) { return true; };
	virtual bool CleanUp();
	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual void Draw(float dt);
	bool IsMouseOn();
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };



public:
	p2SString sprite;
	iPoint position;
	int height;
	int weight;
	SDL_Rect png_pos = { 0,0,0,0 };
	UI_Elements type = UI_Elements::NON_TYPE;
	//
	pugi::xml_node		UI_node;

	SDL_Texture* texture;

	

	bool pressed = false;
	bool mouseOn = false;

	unsigned int fxOn;
	unsigned int fxPressed;

	p2SString FXON;

	//
	SDL_Rect NoPressedNoMouseOn;
	SDL_Rect MouseOn;
	SDL_Rect Pressed;
	bool repeataudio = true;
};

#endif

