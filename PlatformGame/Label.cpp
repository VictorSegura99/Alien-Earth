#include "Label.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Fonts.h"

Label::Label(int x, int y, p2SString name)
{
	Distance.x = position.x - (-App->render->camera.x);
	Distance.y = position.y - (-App->render->camera.y);
	weight = 100;
	height = 100;
	position.x = x;
	position.y = y;
	this->name = name;
	App->tex->UnLoad(atlas);
	atlas = App->fonts->Print(name.GetString());
	
	
}

Label::~Label()
{
}

bool Label::Update(float dt)
{

	position.x = (-App->render->camera.x) + Distance.x;
	position.y = (-App->render->camera.y) + Distance.y;


	return true;
}
