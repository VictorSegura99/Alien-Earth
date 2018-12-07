#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2SString.h"
#include "p2DynArray.h"
#include "p2List.h"


class UI_Element;
struct SDL_Texture;

class UI_Manager : public j1Module
{
public:

	UI_Manager();
	~UI_Manager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	void Draw(float dt, SDL_Texture* texture);
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	UI_Element* CreateButton(int x, int y, int type, UI_Element* parent = nullptr, p2SString name = "NONE", int size = 0);
	UI_Element* CreateCheckBox(int x, int y, UI_Element* parent = nullptr);
	UI_Element* CreateImage(int x, int y, bool WantToBeMoved, UI_Element* parent = nullptr);
	UI_Element* CreateLabel(int x, int y, p2SString name, int size, bool CanBeMoved, UI_Element* parent = nullptr);
	UI_Element* CreateSlider(int x, int y, int SliderPos, UI_Element* parent = nullptr);
	
	void DeleteButtons();
	void DeleteCheckBoxes();
	void DeleteImages();
	void DeleteLabels();
	void DeleteSliders();
	void DeleteAllUI();
	bool DeleteUI_Element(UI_Element* element);
	void DeleteAllUIExeptPlayer();

private:

	p2DynArray<UI_Element*> elements;
	SDL_Texture* atlas = nullptr;
	p2SString sprite;

public:

	bool debug_draw = false;
	
};

#endif
