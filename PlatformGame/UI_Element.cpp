#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2SString.h"
#include "j1Input.h"
#include "UI_Manager.h"
#include "j1Audio.h"

UI_Element::UI_Element()
{
}

UI_Element::UI_Element(int x, int y) : position(x,y)
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;


	config = App->LoadConfig(config_file);
	UI_node = config.child("UI");

	sprite = UI_node.child("sprite").text().as_string();
	FXON = UI_node.child("FXON").text().as_string();
	FXPRESSED = UI_node.child("FXPRESSED").text().as_string();
	fxOn = App->audio->LoadFx(FXON.GetString());
	fxPressed = App->audio->LoadFx(FXPRESSED.GetString());
	
	atlas = App->tex->Load(sprite.GetString());
}

UI_Element::~UI_Element()
{
}

bool UI_Element::Update(float dt)
{



	return true;
}

bool UI_Element::CleanUp()
{
	App->tex->UnLoad(atlas);
	return true;
}

void UI_Element::Draw(float dt)
{
	if (WantToRender) {
		if (type != LABEL)
			App->render->Blit(atlas, position.x, position.y, &(png_pos));
		else {
			App->render->Blit(atlas, position.x, position.y, NULL);
		}

		if (App->ui_manager->debug_draw) {
			App->render->DrawQuad({ position.x,position.y,width,height }, 255, 0, 0, 255, false);
		}
	}
	
}
bool UI_Element::IsMouseOn()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (mouse_pos.x >= position.x && position.x + width >= mouse_pos.x && mouse_pos.y >= position.y && position.y + height >= mouse_pos.y) {
		return true;
	}
	return false;
}

void UI_Element::SetSpritesData(SDL_Rect Idle, SDL_Rect Hover, SDL_Rect Pressed)
{

	NoPressedNoMouseOn = Idle;
	MouseOn = Hover;
	this->Pressed=Pressed;
	width = Hover.w;
	height = Hover.h;

}
