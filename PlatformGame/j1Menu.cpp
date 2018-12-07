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
#include "j1Menu.h"
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

j1Menu::j1Menu() : j1Module()
{
	name.create("choose");
}

// Destructor
j1Menu::~j1Menu()
{}

// Called before render is available
bool j1Menu::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	file_texture = config.child("Start").text().as_string();
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
bool j1Menu::Start()
{
	App->scene->active = false;
	//App->player->active = false;
	App->collision->active = false;
	App->map->active = false;
	GameOn = false;
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	start = false;

	App->audio->PlayMusic(App->scene->SongMenu.GetString());

	ScreenStart = App->tex->Load(file_texture.GetString());
	yellow = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[0].GetString());
	pink = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[1].GetString());
	blue = App->tex->Load(App->entitymanager->GetPlayerData()->sprites_name[2].GetString());
	Settings = App->tex->Load("textures/Settings.png");
	choosefx = App->audio->LoadFx(ChooseFx.GetString());
	introfx = App->audio->LoadFx(IntroFx.GetString());
	CreateButtonsTypePlayer();
	CreateMainMenu();
	CreateSettingsButtons();
	CreatehacksButtons();
	CreateIntro();
	CreateCredits();
	WantToDisappearMainMenu(true);
	WantToDisappearButtonsTypePlayer(true);
	
	return true;
}

// Called each loop iteration
bool j1Menu::PreUpdate()
{
	BROFILER_CATEGORY("Menu: PreUpdate", Profiler::Color::Aquamarine);
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !GameOn && !start) {
		start = true;
		App->audio->PlayFx(introfx);	
		App->ui_manager->DeleteUI_Element(Title);
		App->ui_manager->DeleteUI_Element(sentence);
		WantToDisappearMainMenu(false);
		//CreateMainMenuButtons();
		//CreateButtons();
	}
	return true;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	BROFILER_CATEGORY("Menu: Update", Profiler::Color::Aquamarine);
	if (start) {
		if (!GameOn) {
			if (InSettings)
				SettingsMenu(dt);
			if (InMainMenu)
				MainMenu();
			if (StartChoosing)
				MenuChoosePlayer(dt);
			if (InHacks)
				HacksMenu(dt);
			if (InCredits)
				Credits(dt);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) {
		App->render->camera.y -= 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		App->render->camera.y += 10;
	}
	return true;
}

// Called each loop iteration
bool j1Menu::PostUpdate()
{
	BROFILER_CATEGORY("Menu: PostUpdate", Profiler::Color::Aquamarine);
	bool ret = true;
	if (!GameOn) {
		if (Exit)
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
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			App->entitymanager->ActiveGame = true;
			App->entitymanager->GetPlayerData()->Start();
			App->audio->PlayMusic(App->scene->Song.GetString());
			App->entitymanager->GetPlayerData()->ChangePlayer(playernumber);
			App->scene->SpawnEnemies();
			App->entitymanager->GetPlayerData()->SetUI();
			GameOn = true;
		}
	}
	if (GoStartSaved) {
		if (App->fade->current_step == App->fade->fade_from_black) {
			GoStartSaved = false;
			App->ui_manager->DeleteAllUI();
			InMainMenu = true;
			App->scene->active = !App->scene->active;
			App->collision->active = !App->collision->active;
			App->map->active = !App->map->active;
			App->scene->KnowMap = 0;
			App->entitymanager->ActiveGame = true;
			App->LoadGame("save_game.xml", true);
			App->entitymanager->GetPlayerData()->SetUI();
			GameOn = true;
		}
	}
	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	App->tex->UnLoad(ScreenStart);
	LOG("Freeing scene");
	return true;
}

Animation j1Menu::LoadGigantAliensAnimations(int playernumber, pugi::xml_node& config, p2SString NameAnim) const
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


//MENU
void j1Menu::CreateIntro()
{
	Background = App->ui_manager->CreateImage(0, 0, false);
	Background->SetSpritesData({ 0,1158,1024,768 });
	Title = App->ui_manager->CreateImage((App->win->width / 2) - (354 / 2), 70, false);
	Title->SetSpritesData({ 0,783,354,305 });
	sentence = App->ui_manager->CreateImage((App->win->width / 2) - (268 / 2), 550, false);
	sentence->SetSpritesData({ 0,1105,268,35 });
}

