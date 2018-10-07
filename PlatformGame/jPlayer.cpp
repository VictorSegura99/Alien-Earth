#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "jPlayer.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1input.h"

jPlayer::jPlayer() : j1Module()
{
	name.create("player");

	
	

}


jPlayer::~jPlayer()
{
}


bool jPlayer::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	//config = config.child("player");
	
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

	
	idle.PushBack({ 147,0,65,82 });
	
	GoRight.PushBack({ 0,0,67,83 });
	GoRight.PushBack({ 72,0,70,86 });
	GoRight.speed = 0.03f;

	GoLeft.PushBack({ 292,0,67,83 });
	GoLeft.PushBack({ 217,0,70,86 });
	GoLeft.speed = 0.03f;

	texture = App->tex->Load(sprites_name.start->data->GetString());

	

	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	return ret;
}

bool jPlayer::PreUpdate()
{

	
	return true;
}

bool jPlayer::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		position.x += 2.0f;
		current_animation = &GoRight;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		position.x -= 2.0f;
		current_animation = &GoLeft;
	}
	else {
		current_animation = &idle;
	}

	
	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	return true;
}

bool jPlayer::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}





