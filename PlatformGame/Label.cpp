#include "Label.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Fonts.h"

Label::Label(int x, int y, p2SString name, bool CanBeMoved) : UI_Element (x,y)
{
	this->CanBeMoved = CanBeMoved;
	Distance.x = position.x - (-App->render->camera.x);
	Distance.y = position.y - (-App->render->camera.y);
	this->name = name;
	App->tex->UnLoad(atlas);
	atlas = App->fonts->Print(name.GetString());
	App->fonts->CalcSize(name.GetString(), width, height, App->fonts->default);
	
}

Label::~Label()
{
}

bool Label::Update(float dt)
{
	if (!CanBeMoved) {
		position.x = (-App->render->camera.x) + Distance.x;
		position.y = (-App->render->camera.y) + Distance.y;
	}

	return true;
}
