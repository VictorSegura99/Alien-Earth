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

	NoPressedNoMouseOn = { IDLE };
	MouseOn = { MOUSEON };
	Pressed = { PRESSED };
	width = WIDTH;
	height = HEIGHT;
	if (name != "NONE") {
		label = App->ui_manager->CreateLabel(position.x + width / 2, position.y + height / 2, name, size, true);
		label->position.x -= label->width / 2;
		label->position.y -= label->height / 2;
		label_pos = label->position.y;
	}
	png_pos = NoPressedNoMouseOn;
	
}

Button::~Button()
{
	
}

bool Button::Update(float dt)
{
	if (label != nullptr)
		label->position.y = label_pos;
	pressed = false;
	if (!NoUse) {
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
				if (label != nullptr)
					label->position.y = label_pos + 5;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				pressed = true; 
				
			}
		}
		else {
			pressed = false;
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
	if (NoRenderLabel) {
		if (label != nullptr)
			label->WantToRender = false;
	}
	return true;
}




