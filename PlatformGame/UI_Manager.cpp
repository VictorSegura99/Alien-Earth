#include "UI_Manager.h"
#include "Button.h"

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

UI_Element * UI_Manager::CreateButton(int x, int y, int weight, int height, int type)
{
	UI_Element* ret = nullptr;
	
	ret = new Button(x, y, weight, height, type);
	ret->type = BUTTON;

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


