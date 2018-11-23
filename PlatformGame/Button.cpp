#include "Button.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1Input.h"

Button::Button(int x, int y, int weight, int height, int type)
{
	position.x = x;
	position.y = y;
	this->weight = weight;
	this->height = height;

	switch (type) {
	case 1: {
		NoPressedNoMouseOn = { 215,712,70,39 };
		MouseOn = { 463,712,70,39 };
		Pressed = { 702,711,70,39 };
		break; }
	case 2: {
		NoPressedNoMouseOn = { 0,0,0,0 };
		MouseOn = { 0,0,weight,height };
		Pressed = { 0,0,weight,height };
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

void Button::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y, &(png_pos));
}

bool Button::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

bool Button::IsMouseOn()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (mouse_pos.x >= position.x && position.x + weight >= mouse_pos.x && mouse_pos.y >= position.y && position.y + height >= mouse_pos.y) {
		return true;
	}
	return false;
}
