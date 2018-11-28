#include "Button.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Button::Button(int x, int y, int type, p2SString name) : UI_Element(x, y)
{
	
	switch (type) {
	case 1: {
		NoPressedNoMouseOn = { 215,712,70,39 };
		MouseOn = { 463,712,70,39 };
		Pressed = { 702,711,70,39 };
		width = 70;
		height = 39;
		if (name != "NONE") {
			label = App->ui_manager->CreateLabel(position.x + width / 2, position.y + height / 2, name, true);
			label->position.x -= label->width / 2;
			label->position.y -= label->height / 2;
		}
		break; }
	case 2: { //button choose player
		NoPressedNoMouseOn = { 0,0,0,0 };
		MouseOn = { 0,0,225,441 };
		Pressed = { 0,0,225,441 };
		width = 225;
		height = 441;
		FXON = UI_node.child("button").child("ChooseFx").text().as_string();
		fxOn = App->audio->LoadFx(FXON.GetString());
		break; }
	}

	png_pos = NoPressedNoMouseOn;
}

Button::~Button()
{
	
}

bool Button::Update(float dt)
{
	if (IsMouseOn()) {
		png_pos = MouseOn;
		mouseOn = true;
		if (repeataudio) {
			App->audio->PlayFx(fxOn);
			repeataudio = false;
		}
		
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			png_pos = Pressed;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			pressed = true;
		}
	}
	else {
		repeataudio = true;
		png_pos = NoPressedNoMouseOn;
		mouseOn = false;
	}

	


	return true;
}




