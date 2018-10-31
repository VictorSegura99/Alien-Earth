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



	return true;
}

// Called each loop iteration
bool j1Choose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Choose::Update(float dt)
{




	return true;
}

// Called each loop iteration
bool j1Choose::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}

// Called before quitting
bool j1Choose::CleanUp()
{

	LOG("Freeing scene");
	return true;
}


