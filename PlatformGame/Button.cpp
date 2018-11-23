#include "Button.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Input.h"

Button::Button(int x, int y, int weight, int height)
{
	position.x = x;
	position.y = y;
	this->weight = weight;
	this->height = height;
	texture = App->tex->Load("textures/Choose.png");
	NoPressedNoMouseOn = { 215,712,70,39 };
	MouseOn = { 463,712,70,39 };
	Pressed = { 702,711,70,39 };

	png_pos = NoPressedNoMouseOn;
}

Button::~Button()
{
}

bool Button::Update(float dt)
{
	if (IsMouseOn()) {
		png_pos = MouseOn;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			png_pos = Pressed;
			pressed = true;
		}
	}
	else {
		png_pos = NoPressedNoMouseOn;
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
