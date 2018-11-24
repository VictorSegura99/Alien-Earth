#include "CheckBox.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

CheckBox::CheckBox(int x, int y, int weight, int height)
{
	position.x = x;
	position.y = y;
	this->weight = weight;
	this->height = height;

	NoPressedNoMouseOn = { 215,712,70,39 };
	Pressed = { 702,711,70,39 };

	png_pos = NoPressedNoMouseOn;
}

CheckBox::~CheckBox()
{
}

bool CheckBox::Update(float dt)
{
	
	if (IsMouseOn()) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			pressed = !pressed;
		}
	}
	if (pressed)
		png_pos = Pressed;
	else png_pos = NoPressedNoMouseOn;
	if (App->ui_manager->debug_draw) {
		App->render->DrawQuad({ position.x,position.y,weight,height }, 0, 0, 0, 255, false);
	}

	return true;
}

void CheckBox::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y, &(png_pos));
}

bool CheckBox::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
bool CheckBox::IsMouseOn()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (mouse_pos.x >= position.x && position.x + weight >= mouse_pos.x && mouse_pos.y >= position.y && position.y + height >= mouse_pos.y) {
		return true;
	}
	return false;
}
