#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "UI_Manager.h"
#include "Image.h"
#include "UI_Element.h"
#include "j1Scene.h"
#include "Player.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Menu.h"
#include "Slider.h"
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
	SongMenu = config.child("songmenu").text().as_string();
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	BROFILER_CATEGORY("Scene: Start", Profiler::Color::LightYellow);
	//App->map->Load(map_name.start->data->GetString());
	//active = false;
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
		App->LoadGame("save_game.xml", true);

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
			Delay = 0;
			App->fade->FadeToBlack(3.0f);
			CanStart = true;
		}
	}

	CoinsLogic();
	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !GamePaused) {
		Paused = SDL_GetTicks();
		CreatePauseMenu();
		GamePaused = true;
	}
	if (GamePaused) {
		PauseMenu(dt);
	}


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene: PostUpdate", Profiler::Color::LightYellow);
	bool ret = true;

	if (CanStart && App->fade->current_step == App->fade->fade_from_black) {
		App->entitymanager->GetPlayerData()->lives = 3;
		CanStart = false;
		App->entitymanager->DeleteEnemies();
		App->entitymanager->ActiveGame = false;
		App->scene->active = false;
		App->collision->active = false;
		App->map->active = false;
		App->menu->start = false;
		App->menu->GameOn = false;
		App->entitymanager->GetPlayerData()->Intro = true;
		App->entitymanager->GetPlayerData()->NoInput = false;
		App->entitymanager->GetPlayerData()->IsJumping = false;
		App->entitymanager->GetPlayerData()->IsJumping2 = false;
		App->entitymanager->GetPlayerData()->CanJump = false;
		App->entitymanager->GetPlayerData()->CanJump2 = false;
		App->entitymanager->GetPlayerData()->CanSwim = false;
		App->entitymanager->GetPlayerData()->DeleteUI();
		App->menu->Start();
		KnowMap = 0;
		GamePaused = false;
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	//App->collision->Disable();
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
			//Enemies
			App->entitymanager->CreateEntity(EntityType::BAT, 6000, 600);
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
			App->entitymanager->CreateEntity(EntityType::SPIDER, 6600, 700);

		}
	}
	if (KnowMap == 0) {

		App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 7480, 900);
		App->entitymanager->CreateEntity(EntityType::MOVING_PLATFORM, 8300, 900);
		//Coins
		if (App->entitymanager->GetPlayerData()->NumPlayer == 0) {
			App->entitymanager->CreateEntity(EntityType::COIN, 1173, 442);
			App->entitymanager->CreateEntity(EntityType::COIN, 1395, 305);
			App->entitymanager->CreateEntity(EntityType::COIN, 1395, 525);
		}
		if (App->entitymanager->GetPlayerData()->NumPlayer == 1) {
			App->entitymanager->CreateEntity(EntityType::COIN, 1175, 465);
			App->entitymanager->CreateEntity(EntityType::COIN, 1333, 518);
		}
		if (App->entitymanager->GetPlayerData()->NumPlayer == 2) {
			App->entitymanager->CreateEntity(EntityType::COIN, 1173, 442);
			App->entitymanager->CreateEntity(EntityType::COIN, 1395, 510);
		}
		if (App->entitymanager->GetPlayerData()->NumPlayer == 2 || App->entitymanager->GetPlayerData()->NumPlayer == 1)
			App->entitymanager->CreateEntity(EntityType::COIN, 1850, 552);
		App->entitymanager->CreateEntity(EntityType::COIN, 2416, 818);
		App->entitymanager->CreateEntity(EntityType::COIN, 3057, 538);
		App->entitymanager->CreateEntity(EntityType::COIN, 3445, 608);
		App->entitymanager->CreateEntity(EntityType::COIN, 3833, 608);
		App->entitymanager->CreateEntity(EntityType::COIN, 4385, 405);
		App->entitymanager->CreateEntity(EntityType::COIN, 5015, 640);
		App->entitymanager->CreateEntity(EntityType::COIN, 5580, 660);
		App->entitymanager->CreateEntity(EntityType::COIN, 6170, 545);
		App->entitymanager->CreateEntity(EntityType::COIN, 7340, 680);
		App->entitymanager->CreateEntity(EntityType::COIN, 8200, 825);
		App->entitymanager->CreateEntity(EntityType::COIN, 9140, 680);
		App->entitymanager->CreateEntity(EntityType::COIN, 9947, 615);
		App->entitymanager->CreateEntity(EntityType::COIN, 10500, 685);
	}
	if (KnowMap == 1) {
		App->entitymanager->CreateEntity(EntityType::COIN, 768, 330);
		App->entitymanager->CreateEntity(EntityType::COIN, 1274, 540);
		App->entitymanager->CreateEntity(EntityType::COIN, 1677, 679);
		App->entitymanager->CreateEntity(EntityType::COIN, 2381, 688);
		App->entitymanager->CreateEntity(EntityType::COIN, 2878, 900);
		App->entitymanager->CreateEntity(EntityType::COIN, 3300, 777);
		App->entitymanager->CreateEntity(EntityType::COIN, 3918, 692);
		App->entitymanager->CreateEntity(EntityType::COIN, 3090, 480);
		App->entitymanager->CreateEntity(EntityType::COIN, 4247, 126);
		App->entitymanager->CreateEntity(EntityType::COIN, 4450, 600);
		App->entitymanager->CreateEntity(EntityType::COIN, 4930, 969);
		App->entitymanager->CreateEntity(EntityType::COIN, 5530, 342);
		App->entitymanager->CreateEntity(EntityType::COIN, 6730, 905);
		App->entitymanager->CreateEntity(EntityType::COIN, 7537, 900);
		App->entitymanager->CreateEntity(EntityType::COIN, 8210, 712);
		App->entitymanager->CreateEntity(EntityType::COIN, 10014, 1034);
	}

}

