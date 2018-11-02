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
#include "j1Choose.h"

j1Choose::j1Choose() : j1Module()
{
	name.create("choose");
}

// Destructor
j1Choose::~j1Choose()
{}

// Called before render is available
bool j1Choose::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	file_texture = config.child("tex").text().as_string();

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Choose::Start()
{
	App->scene->active = false;
	App->player->active = false;
	App->collision->active = false;
	App->map->active = false;


	texture = App->tex->Load(file_texture.GetString());
	return true;
}

// Called each loop iteration
bool j1Choose::PreUpdate()
{
	App->input->GetMousePosition(mouse.x, mouse.y);
	
	return true;
}

// Called each loop iteration
bool j1Choose::Update(float dt)
{
	if (!GameOn) {
		if (mouse.x >= 50 && mouse.x <= 350 && mouse.y >= 50 && mouse.y <= 850) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				playernumber = 0;
				App->scene->active = !App->scene->active;
				App->player->active = !App->player->active;
				App->collision->active = !App->collision->active;
				App->map->active = !App->map->active;
				App->player->LoadPushbacks(playernumber);
				GameOn = true;
			}
		}
		else if (mouse.x >= 351 && mouse.x <= 650 && mouse.y >= 50 && mouse.y <= 850) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				playernumber = 1;
				App->scene->active = !App->scene->active;
				App->player->active = !App->player->active;
				App->collision->active = !App->collision->active;
				App->map->active = !App->map->active;
				App->player->LoadPushbacks(playernumber);
				GameOn = true;
			}
		}
		else if (mouse.x >= 651 && mouse.x <= 950 && mouse.y >= 50 && mouse.y <= 850) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				playernumber = 2;
				App->scene->active = !App->scene->active;
				App->player->active = !App->player->active;
				App->collision->active = !App->collision->active;
				App->map->active = !App->map->active;
				App->player->LoadPushbacks(playernumber);
				GameOn = true;
			}
		}
		App->render->Blit(texture, 0, 0, NULL, 1.0f);
	}
	

	return true;
}

// Called each loop iteration
bool j1Choose::PostUpdate()
{
	bool ret = true;
	if (!GameOn) {
		/*if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
			App->scene->active = !App->scene->active;
			App->player->active = !App->player->active;
			App->collision->active = !App->collision->active;
			App->map->active = !App->map->active;
			GameOn = true;
		}*/
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			ret = false;
	}
	
	return ret;
}

// Called before quitting
bool j1Choose::CleanUp()
{

	LOG("Freeing scene");
	return true;
}


