#include "Label.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Fonts.h"


Label::Label(int x, int y, p2SString name, int size, bool CanBeMoved) : UI_Element (x,y)
{
	this->CanBeMoved = CanBeMoved;
	this->name = name;
	App->tex->UnLoad(atlas);
	if (!CanBeMoved) {
		Distance.x = position.x - (-App->render->camera.x);
		Distance.y = position.y - (-App->render->camera.y);
		App->fonts->PlayerUI = App->fonts->Load(App->fonts->path, size);
		atlas = App->fonts->Print(name.GetString(), App->fonts->PlayerUI);
		App->fonts->CalcSize(name.GetString(), width, height, App->fonts->PlayerUI);
	}
	else {
		App->fonts->InitialMenu = App->fonts->Load(App->fonts->path, size);
		atlas = App->fonts->Print(name.GetString(), App->fonts->InitialMenu);
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
		position.x = (-App->render->camera.x) + Distance.x;
		position.y = (-App->render->camera.y) + Distance.y;
	}

	return true;
}