void j1Scene::CreatePauseMenu()
{
	
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	image = App->ui_manager->CreateImage(0, 0, false);
	image->SetSpritesData({ 0,1158,1024,768 });
	imagePAUSE = App->ui_manager->CreateImage(170, 60, true);
	imagePAUSE->SetSpritesData({ 758,0,705,671 });
	buttonRESUME = App->ui_manager->CreateButton(120, 140, 1, imagePAUSE, "RESUME", 30);
	buttonGOMAINMENU = App->ui_manager->CreateButton(400, 140, 1, imagePAUSE, "RETURN MAIN MENU", 30);
	sliderVOLUMEMUSIC = App->ui_manager->CreateSlider(380, 463, App->audio->volume, imagePAUSE);
	labelMUSICVOLUME = App->ui_manager->CreateLabel(100, 450, "MUSIC VOLUME", 50, true, imagePAUSE);
	sliderVOLUMEFX = App->ui_manager->CreateSlider(380, 563, App->audio->fxvolume, imagePAUSE);
	labelVOLUMEFX = App->ui_manager->CreateLabel(100, 550, "FX VOLUME", 50, true, imagePAUSE);
	labelPAUSE = App->ui_manager->CreateLabel(imagePAUSE->width / 2, 50, "PAUSE", 60, true, imagePAUSE);
	labelPAUSE->Local_pos.x -= labelPAUSE->width / 2;
	labelGENERALSOUND = App->ui_manager->CreateLabel(100, 350, "GENERAL SOUND", 50, true, imagePAUSE);
	sliderGENERALSOUND = App->ui_manager->CreateSlider(380, 363, 50, imagePAUSE);
	checkboxSOUND = App->ui_manager->CreateCheckBox(380, 257, imagePAUSE);
	if (!App->audio->NoAudio)
		checkboxSOUND->pressed = true;
	else checkboxSOUND->pressed = false;
	labelSOUND = App->ui_manager->CreateLabel(100, 250, "SOUND", 50, true, imagePAUSE);
}

