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
#include "Spider.h"

#include "Brofiler/Brofiler.h"

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
	BROFILER_CATEGORY("EntityManager: PreUpdate", Profiler::Color::Green);
	if (ActiveGame) {
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
	BROFILER_CATEGORY("EntityManager: Update", Profiler::Color::Green);
	if (ActiveGame) {
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr)
				entities[i]->Update(dt);
		}
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type != PLAYER)
				entities[i]->Draw(dt);
		}
		for (int i = 0; i < entities.Count(); i++) {
			if (entities[i] != nullptr && entities[i]->type == PLAYER)
				entities[i]->Draw(dt);
		}
	}
	
	return true;
}

bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManager: PostUpdate", Profiler::Color::Green);
	if (ActiveGame) {
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
	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}

	entities.Clear();

	return true;
}


void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr)
			entities[i]->OnCollision(c2);
}


Entity* EntityManager::CreateEntity(EntityType type, int x, int y)
{
	Entity* ret = nullptr;
	switch (type) {
	case EntityType::SPIDER: ret = new Spider(x, y);
		ret->type = SPIDER;
		break;
	case EntityType::PLAYER: ret = new Player();
		ret->type = PLAYER;
		break;
	}
	if (ret != nullptr)
		entities.PushBack(ret);
	return ret;
}

void EntityManager::DeleteEntities()
{

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}

	entities.Clear();

}
void EntityManager::DeleteEnemies() {
	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr && entities[i]->type != PLAYER) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}
}

bool EntityManager::DeleteEntity(Entity* entity)
{
	bool ret = true;

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] == entity) {
			entities[i]->CleanUp();
			delete(entities[i]);
			entities[i] = nullptr;
			entities.RemoveAt(i);
		}
	}
	return ret;
}


bool EntityManager::Load(pugi::xml_node& load)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type != PLAYER)
			entities[i]->Load(load);
	}
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr && entities[i]->type == PLAYER)
			entities[i]->Load(load);
	}
	
	return ret;
}

bool EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i] != nullptr)
			entities[i]->Save(save);
	}

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




