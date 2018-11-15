#include "j1App.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"


EntityManager::EntityManager()
{
	name.create("entity_manager");
	player = CreateEntity(EntityType::PLAYER);
}

// Destructor
EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr) 
			entities[i]->Awake(config);
	}

	return ret;
}

bool EntityManager::Start()
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr)
			entities[i]->Start();
	}


	return ret;
}

bool EntityManager::PreUpdate()
{
	if (ActivePlayer) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->PreUpdate();
		}
	}
	


	return true;
}

// Called before render is available
bool EntityManager::Update(float dt)
{
	if (ActivePlayer) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->Update(dt);
		}
	}
	return true;
}

bool EntityManager::PostUpdate()
{
	if (ActivePlayer) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->PostUpdate();
		}
	}
	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	


	return true;
}


void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr)
			entities[i]->OnCollision(c2);
}


Entity * EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;
	switch (type) {
	case EntityType::PLAYER: ret = new Player(); 
		break;
	}
	if (ret != nullptr)
		entities.PushBack(ret);
	return ret;
}


bool EntityManager::Load(pugi::xml_node& load)
{
	bool ret = true;

	
	
	return ret;
}

bool EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;


	return ret;
}

Player* EntityManager::GetPlayerData() const {

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}
