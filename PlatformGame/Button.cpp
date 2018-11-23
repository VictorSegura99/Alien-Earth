#include "Button.h"

Button::Button(SDL_Rect pos)
{



}

Button::~Button()
{
}

bool Button::PreUpdate()
{
	return true;
}

bool Button::PostUpdate()
{
	return true;
}

bool Button::Update(float dt)
{
	return true;
}

void Button::Draw(float dt)
{
}

bool Button::Load(pugi::xml_node &)
{
	return false;
}

bool Button::Save(pugi::xml_node &) const
{
	return true;
}

bool Button::CleanUp()
{
	return true;
}
