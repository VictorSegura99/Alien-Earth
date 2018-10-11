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
#include "j1Collision.h"
#include "j1Audio.h"

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

	Jump.PushBack({ 420,0,67,86 });

	Climb.PushBack({ 488,0,64,86 });
	Climb.PushBack({ 553,0,64,86 });
	Climb.speed = 0.03f;

	Climb.PushBack({ 488,0,64,86 });
	Climb.PushBack({ 553,0,64,86 });
	Climb.speed = 0.03f;

	SwimRight.PushBack({ 617,0,70,86 });
	SwimRight.PushBack({ 617,88,70,86 });
	SwimRight.speed = 0.03f;

	SwimLeft.PushBack({ 617,176,70,86 });
	SwimLeft.PushBack({ 617,263,70,86 });
	SwimLeft.speed = 0.03f;

	texture = App->tex->Load(sprites_name.GetString());
	current_animation = &idle;
	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	coll = App->collision->AddCollider({ 0, 0, 70, 87 }, COLLIDER_PLAYER, this);
	return ret;

	//audio
	jumpfx=App->audio->LoadFx("audio/fx/Jump_fx.wav");
}
bool jPlayer::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		KnowState = GOLEFT;
	}
	else {
		KnowState = IDLE;
	}

	return true;
}
bool jPlayer::Update(float dt)
{
	position.y -= gravity;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		IsJumping = true;
		position.y -= 20.5f;
		App->audio->PlayFx(jumpfx);
		current_animation = &Jump;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		position.x += 5.0f;
		current_animation = &GoRight;
		anime = true;
	}
	if (KnowState == IDLE) {
		position.x += 0.0f;
		position.y += 0.0f;
		current_animation = &idle;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		position.y -= 10.0f;
		current_animation = &Climb;
	}
	if (KnowState == GOLEFT) {
		position.x -= 5.0f;
		current_animation = &GoLeft;
		anime = false;
	}
	if (anime==false && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
			current_animation = &idle2;
			
	}
	if(anime && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		current_animation = &idle;
	}
	if (App->scene->KnowMap == 0 && position.x >= positionWinMap1) {
			NextMap = true;
	}
	if (position.x <= 560 && App->scene->KnowMap == 1) { //If player is in a position where the camera would print out of the map, camera stops
			App->render->camera.x = startpointcameramap2;
	}
	else {
		App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	if (position.y <= 450) { //If player is in a position where the camera would print out of the map, camera stops
		App->render->camera.y = 0;
	}
	else if (position.y >= 750) {
		App->render->camera.y = -290;
	}
	else {
		App->render->camera.y = -position.y + (App->render->camera.h / 2);
	}
	

	coll->SetPos(position.x, position.y);

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
	if (coll)
		coll->to_delete = true;
	return true;
}

void jPlayer::OnCollision(Collider * c1, Collider * c2)
{
		position.y += gravity;
		if (coll == c1 && c2->type == COLLIDER_WALL) {
			KnowState = IDLE;
		}
}





