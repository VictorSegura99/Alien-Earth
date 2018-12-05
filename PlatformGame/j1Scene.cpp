#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Choose.h"
#include "j1FadeToBlack.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	//App->scene->Disable();
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	
	for (pugi::xml_node map = config.child("mapname"); map; map = map.next_sibling("mapname")) {
		p2SString * data = new p2SString;
		data->create(map.attribute("name").as_string());
		map_name.add(data);
	}
	Song = config.child("song").text().as_string();
	tutorial[0] = config.child("tutorialJeff").text().as_string();
	tutorial[1] = config.child("tutorialJane").text().as_string();
	tutorial[2] = config.child("tutorialJerry").text().as_string();
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	BROFILER_CATEGORY("Scene: Start", Profiler::Color::LightYellow);
	//App->map->Load(map_name.start->data->GetString());
	App->audio->PlayMusic(Song.GetString());
	//active = false;
	TutorialJeff = App->tex->Load(tutorial[0].GetString());
	TutorialJane = App->tex->Load(tutorial[1].GetString());
	TutorialJerry = App->tex->Load(tutorial[2].GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene: PreUpdate", Profiler::Color::LightYellow);
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene: Update", Profiler::Color::LightYellow);
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	
	if (App->entitymanager->GetPlayerData()->NextMap) {
		App->entitymanager->GetPlayerData()->NextMap = false;
		KnowMap = 1;
		App->map->ChangeMap(map_name[KnowMap]);
		App->entitymanager->GetPlayerData()->Spawn();
	}
	if (!App->entitymanager->GetPlayerData()->Intro) {
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			App->entitymanager->DeleteEnemies();
			KnowMap = 0;
			App->map->ChangeMap(map_name[KnowMap]);
			App->entitymanager->GetPlayerData()->Spawn();

		}
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			if (KnowMap == 0) {
				App->entitymanager->DeleteEnemies();
				App->map->ChangeMap(map_name[KnowMap]);
				App->entitymanager->GetPlayerData()->Spawn();
			}
			else if (KnowMap == 1) {
				App->entitymanager->DeleteEnemies();
				App->map->ChangeMap(map_name[KnowMap]);
				App->entitymanager->GetPlayerData()->Spawn();
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
			App->entitymanager->DeleteEnemies();
			KnowMap = 1;
			App->map->ChangeMap(map_name[KnowMap]);
			App->entitymanager->GetPlayerData()->Spawn();
		}
		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			App->fade->FadeToBlack(3.0f);
			CanStart = true;
		}
	}
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene: PostUpdate", Profiler::Color::LightYellow);
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (CanStart && App->fade->current_step == App->fade->fade_from_black) {
		CanStart = false;
		App->entitymanager->DeleteEnemies();
		App->entitymanager->ActiveGame = false;
		App->scene->active = false;
		App->collision->active = false;
		App->map->active = false;
		App->choose->start = false;
		App->choose->GameOn = false;
		App->entitymanager->GetPlayerData()->Intro = true;
		App->entitymanager->GetPlayerData()->NoInput = false;
		App->entitymanager->GetPlayerData()->DeleteUI();
		App->choose->Start();
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	//App->collision->Disable();
	App->tex->UnLoad(TutorialJane);
	App->tex->UnLoad(TutorialJeff);
	App->tex->UnLoad(TutorialJerry);
	LOG("Freeing scene");
	return true;
}

bool j1Scene::Load(pugi::xml_node & scene)
{
	KnowMap = scene.child("KnowMap").attribute("value").as_int();

	return true;
}

bool j1Scene::Save(pugi::xml_node & scene) const
{
	scene.append_child("KnowMap").append_attribute("value") = KnowMap;


	return true;
}

void j1Scene::SpawnEnemies() 
{
	BROFILER_CATEGORY("Scene: Spawn", Profiler::Color::LightYellow);
	if (WantToSpawnEnemies) {
		if (KnowMap == 0) {

			//Platforms
			while (bat1 == nullptr) {
				bat1 = App->entitymanager->CreateEntity(EntityType::BAT, 6000, 600);
			}
			App->entitymanager->CreateEntity(EntityType::COIN, 900, 500);
			App->entitymanager->CreateEntity(EntityType::COIN, 1000, 500);
			App->entitymanager->CreateEntity(EntityType::COIN, 1100, 500);
			App->entitymanager->CreateEntity(EntityType::COIN, 1200, 500);
			App->entitymanager->CreateEntity(EntityType::COIN, 1300, 500);
			App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 7480, 900);
			App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 8300, 900);
			//Enemies
			App->entitymanager->CreateEntity(EntityType::SPIDER, 6800, 400);

			App->entitymanager->CreateEntity(EntityType::BAT, 8000, 400);
			App->entitymanager->CreateEntity(EntityType::BAT, 9500, 600);
			App->entitymanager->CreateEntity(EntityType::SPIDER, 10500, 450);
		}
		if (KnowMap == 1) {
			App->entitymanager->CreateEntity(EntityType::SPIDER, 1300, 300);
			App->entitymanager->CreateEntity(EntityType::SPIDER, 1800, 500);
			App->entitymanager->CreateEntity(EntityType::BAT, 3500, 300);
			App->entitymanager->CreateEntity(EntityType::BAT, 7500, 700);
			App->entitymanager->CreateEntity(EntityType::BAT, 4500, 700);
			App->entitymanager->CreateEntity(EntityType::SPIDER, 6600, 700);
		}
	}
	if (KnowMap == 0) {
		App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 7480, 900);
		App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 8300, 900);
	}
}
