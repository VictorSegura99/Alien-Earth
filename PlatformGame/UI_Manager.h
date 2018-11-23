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

	UI_Element* CreateButton(int x, int y, int weight, int height, int type);

	void DeleteButtons();


private:

	p2DynArray<UI_Element*> elements;

public:


};

#endif
