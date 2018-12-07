#include "UI_Manager.h"
#include "Button.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "Label.h"
#include "CheckBox.h"
#include "Slider.h"
#include "Image.h"

#include "Brofiler/Brofiler.h"


UI_Manager::UI_Manager()
{

	name.create("UI");

}

UI_Manager::~UI_Manager()
{
}

bool UI_Manager::Awake(pugi::xml_node & config)
{
	sprite = config.child("sprite").text().as_string();

	return true;
}

bool UI_Manager::Start()
{
	atlas = App->tex->Load(sprite.GetString());
	return true;
}

bool UI_Manager::Update(float dt)
{
	BROFILER_CATEGORY("UI: Update", Profiler::Color::LightPink);
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug_draw = !debug_draw;
	}

	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr)
			elements[i]->Update(dt);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type != LABEL && elements[i]->type != BUTTON && elements[i]->type != SLIDER && elements[i]->type != CHECKBOX)
			elements[i]->Draw(dt, atlas);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type == PLAYERUI)
			elements[i]->Draw(dt, atlas);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type == BUTTON)
			elements[i]->Draw(dt, atlas);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type == CHECKBOX)
			elements[i]->Draw(dt, atlas);
	}
	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type == SLIDER)
			elements[i]->Draw(dt, atlas);
	}

	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr && elements[i]->type == LABEL)
			elements[i]->Draw(dt, atlas);
	}


	return true;
}

bool UI_Manager::CleanUp()
{
	//App->tex->UnLoad(atlas);
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

void UI_Manager::Draw(float dt, SDL_Texture* texture)
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

UI_Element * UI_Manager::CreateButton(int x, int y, int type, UI_Element* parent, p2SString name, int size)
{
	UI_Element* ret = nullptr;
	
	ret = new Button(x, y, type, parent, name, size);
	ret->type = BUTTON;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}
UI_Element * UI_Manager::CreateCheckBox(int x, int y, UI_Element* parent)
{
	UI_Element* ret = nullptr;

	ret = new CheckBox(x, y, parent);
	ret->type = CHECKBOX;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

UI_Element * UI_Manager::CreateImage(int x, int y, bool WantToBeMoved, UI_Element* parent)
{
	UI_Element* ret = nullptr;

	ret = new Image(x, y, WantToBeMoved, parent);
	ret->type = IMAGE;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

UI_Element * UI_Manager::CreateLabel(int x, int y, p2SString name, int size, bool CanBeMoved, UI_Element* parent)
{
	UI_Element* ret = nullptr;

	ret = new Label(x, y, name, size, CanBeMoved, parent);
	ret->type = LABEL;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}

UI_Element * UI_Manager::CreateSlider(int x, int y, int SliderPos, UI_Element* parent)
{
	UI_Element* ret = nullptr;

	ret = new Slider(x, y, SliderPos, parent);
	ret->type = SLIDER;

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

void UI_Manager::DeleteImages()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type == IMAGE) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}

void UI_Manager::DeleteLabels()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type == LABEL) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}

void UI_Manager::DeleteSliders()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type == SLIDER) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}

void UI_Manager::DeleteAllUI()
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
}

bool UI_Manager::DeleteUI_Element(UI_Element * element)
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] == element) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
	return true;
}

void UI_Manager::DeleteAllUIExeptPlayer()
{
	for (int i = elements.Count() - 1; i >= 0; --i)
	{
		if (elements[i] != nullptr && elements[i]->type != PLAYERUI) {
			elements[i]->CleanUp();
			delete(elements[i]);
			elements[i] = nullptr;
			elements.RemoveAt(i);
		}
	}
}


