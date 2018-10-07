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

jPlayer::jPlayer() : j1Module()
{
	name.create("player");
}

jPlayer::~jPlayer() {}

bool jPlayer::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	pugi::xml_node sprite = config.child("sprites");
	data->create(sprite.attribute("name").as_string());
	sprites_name.add(data);
	initialX = config.child("positionX").attribute("x").as_float();
	initialY = config.child("positionY").attribute("y").as_float();
	bool ret = true;
	return ret;
}

bool jPlayer::Start()
{
	bool ret = true;

	position.x = initialX;
	position.y = initialY;

	idle.PushBack({ 142,0,66,86 });
	idle2.PushBack({ 353,0,66,86 });
	
	GoRight.PushBack({ 0,0,67,86 });
	GoRight.PushBack({ 69,0,70,86 });
	GoRight.speed = 0.03f;

	GoLeft.PushBack({ 285,0,67,86 });
	GoLeft.PushBack({ 212,0,70,87 });
	GoLeft.speed = 0.03f;

	texture = App->tex->Load(sprites_name.start->data->GetString());

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
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		position.x -= 1.0f;
		current_animation = &GoLeft;
		anime = false;
	}
	else if (anime==false)
	{
			current_animation = &idle2;
	}
	else if(anime)
	{
		current_animation = &idle;
	}

	
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{
		position.x = 30;
		position.y = 405;
	}

	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	if (position.x >= 10085) {
		NextMap = true;
	}
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





