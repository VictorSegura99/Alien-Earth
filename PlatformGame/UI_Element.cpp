#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"

UI_Element::UI_Element()
{
}

UI_Element::UI_Element(int x, int y)
{

}

UI_Element::~UI_Element()
{
}

void UI_Element::Draw(float dt)
{

	App->render->Blit(texture, position.x, position.y, &(png_pos));

}