void j1Menu::CreateMainMenu()
{
	AlreadyChoosen = false;
	MainTitle = App->ui_manager->CreateImage((App->win->width / 2) - (844 / 2), 70, false);
	MainTitle->SetSpritesData({ 401,784,844	,165 });
	MainTitle->type = BUTTON;
	buttonSTART = App->ui_manager->CreateButton(400, 270, 1, nullptr, "START", 30);
	buttonCONTINUE = App->ui_manager->CreateButton(400, 350, 1, nullptr, "CONTINUE", 30);
	buttonSETTINGS = App->ui_manager->CreateButton(400, 430, 1, nullptr, "SETTINGS", 30);
	buttonHACKS = App->ui_manager->CreateButton(400, 510, 1, nullptr, "HACKS", 30);
	buttonCREDITS = App->ui_manager->CreateButton(400, 590, 1, nullptr, "CREDITS", 30);
	buttonEXIT = App->ui_manager->CreateButton(400, 670, 1, nullptr, "EXIT", 30);
}

void j1Menu::MainMenu()
{
	if (buttonSTART->pressed) {
		WantToDisappearMainMenu(true);
		WantToDisappearButtonsTypePlayer(false);
		//buttonJEFF->pressed = false;
		//buttonJANE->pressed = false;
		//buttonJERRY->pressed = false;
		InMainMenu = false;
		StartChoosing = true;
	}
	if (buttonCONTINUE->pressed) {
		App->LoadGame("save_game.xml", false);
		if (App->CanLoad) {
			App->ui_manager->DeleteAllUI();
			App->entitymanager->GetPlayerData()->Intro = false;
			App->fade->FadeToBlack(3.0f);
			GoStartSaved = true;
		}
		
	}
	if (buttonSETTINGS->pressed) {
		SettingMenuDone = false;
		Positioned = false;
		buttonSTART->NoUse = true;
		buttonCONTINUE->NoUse = true;
		buttonSETTINGS->NoUse = true;
		buttonHACKS->NoUse = true;
		buttonEXIT->NoUse = true;
		buttonCREDITS->NoUse = true;
		InMainMenu = false;
		InSettings = true;
		if (App->capactivated)
			checkboxFPS->pressed = true;
		else checkboxFPS->pressed = false;

	}
	if (buttonHACKS->pressed) {
		HacksMenuDone = false;
		positioned = false;
		buttonSTART->NoUse = true;
		buttonCONTINUE->NoUse = true;
		buttonSETTINGS->NoUse = true;
		buttonHACKS->NoUse = true;
		buttonEXIT->NoUse = true;
		buttonCREDITS->NoUse = true;
		InMainMenu = false;
		InHacks = true;
		if (App->entitymanager->GetPlayerData()->God)
			checkboxGODMODE->pressed = true;
		else checkboxGODMODE->pressed = false;
		if (App->scene->WantToSpawnEnemies)
			checkboxNOENEMIES->pressed = false;
		else checkboxNOENEMIES->pressed = true;
		if (App->scene->KnowMap == 0)
			checkboxSTARTLEVEL2->pressed = false;
		else checkboxSTARTLEVEL2->pressed = true;

	}
	if (buttonCREDITS->pressed) {
		WantToDisappearMainMenu(true);
		InMainMenu = false;
		InCredits = true;
	}
	if (buttonEXIT->pressed) {
		Exit = true;
	}
}


void j1Menu::WantToDisappearMainMenu(bool Disappear)
{
	if (Disappear) {
		buttonSTART->NoUse = true;
		buttonCONTINUE->NoUse = true;
		buttonSETTINGS->NoUse = true;
		buttonEXIT->NoUse = true;
		buttonCREDITS->NoUse = true;
		buttonHACKS->NoUse = true;
		buttonSTART->WantToRender = false;
		buttonCONTINUE->WantToRender = false;
		buttonSETTINGS->WantToRender = false;
		buttonHACKS->WantToRender = false;
		buttonEXIT->WantToRender = false;
		buttonCREDITS->WantToRender = false;
		MainTitle->WantToRender = false;
	}
	else {
		buttonSTART->NoUse = false;
		buttonCONTINUE->NoUse = false;
		buttonSETTINGS->NoUse = false;
		buttonEXIT->NoUse = false;
		buttonHACKS->NoUse = false;
		buttonCREDITS->NoUse = false;
		buttonSTART->WantToRender = true;
		buttonCONTINUE->WantToRender = true;
		buttonSETTINGS->WantToRender = true;
		buttonHACKS->WantToRender = true;
		buttonEXIT->WantToRender = true;
		buttonCREDITS->WantToRender = true;
		MainTitle->WantToRender = true;
	}
}

