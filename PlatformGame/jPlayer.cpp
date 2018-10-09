#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "jPlayer.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1input.h"
#include "j1Map.h"
#include "j1Scene.h"

jPlayer::jPlayer() : j1Module()
{
	name.create("player");
}

jPlayer::~jPlayer() {}

bool jPlayer::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	sprites_name = config.child("sprites").text().as_string();
	initialXmap1 = config.child("positionXmap1").attribute("x").as_float();
	initialYmap1 = config.child("positionYmap1").attribute("y").as_float();
	initialXmap2 = config.child("positionXmap2").attribute("x").as_float();
	initialYmap2 = config.child("positionYmap2").attribute("y").as_float();
	bool ret = true;
	return ret;
}

bool jPlayer::Start()
{
	bool ret = true;
	if (App->scene->KnowMap == 0) {
		position.x = initialXmap1;
		position.y = initialYmap1;
	}
	else if (App->scene->KnowMap == 1) {
		position.x = initialXmap2;
		position.y = initialYmap2;
	}

	idle.PushBack({ 142,0,66,86 });

	idle2.PushBack({ 353,0,66,86 });
	
	GoRight.PushBack({ 0,0,67,86 });
	GoRight.PushBack({ 69,0,70,86 });
	GoRight.speed = 0.03f;

	GoLeft.PushBack({ 285,0,67,86 });
	GoLeft.PushBack({ 212,0,70,87 });
	GoLeft.speed = 0.03f;

	texture = App->tex->Load(sprites_name.GetString());
	current_animation = &idle;
	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	return ret;
}
bool jPlayer::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		position.x += 1.0f;
		current_animation = &GoRight;
		anime = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		position.x -= 1.0f;
		current_animation = &GoLeft;
		anime = false;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			current_animation = &idle;
		}
	}
	if (anime==false && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			current_animation = &idle2;
			
	}
	if(anime && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		current_animation = &idle;
	}
	if (App->scene->KnowMap == 0) {
		if (position.x >= positionWinMap1) {
			NextMap = true;
		}
	}
	if (position.x <= 560) {
		if (App->scene->KnowMap == 1) {
			App->render->camera.x = startpointcameramap2;
		}
	}
	else {
		App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	App->render->camera.y = -position.y + (App->render->camera.h / 2);
	
	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	
	
	return true;
}
bool jPlayer::PostUpdate()
{
	
	return true;
}
bool jPlayer::CleanUp()
{
	App->tex->UnLoad(texture);
	anime = true;
	NextMap = false;
	return true;
}





