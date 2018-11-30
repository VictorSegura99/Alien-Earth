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
#include "j1FadeToBlack.h"
#include "EntityManager.h"
#include "Button.h"
#include "UI_Element.h"
#include "Image.h"
#include "UI_Manager.h"

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
	//choose1 = App->tex->Load(file_texture[2].GetString());
	//choose2 = App->tex->Load(file_texture[3].GetString());
	//choose3 = App->tex->Load(file_texture[4].GetString());
	Choose = App->tex->Load("textures/ChooseReworked.png");
	yellow = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[0].GetString());
	pink = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[1].GetString());
	blue = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[2].GetString());
	Settings = App->tex->Load("textures/Settings.png");
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
		CreateMainMenuButtons();
		//CreateButtons();
	}
	return true;
}

// Called each loop iteration
bool j1Choose::Update(float dt)
{
	BROFILER_CATEGORY("Choose: Update", Profiler::Color::Aquamarine);
	if (start) {
		if (!GameOn) {
			if (InSettings)
				SettingsMenu(dt);
			if (InMainMenu)
				MainMenu();
			if (StartChoosing)
				MenuChoosePlayer(dt);
			
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
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || Exit)
			ret = false;
	}
	if (GoStart) {
		if (App->fade->current_step==App->fade->fade_from_black) {
			GoStart = false;
			InMainMenu = true;
			StartChoosing = false;
			App->scene->active = !App->scene->active;
			App->collision->active = !App->collision->active;
			App->map->active = !App->map->active;
			App->scene->KnowMap = 0;
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			App->entitymanager->ActiveGame = true;
			App->entitymanager->GetPlayerData()->Start();
			App->entitymanager->GetPlayerData()->ChangePlayer(playernumber);
			App->scene->SpawnEnemies();
			App->entitymanager->GetPlayerData()->SetUI();
			App->ui_manager->DeleteButtons();
			GameOn = true;
		}
	}
	if (GoStartSaved) {
		if (App->fade->current_step == App->fade->fade_from_black) {
			GoStartSaved = false;
			InMainMenu = true;
			App->scene->active = !App->scene->active;
			App->collision->active = !App->collision->active;
			App->map->active = !App->map->active;
			App->scene->KnowMap = 0;
			App->entitymanager->ActiveGame = true;
			App->LoadGame("save_game.xml");
			App->entitymanager->GetPlayerData()->SetUI();
			App->ui_manager->DeleteButtons();
			GameOn = true;
		}
	}
	return ret;
}

// Called before quitting
bool j1Choose::CleanUp()
{
	App->tex->UnLoad(ScreenStart);
	App->tex->UnLoad(NoChoose);
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

void j1Choose::CreateButtonsTypePlayer()
{
	buttonJEFF = App->ui_manager->CreateButton(138, 229, 2);
	buttonJEFF->SetSpritesData({ 0,0,0,0 }, { 0,0,225,441 }, { 0,0,225,441 });
	buttonJANE = App->ui_manager->CreateButton(388, 229, 2);
	buttonJANE->SetSpritesData({ 0,0,0,0 }, { 0,0,225,441 }, { 0,0,225,441 });
	buttonJERRY = App->ui_manager->CreateButton(638, 229, 2);
	buttonJERRY->SetSpritesData({ 0,0,0,0 }, { 0,0,225,441 }, { 0,0,225,441 });
	buttonGOBACK = App->ui_manager->CreateButton(50, 25, 3);
	buttonGOBACK->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
}

void j1Choose::StartLevel()
{
	App->entitymanager->GetPlayerData()->Intro = true;
	App->fade->FadeToBlack(3.0f);
	GoStart = true;
}

void j1Choose::MenuChoosePlayer(float dt)
{
	App->render->Blit(ScreenStart, 0, 0, NULL);
	
	if (buttonJEFF->mouseOn) {
		App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(yellow, 150, 350, &(YellowWalk.GetCurrentFrame(dt)));
		App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame(dt)));
		if (buttonJEFF->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber1;
			StartLevel();
		}
	}
	else if (buttonJANE->mouseOn) {
		App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, 418, 350, &(PinkWalk.GetCurrentFrame(dt)));
		if (buttonJANE->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber2;
			StartLevel();
		}
	}
	else if (buttonJERRY->mouseOn) {
		App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, 665, 350, &(BlueWalk.GetCurrentFrame(dt)));
		if (buttonJERRY->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber3;
			StartLevel();
		}
	}
	else {
		App->render->Blit(ScreenStart, 0, 0, NULL);
		App->render->Blit(yellow, 158, 350, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, 421, 350, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, 665, 350, &(BlueStand.GetCurrentFrame(dt)));
		repeat = false;
	}
	if (buttonGOBACK->pressed) {
		App->ui_manager->DeleteButtons();
		App->ui_manager->DeleteLabels();
		StartChoosing = false;
		InMainMenu = true;
		CreateMainMenuButtons();
	}
	
}

