#include "UI_Manager.h"
#include "Button.h"

UI_Manager::UI_Manager()
{
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

bool UI_Manager::PreUpdate()
{

	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr)
			elements[i]->PreUpdate();
	}


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

bool UI_Manager::PostUpdate()
{

	for (int i = 0; i < elements.Count(); i++) {
		if (elements[i] != nullptr)
			elements[i]->PostUpdate();
	}

	return true;
}

bool UI_Manager::Load(pugi::xml_node &)
{
	return true;
}

bool UI_Manager::Save(pugi::xml_node &) const
{
	return true;
}

bool UI_Manager::CleanUp()
{
	return true;
}

UI_Element * UI_Manager::CreateButton(SDL_Rect pos)
{
	UI_Element* ret = nullptr;
	
	ret = new Button(pos);
	ret->type = BUTTON;

	if (ret != nullptr)
		elements.PushBack(ret);

	return ret;
}
