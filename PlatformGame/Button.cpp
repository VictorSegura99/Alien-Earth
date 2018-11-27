#include "Button.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Button::Button(int x, int y, int type)
{
	position.x = x;
	position.y = y;
	
	switch (type) {
	case 1: {
		NoPressedNoMouseOn = { 215,712,70,39 };
		MouseOn = { 463,712,70,39 };
		Pressed = { 702,711,70,39 };
		weight = 70;
		height = 39;
		break; }
	case 2: { //button choose player
		NoPressedNoMouseOn = { 0,0,0,0 };
		MouseOn = { 0,0,225,441 };
		Pressed = { 0,0,225,441 };
		weight = 225;
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

	if (App->ui_manager->debug_draw) {
		App->render->DrawQuad({ position.x,position.y,weight,height }, 0, 0, 0, 255, false);
	}
	Draw(dt);
	return true;
}




