#include "UI_Manager.h"
#include "Button.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "CheckBox.h"
#include "Image.h"

UI_Manager::UI_Manager()
{

	name.create("UI");

}

UI_Manager::~UI_Manager()
{
}

bool UI_Manager::Awake(pugi::xml_node & config)
{
	return true;
}

bool UI_Manager::Start()
{
	return true;
}

bool UI_Manager::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug_draw = !debug_draw;
	}

	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr)
			elements[i]->Update(dt);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr)
			elements[i]->Draw(dt);
	}

	return true;
}

bool UI_Manager::CleanUp()
{

	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}

	elements.Clear();

	return true;
}

void UI_Manager::Draw(float dt)
{
}

bool UI_Manager::Load(pugi::xml_node &)
{
	return true;
}

bool UI_Manager::Save(pugi::xml_node &) const
{
	return true;
}

UI_Element * UI_Manager::CreateButton(int x, int y, int type)
{
	UI_Element* ret = nullptr;
	
	ret = new Button(x, y, type);
	ret->type = BUTTON;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

UI_Element * UI_Manager::CreateCheckBox(int x, int y)
{
	UI_Element* ret = nullptr;

	ret = new CheckBox(x, y);
	ret->type = CHECKBOX;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

UI_Element * UI_Manager::CreateImage(int x, int y)
{
	UI_Element* ret = nullptr;

	ret = new Image(x, y);
	ret->type = IMAGE;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

void UI_Manager::DeleteButtons()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type == BUTTON) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}
void UI_Manager::DeleteCheckBoxes()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type == CHECKBOX) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}


