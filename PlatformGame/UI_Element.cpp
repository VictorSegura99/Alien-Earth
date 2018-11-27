#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2SString.h"
#include "j1Input.h"
#include "UI_Manager.h"

UI_Element::UI_Element()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;


	config = App->LoadConfig(config_file);
	UI_node = config.child("UI");

	sprite = UI_node.child("sprite").text().as_string();

	texture = App->tex->Load(sprite.GetString());
}

UI_Element::~UI_Element()
{
}

bool UI_Element::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

void UI_Element::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y, &(png_pos));
	if (App->ui_manager->debug_draw) {
		App->render->DrawQuad({ position.x,position.y,weight,height }, 255, 0, 0, 255, false);
	}
}
bool UI_Element::IsMouseOn()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (mouse_pos.x >= position.x && position.x + weight >= mouse_pos.x && mouse_pos.y >= position.y && position.y + height >= mouse_pos.y) {
		return true;
	}
	return false;
}
