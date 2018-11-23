#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2SString.h"
#include "j1Input.h"

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

bool UI_Element::PreUpdate()
{

	return true;
}

void UI_Element::Draw(float dt)
{
}