void j1Menu::CreateButtonsTypePlayer()
{
	CHOOSE = App->ui_manager->CreateImage((App->win->width / 2) - (608 / 2), 70, false);
	CHOOSE->SetSpritesData({ 0,1928,608,72 });
	CHOOSE->type = BUTTON;
	JEFFNAME = App->ui_manager->CreateImage((App->win->width / 4) - (68 / 2), 620, false);
	JEFFNAME->SetSpritesData({ 611,1928,68,38 });
	JEFFNAME->type = BUTTON;
	JANENAME = App->ui_manager->CreateImage((App->win->width / 4) * 2 - (72 / 2), 620, false);
	JANENAME->SetSpritesData({ 681,1928,72,38 });
	JANENAME->type = BUTTON;
	JERRYNAME = App->ui_manager->CreateImage((App->win->width / 4) * 3 - (92 / 2), 620, false);
	JERRYNAME->SetSpritesData({ 754,1928,92,38 });
	JERRYNAME->type = BUTTON;
	buttonJEFF = App->ui_manager->CreateButton((App->win->width / 4)-112, 159, 2);
	buttonJEFF->SetSpritesData({ 0,0,0,0 }, { 0,0,225,441 }, { 0,0,225,441 });
	buttonJANE = App->ui_manager->CreateButton((App->win->width / 4)*2 - 112, 159, 2);
	buttonJANE->SetSpritesData({ 0,0,0,0 }, { 0,0,225,441 }, { 0,0,225,441 });
	buttonJERRY = App->ui_manager->CreateButton((App->win->width / 4) *3-112, 159, 2);
	buttonJERRY->SetSpritesData({ 0,0,0,0	}, { 0,0,225,441 }, { 0,0,225,441 });
	buttonGOBACK = App->ui_manager->CreateButton(50, 25, 3);
	buttonGOBACK->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
}

