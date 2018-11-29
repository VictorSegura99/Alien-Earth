#include "Button.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Button::Button(int x, int y, int type, p2SString name, int size) : UI_Element(x, y)
{
	this->type = type;
	switch (type) {
	case 1: {
		NoPressedNoMouseOn = { 349,99,190,49 };
		MouseOn = { 349,47,190,49 };
		Pressed = { 349,1,190,45 };
		width = 190;
		height = 49;
		if (name != "NONE") {
			label = App->ui_manager->CreateLabel(position.x + width / 2, position.y + height / 2, name, size, true);
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
		fXOn = App->audio->LoadFx(FXON.GetString());
		break; }
	case 3: {
		this->type = 1;
		NoPressedNoMouseOn = { 559,0,39,31 };
		MouseOn = { 652,0,39,31 };
		Pressed = { 608,0,39,28 };
		width = 39;
		height = 31;
		break;
	}
	}

	png_pos = NoPressedNoMouseOn;
}

Button::~Button()
{
	
}

bool Button::Update(float dt)
{
	switch (type) {
	case 1: {
		if (WantToRender) {
			if (IsMouseOn()) {
				png_pos = MouseOn;
				mouseOn = true;
				if (repeataudio) {
					App->audio->PlayFx(fxOn);
					repeataudio = false;
				}
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
					App->audio->PlayFx(fxPressed);
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
		}
		if (!WantToRender) {
			if (label != nullptr)
				label->WantToRender = false;
		}
		if (WantToRender) {
			if (label != nullptr)
				label->WantToRender = true;
		}
		break; }
	case 2: {
		if (IsMouseOn()) {
			png_pos = MouseOn;
			mouseOn = true;
			if (repeataudio) {
				App->audio->PlayFx(fXOn);
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
		break; }
	}
	
	


	return true;
}




