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
	file_texture[0] = config.child("Start").text().as_string();
	file_texture[1] = config.child("NoChoose").text().as_string();
	file_texture[2] = config.child("Choose1").text().as_string();
	file_texture[3] = config.child("Choose2").text().as_string();
	file_texture[4] = config.child("Choose3").text().as_string();
	MinY_ChooseRect = config.child("MinY_ChooseRect").attribute("value").as_int();
	MaxY_ChooseRect = config.child("MaxY_ChooseRect").attribute("value").as_int();
	MinX_RectChoosePlayer1 = config.child("MinX_RectChoosePlayer1").attribute("value").as_int();
	MaxX_RectChoosePlayer1 = config.child("MaxX_RectChoosePlayer1").attribute("value").as_int();
	MinX_RectChoosePlayer2 = config.child("MinX_RectChoosePlayer2").attribute("value").as_int();
	MaxX_RectChoosePlayer2 = config.child("MaxX_RectChoosePlayer2").attribute("value").as_int();
	MinX_RectChoosePlayer3 = config.child("MinX_RectChoosePlayer3").attribute("value").as_int();
	MaxX_RectChoosePlayer3 = config.child("MaxX_RectChoosePlayer3").attribute("value").as_int();
	PlayerNumber1 = config.child("PlayerNumber1").attribute("value").as_int();
	PlayerNumber2 = config.child("PlayerNumber2").attribute("value").as_int();
	PlayerNumber3 = config.child("PlayerNumber3").attribute("value").as_int();
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

	ScreenStart = App->tex->Load(file_texture[0].GetString());
	NoChoose = App->tex->Load(file_texture[1].GetString());
	choose1 = App->tex->Load(file_texture[2].GetString());
	choose2 = App->tex->Load(file_texture[3].GetString());
	choose3 = App->tex->Load(file_texture[4].GetString());

	//YellowStand.PushBack({ 500,93,65,82 });
	YellowStand.PushBack({ 26,261,189,236 });

	YellowWalk.PushBack({ 216,261,191,236 });
	YellowWalk.PushBack({ 411,254,199,243 });
	YellowWalk.speed = 0.1f;


	yellow = App->tex->Load(App->player->sprites_name[0].GetString());

	//PinkStand.PushBack({ 520,101,65,92 });
	PinkStand.PushBack({ 26,261,169,236 });
	pink = App->tex->Load(App->player->sprites_name[1].GetString());

	BlueStand.PushBack({0,261,168,236});

	BlueWalk.PushBack({ 193,260,162,236 });
	BlueWalk.PushBack({ 368,261,163,237 });
	BlueWalk.speed = 0.1f;

	blue = App->tex->Load(App->player->sprites_name[2].GetString());

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
			if (mouse.x >= MinX_RectChoosePlayer1 && mouse.x <= MaxX_RectChoosePlayer1 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose1, 0, 0, NULL, 1.0f);
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(yellow, 150, 350, &(YellowWalk.GetCurrentFrame()), 1.0f);
				App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame()), 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber1;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else if (mouse.x >= MinX_RectChoosePlayer2 && mouse.x <= MaxX_RectChoosePlayer2 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose2, 0, 0, NULL, 1.0f);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame()), 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber2;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else if (mouse.x >= MinX_RectChoosePlayer3 && mouse.x <= MaxX_RectChoosePlayer3 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose3, 0, 0, NULL, 1.0f);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(blue, 665, 350, &(BlueWalk.GetCurrentFrame()), 1.0f);
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber3;
					App->scene->active = !App->scene->active;
					App->player->active = !App->player->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->player->ChangePlayer(playernumber);
					GameOn = true;
				}
			}
			else {
				App->render->Blit(NoChoose, 0, 0, NULL, 1.0f);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame()), 1.0f);
				App->render->Blit(blue, 665,350, &(BlueStand.GetCurrentFrame()), 1.0f);
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
	App->tex->UnLoad(NoChoose);
	App->tex->UnLoad(choose1);
	App->tex->UnLoad(choose2);
	App->tex->UnLoad(choose3);
	LOG("Freeing scene");
	return true;
}


