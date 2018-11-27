#include "CheckBox.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

CheckBox::CheckBox(int x, int y)
{
	position.x = x;
	position.y = y;
	weight = 40;
	height = 39;

	NoPressedNoMouseOn = { 283,0,40,39 };
	Pressed = { 243,0,40,39 };

	png_pos = NoPressedNoMouseOn;
}

CheckBox::~CheckBox()
{
}

bool CheckBox::Update(float dt)
{
	
	if (IsMouseOn()) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			png_pos = Pressed;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			pressed = !pressed;
		}
	}
	if (pressed)
		png_pos = Pressed;
	else png_pos = NoPressedNoMouseOn;


	return true;
}


