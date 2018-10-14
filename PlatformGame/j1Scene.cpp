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
#include "jPlayer.h"

j1Scene::j1Scene() : j1Module()
{
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
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	App->map->Load(map_name.start->data->GetString());
	//App->audio->PlayMusic("audio/music/Level1.ogg", 0.0f);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;
	if (App->player->NextMap) {
		KnowMap = 1;
		App->player->CleanUp();
		App->player->Disable();
		App->map->ChangeMap(map_name[KnowMap]);
		App->player->Start();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT) {
			App->player->CleanUp();
			App->player->Disable();
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Start();
			KnowMap = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		if (KnowMap == 0) {
			App->player->CleanUp();
			App->player->Disable();
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Start();
		}
		else if (KnowMap == 1) {
			App->player->CleanUp();
			App->player->Disable();
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Start();
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT) {
			App->player->CleanUp();
			App->player->Disable();
			App->map->ChangeMap(map_name[KnowMap]);
			App->player->Start();
			KnowMap = 1;
	}

	//App->render->Blit(img, 0, 0);
	App->map->Draw();


	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->collision->Disable();
	LOG("Freeing scene");
	return true;
}
