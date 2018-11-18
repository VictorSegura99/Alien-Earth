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
#include "j1Choose.h"
#include "Player.h"
#include "j1Collision.h"
#include "Entity.h"
#include "EntityManager.h"

#include "Brofiler/Brofiler.h"

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
	ChooseFx= config.child("ChooseFx").text().as_string();
	IntroFx = config.child("IntroFx").text().as_string();
	YellowStand = LoadGigantAliensAnimations(0, config, "Stand");
	PinkStand = LoadGigantAliensAnimations(1, config, "Stand");
	BlueStand = LoadGigantAliensAnimations(2, config, "Stand");
	YellowWalk = LoadGigantAliensAnimations(0, config, "Walk");
	PinkWalk = LoadGigantAliensAnimations(1, config, "Walk");
	BlueWalk = LoadGigantAliensAnimations(2, config, "Walk");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Choose::Start()
{
	App->scene->active = false;
	//App->player->active = false;
	App->collision->active = false;
	App->map->active = false;
	GameOn = false;
	ScreenStart = App->tex->Load(file_texture[0].GetString());
	NoChoose = App->tex->Load(file_texture[1].GetString());
	choose1 = App->tex->Load(file_texture[2].GetString());
	choose2 = App->tex->Load(file_texture[3].GetString());
	choose3 = App->tex->Load(file_texture[4].GetString());

	yellow = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[0].GetString());
	pink = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[1].GetString());
	blue = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[2].GetString());

	choosefx = App->audio->LoadFx(ChooseFx.GetString());
	introfx = App->audio->LoadFx(IntroFx.GetString());

	return true;
}

// Called each loop iteration
bool j1Choose::PreUpdate()
{
	BROFILER_CATEGORY("Choose: PreUpdate", Profiler::Color::Aquamarine);
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !GameOn && !start) {
		start = true;
		App->audio->PlayFx(introfx);
	}
	return true;
}

// Called each loop iteration
bool j1Choose::Update(float dt)
{
	BROFILER_CATEGORY("Choose: Update", Profiler::Color::Aquamarine);
	if (start) {
		if (!GameOn) {
			if (mouse.x >= MinX_RectChoosePlayer1 && mouse.x <= MaxX_RectChoosePlayer1 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose1, 0, 0, NULL);
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
				App->render->Blit(yellow, 150, 350, &(YellowWalk.GetCurrentFrame(dt)));
				App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame(dt)));
				if (!repeat) {
					App->audio->PlayFx(choosefx);
					repeat = true;
				}
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber1;
					App->scene->active = !App->scene->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->scene->KnowMap = 0;
					App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
					App->entitymanager->ActiveGame = true;
					if (App->entitymanager->player == nullptr)
						App->entitymanager->player = App->entitymanager->player = App->entitymanager->CreateEntity(EntityType::PLAYER);
					App->entitymanager->GetPlayerData()->Start();
					App->entitymanager->GetPlayerData()->ChangePlayer(playernumber);
					App->scene->SpawnEnemies();
					App->entitymanager->GetPlayerData()->SetCamera();
					GameOn = true;
				}
			}
			else if (mouse.x >= MinX_RectChoosePlayer2 && mouse.x <= MaxX_RectChoosePlayer2 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose2, 0, 0, NULL);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
				App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame(dt)));
				App->render->Blit(pink, 418, 350, &(PinkWalk.GetCurrentFrame(dt)));
				if (!repeat) {
					App->audio->PlayFx(choosefx);
					repeat = true;
				}
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber2;
					App->scene->active = !App->scene->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->scene->KnowMap = 0;
					App->scene->SpawnEnemies();
					App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
					App->entitymanager->ActiveGame = true;
					if (App->entitymanager->player == nullptr)
						App->entitymanager->player = App->entitymanager->player = App->entitymanager->CreateEntity(EntityType::PLAYER);
					App->entitymanager->GetPlayerData()->Start();
					App->entitymanager->GetPlayerData()->ChangePlayer(playernumber);
					App->entitymanager->GetPlayerData()->SetCamera();
					GameOn = true;
				}
			}
			else if (mouse.x >= MinX_RectChoosePlayer3 && mouse.x <= MaxX_RectChoosePlayer3 && mouse.y >= MinY_ChooseRect && mouse.y <= MaxY_ChooseRect) {
				App->render->Blit(choose3, 0, 0, NULL);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
				App->render->Blit(blue, 665, 350, &(BlueWalk.GetCurrentFrame(dt)));
				if (!repeat) {
					App->audio->PlayFx(choosefx);
					repeat = true;
				}
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					playernumber = PlayerNumber3;
					App->scene->active = !App->scene->active;
					App->collision->active = !App->collision->active;
					App->map->active = !App->map->active;
					App->scene->KnowMap = 0;
					App->scene->SpawnEnemies();
					App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
					App->entitymanager->ActiveGame = true;
					if (App->entitymanager->player == nullptr)
						App->entitymanager->player = App->entitymanager->player = App->entitymanager->CreateEntity(EntityType::PLAYER);
					App->entitymanager->GetPlayerData()->Start();
					App->entitymanager->GetPlayerData()->ChangePlayer(playernumber);
					App->entitymanager->GetPlayerData()->SetCamera();
					GameOn = true;
				}
			}
			else {
				App->render->Blit(NoChoose, 0, 0, NULL);
				App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
				App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
				App->render->Blit(blue, 665,350, &(BlueStand.GetCurrentFrame(dt)));
				repeat = false;
			}
		}
	}
	else {
		App->render->Blit(ScreenStart, 0, 0, NULL);
	}
	return true;
}

// Called each loop iteration
bool j1Choose::PostUpdate()
{
	BROFILER_CATEGORY("Choose: PostUpdate", Profiler::Color::Aquamarine);
	bool ret = true;
	if (!GameOn) {
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

Animation j1Choose::LoadGigantAliensAnimations(int playernumber, pugi::xml_node& config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	switch (playernumber) {
	case 0:
		XML_Name_Player_Anims = "AnimationsPlayerYellow";
		break;
	case 1:
		XML_Name_Player_Anims = "AnimationsPlayerPink";
		break;
	case 2:
		XML_Name_Player_Anims = "AnimationsPlayerBlue";
		break;
	}

	for (pugi::xml_node frames = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	}
	anim.speed = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("speed").as_float();
	anim.loop = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("loop").as_bool();

	return anim;
}


