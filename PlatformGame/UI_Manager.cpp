#include "UI_Manager.h"

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
	return true;
}

bool UI_Manager::Update(float dt)
{
	return true;
}

bool UI_Manager::PostUpdate()
{
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
