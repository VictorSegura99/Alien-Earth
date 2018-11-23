#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2DynArray.h"

enum UI_Elements
{
	BUTTON,


};

class UI_Element;


class UI_Manager : public j1Module
{
public:

	UI_Manager();
	~UI_Manager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool CleanUp();

	

	


private:

	p2DynArray<UI_Element*> elements;

public:


};

#endif