void j1Menu::MenuChoosePlayer(float dt)
{
	if (buttonJEFF->mouseOn) {
		App->render->Blit(yellow, (App->win->width / 4) - (195 / 2), 265, &(YellowWalk.GetCurrentFrame(dt)));
		App->render->Blit(pink, (App->win->width / 4) * 2 - (160/ 2), 265, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, (App->win->width / 4) * 3 - (169 / 2), 265, &(BlueStand.GetCurrentFrame(dt)));
		if (buttonJEFF->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber1;
			StartLevel();
		}
	}
	else if (buttonJANE->mouseOn) {
		App->render->Blit(yellow, (App->win->width / 4) - (184 / 2), 265, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, (App->win->width / 4) * 3 - (168/2), 265, &(BlueStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, (App->win->width / 4) * 2 - (168 / 2), 265, &(PinkWalk.GetCurrentFrame(dt)));
		if (buttonJANE->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber2;
			StartLevel();
		}
	}
	else if (buttonJERRY->mouseOn) {
		App->render->Blit(yellow, (App->win->width / 4) - (184 / 2), 265, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, (App->win->width / 4) * 2 - (160 / 2), 265, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, (App->win->width / 4) * 3 - (163/2), 265, &(BlueWalk.GetCurrentFrame(dt)));
		if (buttonJERRY->pressed && !AlreadyChoosen) {
			AlreadyChoosen = true;
			playernumber = PlayerNumber3;
			StartLevel();
		}
	}
	else {
		App->render->Blit(yellow, (App->win->width / 4) - (184 / 2), 265, &(YellowStand.GetCurrentFrame(dt)));
		App->render->Blit(pink, (App->win->width / 4) * 2 - (160 / 2), 265, &(PinkStand.GetCurrentFrame(dt)));
		App->render->Blit(blue, (App->win->width / 4) * 3 - (168 / 2), 265, &(BlueStand.GetCurrentFrame(dt)));
		repeat = false;
	}
	if (buttonGOBACK->pressed) {
		WantToDisappearButtonsTypePlayer(true);
		WantToDisappearMainMenu(false);
		StartChoosing = false;
		InMainMenu = true;
	}

}

void j1Menu::WantToDisappearButtonsTypePlayer(bool Disappear)
{
	if (Disappear) {
		CHOOSE->WantToRender = false;
		JEFFNAME->WantToRender = false;
		JANENAME->WantToRender = false;
		JERRYNAME->WantToRender = false;
		buttonJEFF->WantToRender = false;
		buttonJANE->WantToRender = false;
		buttonJERRY->WantToRender = false;
		buttonGOBACK->WantToRender = false;
		CHOOSE->NoUse = true;
		JEFFNAME->NoUse = true;
		JANENAME->NoUse = true;
		JERRYNAME->NoUse = true;
		buttonJEFF->NoUse = true;
		buttonJANE->NoUse = true;
		buttonJERRY->NoUse = true;
		buttonGOBACK->NoUse = true;
	}
	else {
		CHOOSE->WantToRender = true;
		JEFFNAME->WantToRender = true;
		JANENAME->WantToRender = true;
		JERRYNAME->WantToRender = true;
		buttonJEFF->WantToRender = true;
		buttonJANE->WantToRender = true;
		buttonJERRY->WantToRender = true;
		buttonGOBACK->WantToRender = true;
		CHOOSE->NoUse = false;
		JEFFNAME->NoUse = false;
		JANENAME->NoUse = false;
		JERRYNAME->NoUse = false;
		buttonJEFF->NoUse = false;
		buttonJANE->NoUse = false;
		buttonJERRY->NoUse = false;
		buttonGOBACK->NoUse = false;
	}
}

void j1Menu::CreateSettingsButtons()
{
	SettingMenuDone = false;
	imageSETTINGS = App->ui_manager->CreateImage(170, 1000, true);
	imageSETTINGS->SetSpritesData({ 758,0,705,671 });
	imageSETTINGS->type = BUTTON;
	buttonGOBACKSETTINGS = App->ui_manager->CreateButton(37, 40, 3, imageSETTINGS);
	buttonGOBACKSETTINGS->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
	checkboxFPS = App->ui_manager->CreateCheckBox(380, 157, imageSETTINGS);
	labelFPS = App->ui_manager->CreateLabel(100, 150, "CAP FPS TO 30", 50, true, imageSETTINGS);
	sliderVOLUMEMUSIC = App->ui_manager->CreateSlider(380, 452, App->audio->volume, imageSETTINGS);
	labelMUSICVOLUME = App->ui_manager->CreateLabel(100, 450, "MUSIC VOLUME", 50, true, imageSETTINGS);
	sliderVOLUMEFX = App->ui_manager->CreateSlider(380, 552, App->audio->fxvolume, imageSETTINGS);
	labelVOLUMEFX = App->ui_manager->CreateLabel(100, 550, "FX VOLUME", 50, true, imageSETTINGS);
	labelSETTINGS = App->ui_manager->CreateLabel(imageSETTINGS->width / 2, 50, "SETTINGS", 60, true, imageSETTINGS);
	labelSETTINGS->Local_pos.x -= labelSETTINGS->width / 2;
	labelGENERALSOUND = App->ui_manager->CreateLabel(100, 350, "GENERAL SOUND", 50, true, imageSETTINGS);
	sliderGENERALSOUND = App->ui_manager->CreateSlider(380, 352, 50, imageSETTINGS);
	checkboxSOUND = App->ui_manager->CreateCheckBox(380, 257, imageSETTINGS);
	if (!App->audio->NoAudio)
		checkboxSOUND->pressed = true;
	else checkboxSOUND->pressed = false;
	labelSOUND = App->ui_manager->CreateLabel(100, 250, "SOUND", 50, true, imageSETTINGS);
	if (App->capactivated)
		checkboxFPS->pressed = true;
}


void j1Menu::SettingsMenu(float dt)
{
	Title->NoRenderLabel = true;
	sentence->NoRenderLabel = true;
	if (imageSETTINGS->Local_pos.y <= buttonEXIT->Local_pos.y + buttonEXIT->height) {
		buttonEXIT->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y <= buttonHACKS->Local_pos.y + buttonHACKS->height) {
		buttonHACKS->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y <= buttonCREDITS->Local_pos.y + buttonCREDITS->height) {
		buttonCREDITS->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y <= buttonSETTINGS->Local_pos.y + buttonSETTINGS->height) {
		buttonSETTINGS->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y <= buttonCONTINUE->Local_pos.y + buttonCONTINUE->height) {
		buttonCONTINUE->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y <= buttonSTART->Local_pos.y + buttonSTART->height) {
		buttonSTART->WantToRender = false;
	}
	if (imageSETTINGS->Local_pos.y >= buttonEXIT->Local_pos.y) {
		buttonEXIT->WantToRender = true;
	}
	if (imageSETTINGS->Local_pos.y >= buttonCREDITS->Local_pos.y) {
		buttonCREDITS->WantToRender = true;
	}
	if (imageSETTINGS->Local_pos.y >= buttonHACKS->Local_pos.y) {
		buttonHACKS->WantToRender = true;
	}
	if (imageSETTINGS->Local_pos.y >= buttonSETTINGS->Local_pos.y) {
		buttonSETTINGS->WantToRender = true;
	}
	if (imageSETTINGS->Local_pos.y >= buttonCONTINUE->Local_pos.y) {
		buttonCONTINUE->WantToRender = true;
	}
	if (imageSETTINGS->Local_pos.y >= buttonSTART->Local_pos.y) {
		buttonSTART->WantToRender = true;
	}
	if (!Positioned && !SettingMenuDone) { //MENU GOING UP
		imageSETTINGS->Local_pos.y -= 1000 * dt;
		if (imageSETTINGS->Local_pos.y <= 100) {
			Positioned = true;
			//buttonGOBACKSETTINGS->pressed = false;
			SettingMenuDone = true;
		}
	}
	if (!Positioned && SettingMenuDone) { //MENU GOING DOWN

		imageSETTINGS->Local_pos.y += 2000 * dt;
		if (imageSETTINGS->Local_pos.y >= 1225) {
			buttonSTART->NoUse = false;
			buttonCONTINUE->NoUse = false;
			buttonSETTINGS->NoUse = false;
			buttonEXIT->NoUse = false;
			buttonHACKS->NoUse = false;
			buttonCREDITS->NoUse = false;
			InSettings = false;
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
	}
}

void j1Menu::CreatehacksButtons()
{
	HacksMenuDone = false;
	imageHACKS = App->ui_manager->CreateImage(170, -700, true);
	imageHACKS->SetSpritesData({ 758,0,705,671 });
	imageHACKS->type = BUTTON;
	buttonGOBACKHACKS = App->ui_manager->CreateButton(37, 40, 3, imageHACKS);
	buttonGOBACKHACKS->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
	labelGODMODE = App->ui_manager->CreateLabel(100, 150, "GODMODE", 50, true, imageHACKS);
	checkboxGODMODE = App->ui_manager->CreateCheckBox(380, 157, imageHACKS);
	checkboxNOENEMIES = App->ui_manager->CreateCheckBox(380, 257, imageHACKS);
	labelNOENEMIES = App->ui_manager->CreateLabel(100, 250, "NO ENEMIES", 50, true, imageHACKS);
	checkboxSTARTLEVEL2 = App->ui_manager->CreateCheckBox(380, 357, imageHACKS);
	labelSTARTLEVEL2 = App->ui_manager->CreateLabel(100, 350, "START IN LEVEL 2", 50, true, imageHACKS);
	labelHACKS = App->ui_manager->CreateLabel(imageHACKS->width / 2, 50, "HACKS", 60, true, imageHACKS);
	labelHACKS->Local_pos.x -= labelHACKS->width / 2;


	
}

void j1Menu::CreateCredits()
{

	imageCREDITS = App->ui_manager->CreateImage(10, 880, true);
	imageCREDITS->SetSpritesData({ 1480,0,1000,1360 });
	buttonGOBACKCREDITS = App->ui_manager->CreateButton(40, 1400, 1, imageCREDITS);
	buttonGOBACKCREDITS->SetSpritesData({ 559,0,39,31 }, { 652,0,39,31 }, { 608,0,39,28 });
	buttonGITHUB = App->ui_manager->CreateButton(60, 1550, 1, imageCREDITS,"GAME'S GITHUB", 30);
	buttonORIOLGIT = App->ui_manager->CreateButton(750, 1750, 1, imageCREDITS,"ORIOL'S GITHUB", 30);
	buttonVICTORGIT = App->ui_manager->CreateButton(60, 1950, 1, imageCREDITS, "VICTOR'S GITHUB",30);
}

void j1Menu::Credits(float dt)
{
	if (buttonGOBACKCREDITS->Scree_pos.y <= 30) {
		if (buttonGOBACKCREDITS->pressed) {
			DeleteCredits();
			CreateCredits();
			InCredits = false;
			InMainMenu = true;
			WantToDisappearMainMenu(false);
		}
		if (buttonGITHUB->pressed) {
			ShellExecute(NULL, "open", "https://github.com/VictorSegura99/Alien-Earth", NULL, NULL, SW_SHOWNORMAL);
		}
		if (buttonORIOLGIT->pressed) {
			ShellExecute(NULL, "open", "https://github.com/OriolCS2", NULL, NULL, SW_SHOWNORMAL);
		}
		if (buttonVICTORGIT->pressed) {
			ShellExecute(NULL, "open", "https://github.com/VictorSegura99", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else {
		imageCREDITS->Local_pos.y -= 200 * dt;
	}
		

}

void j1Menu::DeleteCredits()
{
	App->ui_manager->DeleteUI_Element(imageCREDITS);
	App->ui_manager->DeleteUI_Element(buttonGOBACKCREDITS);
	App->ui_manager->DeleteUI_Element(buttonGITHUB);
	App->ui_manager->DeleteUI_Element(buttonORIOLGIT);
	App->ui_manager->DeleteUI_Element(buttonVICTORGIT);
	App->ui_manager->DeleteUI_Element(buttonVICTORGIT->label);
	App->ui_manager->DeleteUI_Element(buttonORIOLGIT->label);
	App->ui_manager->DeleteUI_Element(buttonGITHUB->label);
}

void j1Menu::HacksMenu(float dt)
{
	Title->NoRenderLabel = true;
	sentence->NoRenderLabel = true;
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonEXIT->Local_pos.y) {
		buttonEXIT->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonHACKS->Local_pos.y) {
		buttonHACKS->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonCREDITS->Local_pos.y) {
		buttonCREDITS->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonSETTINGS->Local_pos.y) {
		buttonSETTINGS->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonCONTINUE->Local_pos.y) {
		buttonCONTINUE->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height >= buttonSTART->Local_pos.y) {
		buttonSTART->WantToRender = false;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonEXIT->Local_pos.y) {
		buttonEXIT->WantToRender = true;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonCREDITS->Local_pos.y) {
		buttonCREDITS->WantToRender = true;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonHACKS->Local_pos.y) {
		buttonHACKS->WantToRender = true;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonSETTINGS->Local_pos.y) {
		buttonSETTINGS->WantToRender = true;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonCONTINUE->Local_pos.y) {
		buttonCONTINUE->WantToRender = true;
	}
	if (imageHACKS->Local_pos.y + imageHACKS->height <= buttonSTART->Local_pos.y) {
		buttonSTART->WantToRender = true;
	}
	if (!positioned && !HacksMenuDone) { //MENU GOING UP
		imageHACKS->Local_pos.y += 1000 * dt;
		if (imageHACKS->Local_pos.y + imageHACKS->height >= 700) {
			positioned = true;
			buttonGOBACKHACKS->pressed = false;
			HacksMenuDone = true;
		}
	}
	if (!positioned && HacksMenuDone) { //MENU GOING DOWN

		imageHACKS->Local_pos.y -= 2000 * dt;
		if (imageHACKS->Local_pos.y <= -700) {
			buttonSTART->NoUse = false;
			buttonCONTINUE->NoUse = false;
			buttonSETTINGS->NoUse = false;
			buttonHACKS->NoUse = false;
			buttonEXIT->NoUse = false;
			buttonCREDITS->NoUse = false;
			InHacks = false;
			InMainMenu = true;
		}
	}

	if (HacksMenuDone) { //MENU LOGIC BUTTONS
		if (buttonGOBACKHACKS->pressed) {
			positioned = false;
		}
		if (checkboxGODMODE->pressed && !App->entitymanager->GetPlayerData()->God) {
			App->entitymanager->GetPlayerData()->God = true;
		}
		if (!checkboxGODMODE->pressed && App->entitymanager->GetPlayerData()->God) {
			App->entitymanager->GetPlayerData()->God = false;
		}
		if (checkboxNOENEMIES->pressed && App->scene->WantToSpawnEnemies) {
			App->scene->WantToSpawnEnemies = false;
		}
		if (!checkboxNOENEMIES->pressed && !App->scene->WantToSpawnEnemies) {
			App->scene->WantToSpawnEnemies = true;
		}
		if (checkboxSTARTLEVEL2->pressed && App->scene->KnowMap != 1) {
			App->scene->KnowMap = 1;
		}
		if (!checkboxSTARTLEVEL2->pressed && App->scene->KnowMap != 0) {
			App->scene->KnowMap = 0;
		}
	}
}

void j1Menu::StartLevel()
{
	Mix_FadeOutMusic(1000);
	App->ui_manager->DeleteAllUI();
	if (App->scene->KnowMap == 0)
		App->entitymanager->GetPlayerData()->Intro = true;
	App->fade->FadeToBlack(3.0f);
	GoStart = true;
}