void j1Choose::MainMenu()
{
	App->render->Blit(ScreenStart, 0, 0, NULL);
	if (buttonSTART->pressed) {
		App->ui_manager->DeleteButtons();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteCheckBoxes();
		App->ui_manager->DeleteImages();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteSliders();
		CreateButtonsTypePlayer();
		InMainMenu = false;
		StartChoosing = true;
	}
	if (buttonCONTINUE->pressed) {
		App->ui_manager->DeleteButtons();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteCheckBoxes();
		App->ui_manager->DeleteImages();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteSliders();
		App->entitymanager->GetPlayerData()->Intro = false;
		App->fade->FadeToBlack(3.0f);
		GoStartSaved = true;
	}
	if (buttonSETTINGS->pressed) {
		App->ui_manager->DeleteButtons();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteCheckBoxes();
		App->ui_manager->DeleteImages();
		App->ui_manager->DeleteLabels();
		App->ui_manager->DeleteSliders();
		CreateMainMenuButtons();
		CreateSettingsButtons();
		buttonSTART->NoUse = true;
		buttonCONTINUE->NoUse = true;
		buttonSETTINGS->NoUse = true;
		buttonEXIT->NoUse = true;
		buttonCREDITS->NoUse = true;
		InMainMenu = false;
		InSettings = true;

	}
	if (buttonEXIT->pressed) {
		Exit = true;
	}

}

void j1Choose::CreateMainMenuButtons()
{
	AlreadyChoosen = false;
	buttonSTART = App->ui_manager->CreateButton(400, 250, 1, "START", 30);
	buttonCONTINUE = App->ui_manager->CreateButton(400, 350, 1, "CONTINUE", 30);
	buttonSETTINGS = App->ui_manager->CreateButton(400, 450, 1, "SETTINGS", 30);
	buttonCREDITS = App->ui_manager->CreateButton(400, 550, 1, "CREDITS", 30);
	buttonEXIT = App->ui_manager->CreateButton(400, 650, 1, "EXIT", 30);

	
	
}

