#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2DynArray.h"
#include "p2List.h"


class UI_Element;


class UI_Manager : public j1Module
{
public:

	UI_Manager();
	~UI_Manager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	void Draw(float dt);
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	UI_Element* CreateButton(int x, int y, int type);
	UI_Element* CreateCheckBox(int x, int y);
	UI_Element* CreateImage(int x, int y);
	
	void DeleteButtons();
	void DeleteCheckBoxes();


private:

	p2DynArray<UI_Element*> elements;

public:

	bool debug_draw = false;
};

#endif
