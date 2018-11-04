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
	GameOn = false;

	ScreenStart = App->tex->Load("textures/Start.png");
	texture = App->tex->Load(file_texture.GetString());
	choose1 = App->tex->Load("textures/Choose1.png");
	choose2 = App->tex->Load("textures/Choose2.png");
	choose3 = App->tex->Load("textures/Choose3.png");

	YellowStand.PushBack({ 500,93,65,82 });
	yellow = App->tex->Load(App->player->sprites_name[0].GetString());

	PinkStand.PushBack({ 520,101,65,92 });
	pink = App->tex->Load(App->player->sprites_name[1].GetString());

	

	return true;
}

// Called each loop iteration
bool j1Choose::PreUpdate()
{

	App->input->GetMousePosition(mouse.x, mouse.y);
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		start = true;
	}
	return true;
}

// Called each loop iteration
bool j1Choose::Update(float dt)
{
	if (start) {
		if (!GameOn) {
			if (mouse.x >= 50 && mouse.x <= 350 && mouse.y >= 50 && mouse.y <= 850) {
				App->render->Blit(choose1, 0, 0, NULL, 1.0f);
				App->render->Blit(pink, 450, 600, &(PinkStand.GetCurrentFrame()), 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = 0;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else if (mouse.x >= 351 && mouse.x <= 650 && mouse.y >= 50 && mouse.y <= 850) {
				App->render->Blit(choose2, 0, 0, NULL, 1.0f);
				App->render->Blit(yellow, 200, 600, &(YellowStand.GetCurrentFrame()), 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = 1;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else if (mouse.x >= 651 && mouse.x <= 950 && mouse.y >= 50 && mouse.y <= 850) {
				App->render->Blit(choose3, 0, 0, NULL, 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = 2;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else {
				App->render->Blit(texture, 0, 0, NULL, 1.0f);
				App->render->Blit(yellow, 200, 600, &(YellowStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(pink, 450, 600, &(PinkStand.GetCurrentFrame()), 1.0f);
			}
		}
	}
	else {
		App->render->Blit(ScreenStart, 0, 0, NULL, 1.0f);
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
	App->tex->UnLoad(ScreenStart);
	App->tex->UnLoad(texture);
	App->tex->UnLoad(choose1);
	App->tex->UnLoad(choose2);
	App->tex->UnLoad(choose3);
	LOG("Freeing scene");
	return true;
}


