#include "Label.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Fonts.h"


Label::Label(int x, int y, p2SString name, int size, bool CanBeMoved, UI_Element* parent) : UI_Element (x,y, parent)
{
	this->CanBeMoved = CanBeMoved;
	this->name = name;
	if (!CanBeMoved) {
		Distance.x = Local_pos.x - (-App->render->camera.x);
		Distance.y = Local_pos.y - (-App->render->camera.y);
		App->fonts->PlayerUI = App->fonts->Load(App->fonts->path, size);
		tex = App->fonts->Print(name.GetString(), App->fonts->PlayerUI);
		App->fonts->CalcSize(name.GetString(), width, height, App->fonts->PlayerUI);
	}
	else {
		App->fonts->InitialMenu = App->fonts->Load(App->fonts->path, size);
		tex = App->fonts->Print(name.GetString(), App->fonts->InitialMenu);
		App->fonts->CalcSize(name.GetString(), width, height, App->fonts->InitialMenu);
	}

}

Label::~Label()
{
	//TTF_CloseFont(App->fonts->InitialMenu);
}

bool Label::Update(float dt)
{
	if (!CanBeMoved) {
		Local_pos.x = (-App->render->camera.x) + Distance.x;
		Local_pos.y = (-App->render->camera.y) + Distance.y;
	}

	return true;
}