void j1Scene::PauseMenu(float dt)
{
	
	if (buttonRESUME->pressed) {
		buttonRESUME->pressed = false;
		StartTime = SDL_GetTicks();
		StartTimer = true;
		App->ui_manager->DeleteAllUIExeptPlayer();
		imageNUMBER3 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, false);
		imageNUMBER3->type = PLAYERUIIMAGE;
		imageNUMBER3->SetSpritesData({ 1584,1963,29,44 });
		imageNUMBER3->Local_pos.x -= imageNUMBER3->width / 2;
		imageNUMBER3->Local_pos.y -= imageNUMBER3->height / 2;
	}
	if (buttonGOMAINMENU->pressed) {
		NumberCoins = 0;
		Delay = 0;
		StartTimer = true;
		App->fade->FadeToBlack(3.0f);
		CanStart = true;
		buttonGOMAINMENU->pressed = false;
		App->ui_manager->DeleteAllUI();
	}
	if (checkboxSOUND->pressed) {
		App->audio->NoAudio = false;
		sliderGENERALSOUND->NoUse = false;
		sliderVOLUMEMUSIC->NoUse = false;
		sliderVOLUMEFX->NoUse = false;
		App->audio->general = sliderGENERALSOUND->Value;
		App->audio->volume = sliderVOLUMEMUSIC->Value;
		App->audio->fxvolume = sliderVOLUMEFX->Value;
	}
	else {
		App->audio->NoAudio = true;
		sliderGENERALSOUND->NoUse = true;
		sliderVOLUMEMUSIC->NoUse = true;
		sliderVOLUMEFX->NoUse = true;
	}

	if (StartTimer && !CanStart) {
		if (SDL_GetTicks() - StartTime > 1000) {
			Number -= 1;
			StartTime = SDL_GetTicks();
			if (Number == 2) {
				App->ui_manager->DeleteImage(imageNUMBER3);
				imageNUMBER2 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, true);
				imageNUMBER2->type = PLAYERUIIMAGE;
				imageNUMBER2->SetSpritesData({ 1552,1963,31,42 });
				imageNUMBER2->Local_pos.x -= imageNUMBER2->width / 2;
				imageNUMBER2->Local_pos.y -= imageNUMBER2->height / 2;
			}
			if (Number == 1) {
				App->ui_manager->DeleteImage(imageNUMBER2);
				imageNUMBER1 = App->ui_manager->CreateImage(App->win->Width / 2, App->win->Height / 2-100, true);
				imageNUMBER1->type = PLAYERUIIMAGE;
				imageNUMBER1->SetSpritesData({ 1524,1963,26,42 });
				imageNUMBER1->Local_pos.x -= imageNUMBER1->width / 2;
				imageNUMBER1->Local_pos.y -= imageNUMBER1->height / 2;
			}
			if (Number == 0) {
				Number = 3;
				App->ui_manager->DeleteImage(imageNUMBER1);
				Delay += SDL_GetTicks() - Paused;
				StartTimer = false;
				GamePaused = false;
				
			}
				
			
		}
	}
}




void j1Scene::DeletePauseMenu()
{
	App->ui_manager->DeleteUI_Element(imagePAUSE);
	App->ui_manager->DeleteUI_Element(buttonRESUME);
	App->ui_manager->DeleteUI_Element(buttonGOMAINMENU);
	App->ui_manager->DeleteUI_Element(buttonGOMAINMENU->label);
	App->ui_manager->DeleteUI_Element(buttonRESUME->label);
	App->ui_manager->DeleteUI_Element(sliderVOLUMEMUSIC);
	App->ui_manager->DeleteUI_Element(labelMUSICVOLUME);
	App->ui_manager->DeleteUI_Element(sliderVOLUMEFX);
	App->ui_manager->DeleteUI_Element(labelVOLUMEFX);
	App->ui_manager->DeleteUI_Element(labelPAUSE);
	App->ui_manager->DeleteUI_Element(labelGENERALSOUND);
	App->ui_manager->DeleteUI_Element(sliderGENERALSOUND);
	App->ui_manager->DeleteUI_Element(checkboxSOUND);
	App->ui_manager->DeleteUI_Element(labelSOUND);
	App->ui_manager->DeleteUI_Element(sliderGENERALSOUND->image);
	App->ui_manager->DeleteUI_Element(sliderVOLUMEFX->image);
	App->ui_manager->DeleteUI_Element(sliderVOLUMEMUSIC->image);
	App->ui_manager->DeleteUI_Element(image);
	buttonRESUME->pressed = false;
}

void j1Scene::CoinsLogic()
{
	if (NumberCoins >= 10) {
		App->entitymanager->GetPlayerData()->lives++;
		App->entitymanager->GetPlayerData()->Lives();
		NumberCoins = 0;
		App->entitymanager->GetPlayerData()->CountCoins();
	}
}