void j1Choose::SettingsMenu(float dt)
{
	App->render->Blit(ScreenStart, 0, 0, NULL);
	if (imageSETTINGS->position.y <= buttonEXIT->position.y + buttonEXIT->height) {
		buttonEXIT->NoRenderLabel = true;
	}
	if (imageSETTINGS->position.y <= buttonCREDITS->position.y + buttonCREDITS->height) {
		buttonCREDITS->NoRenderLabel = true;
	}
	if (imageSETTINGS->position.y <= buttonSETTINGS->position.y + buttonSETTINGS->height) {
		buttonSETTINGS->NoRenderLabel = true;
	}
	if (imageSETTINGS->position.y <= buttonCONTINUE->position.y + buttonCONTINUE->height) {
		buttonCONTINUE->NoRenderLabel = true;
	}
	if (imageSETTINGS->position.y <= buttonSTART->position.y + buttonSTART->height) {
		buttonSTART->NoRenderLabel = true;
	}
	if (imageSETTINGS->position.y >= buttonEXIT->position.y) {
		buttonEXIT->NoRenderLabel = false;
	}
	if (imageSETTINGS->position.y >= buttonCREDITS->position.y) {
		buttonCREDITS->NoRenderLabel = false;
	}
	if (imageSETTINGS->position.y >= buttonSETTINGS->position.y) {
		buttonSETTINGS->NoRenderLabel = false;
	}
	if (imageSETTINGS->position.y >= buttonCONTINUE->position.y) {
		buttonCONTINUE->NoRenderLabel = false;
	}
	if (imageSETTINGS->position.y >= buttonSTART->position.y) {
		buttonSTART->NoRenderLabel = false;
	}
	if (!Positioned && !SettingMenuDone) { //MENU GOING UP

		buttonGOBACKSETTINGS->position.y -= 1000 * dt;
		imageSETTINGS->position.y -= 1000 * dt;
		checkboxFPS->position.y -= 1000 * dt;
		labelFPS->position.y -= 1000 * dt;
		sliderVOLUMEMUSIC->position.y -= 1000 * dt;
		labelMUSICVOLUME->position.y -= 1000 * dt;
		sliderVOLUMEFX->position.y -= 1000 * dt;
		labelVOLUMEFX->position.y -= 1000 * dt;
		if (buttonGOBACKSETTINGS->position.y <= 100) {
			Positioned = true;
			SettingMenuDone = true;
		}
	}
	if (!Positioned && SettingMenuDone) { //MENU GOING DOWN

		buttonGOBACKSETTINGS->position.y += 2000 * dt;
		imageSETTINGS->position.y += 2000 * dt;
		checkboxFPS->position.y += 2000 * dt;
		labelFPS->position.y += 2000 * dt;
		sliderVOLUMEMUSIC->position.y += 2000 * dt;
		labelMUSICVOLUME->position.y += 2000 * dt;
		sliderVOLUMEFX->position.y += 2000 * dt;
		labelVOLUMEFX->position.y += 2000 * dt;
		if (buttonGOBACKSETTINGS->position.y >= 1225) {
			buttonSTART->NoUse = false;
			buttonCONTINUE->NoUse = false;
			buttonSETTINGS->NoUse = false;
			buttonEXIT->NoUse = false;
			buttonCREDITS->NoUse = false;
			InSettings = false;
			SettingMenuDone = false;
			InMainMenu = true;
		}
	}
	if (SettingMenuDone) { //MENU LOGIC BUTTONS
		if (buttonGOBACKSETTINGS->pressed) {
			Positioned = false;
		}
		if (checkboxFPS->pressed) {
			App->capactivated = true;
		}
		if (!checkboxFPS->pressed) {
			App->capactivated = false;
		}
		App->audio->volume= sliderVOLUMEMUSIC->Value;
		App->audio->fxvolume = sliderVOLUMEFX->Value;
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) {
			App->render->camera.y += 20;
		}if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
			App->render->camera.y -= 20;
		}
	}

}

void j1Choose::CreateSettingsButtons()
{

	imageSETTINGS = App->ui_manager->CreateImage(170, 1300, true);
	imageSETTINGS->SetSpritesData({ 758,0,705,671 });
	buttonGOBACKSETTINGS = App->ui_manager->CreateButton(200, 1335, 3);
	buttonGOBACKSETTINGS->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
	checkboxFPS = App->ui_manager->CreateCheckBox(550, 1407);
	labelFPS = App->ui_manager->CreateLabel(270, 1400, "CAP FPS TO 30", 50, true);
	sliderVOLUMEMUSIC = App->ui_manager->CreateSlider(550, 1507, App->audio->volume);
	labelMUSICVOLUME = App->ui_manager->CreateLabel(270, 1507, "MUSIC VOLUME", 50, true);
	sliderVOLUMEFX = App->ui_manager->CreateSlider(550, 1607, App->audio->fxvolume);
	labelVOLUMEFX = App->ui_manager->CreateLabel(270, 1607, "FX VOLUME", 50, true);

	if (App->capactivated)
		checkboxFPS->pressed = true;
}

