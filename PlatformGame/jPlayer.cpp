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
	JumpFx = config.child("JumpFx").attribute("source").as_string();
	DeathFx = config.child("DeathFx").attribute("source").as_string();
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
	GoRight.speed = 0.1f;

	GoLeft.PushBack({ 285,0,67,86 });
	GoLeft.PushBack({ 212,0,70,87 });
	GoLeft.speed = 0.1f;

	jumpR.PushBack({ 420,0,67,86 });


	jumpL.PushBack({ 420,88,67,86 });


	Climb.PushBack({ 488,0,64,86 });
	Climb.PushBack({ 553,0,64,86 });
	Climb.speed = 0.1f;

	ClimbIdle.PushBack({ 488,0,64,86 });

	SwimRight.PushBack({ 617,0,70,86 });
	SwimRight.PushBack({ 617,88,70,86 });
	SwimRight.speed = 0.1f;

	SwimLeft.PushBack({ 617,176,70,86 });
	SwimLeft.PushBack({ 617,263,70,86 });
	SwimLeft.speed = 0.1f;

	Death.PushBack({ 0,94,68,81 });
	Death.PushBack({ 73,94,68,81 });
	Death.PushBack({ 142,94,68,81 });
	Death.PushBack({ 213,94,68,81 });
	Death.PushBack({ 283,94,68,81 });
	Death.PushBack({ 351,94,68,81 });
	Death.PushBack({ 0,175,68,81 });
	Death.PushBack({ 69,175,68,81 });
	Death.PushBack({ 139,175,68,81 });
	Death.PushBack({ 206,175,68,81 });
	Death.PushBack({ 272,175,68,81 });
	Death.speed=0.1f;
	Death.loop = false;
	Death.Reset();

	texture = App->tex->Load(sprites_name.GetString());
	current_animation = &idle;	
	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	coll = App->collision->AddCollider({ 0, 0, 65, 87 }, COLLIDER_PLAYER, this);
	return ret;

	//audio
	App->audio->LoadFx(JumpFx.GetString());
	
}
bool jPlayer::PreUpdate()
{
	WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	GoUp = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	GoDown = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;

	if (!WalkLeft && !WalkRight && !IsJumping && !CanSwim && !CanClimb)
		Idle = true;
	else
		Idle = false;
	

	return true;
}
bool jPlayer::Update(float dt)
{
	position.y -= gravity;
	if (Jump && CanJump && !CanSwim) {
		IsJumping = true;
		CanJump = false;
		Jump = false;
	}
	if (IsJumping) {
		JumpTime += 1;
		AddFx(1, 0);
		if (JumpTime <= 30 && WalkRight) {
			current_animation = &jumpR;
			position.y -= 10.0f;
		}
		else if (JumpTime <= 30 && WalkLeft) {
			current_animation = &jumpL;
			position.y -= 10.0f;
		}
		else if (JumpTime <= 30) {	
			current_animation = &jumpR;
			position.y -= 10.0f;
		}
		else
			IsJumping = false;
	}
	if (CanSwim && GoUp) {
		position.y -= 7.0f;
	}
	if (CanSwim && GoDown) {
		position.y += 2.0f;
	}
	if (WalkRight) {
		if (!IsJumping && !CanSwim) {
			position.x += 8.0f;
			current_animation = &GoRight;
		}
		if (IsJumping) {
			position.x += 8.0f;
		}
		if (CanSwim && !CanClimb) {
			position.x += 4.0f;
			current_animation = &SwimRight;
		}

	}
	if (Idle) {
		position.x += 0.0f;
		position.y += 0.0f;
		current_animation = &idle;
	}
	if (CanClimb && GoUp) {
		position.y -= 4.0f;
		current_animation = &Climb;
	}
	if (CanClimb && GoDown) {
		position.y += 4.0f;
		current_animation = &Climb;
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle;
	if (WalkLeft) {
		if (!IsJumping && !CanSwim) {
			position.x -= 8.0f;
			current_animation = &GoLeft;
		}
		if (IsJumping) {
			position.x -= 8.0f;
		}
		if (CanSwim && !CanClimb) {
			position.x -= 8.0f;
			current_animation = &SwimLeft;
		}
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
	else if (position.y >= 740	) {
		App->render->camera.y = -290;
	}
	else {
		App->render->camera.y = -position.y + (App->render->camera.h / 2);
	}
	if (death) 
		Die();
	if (fall)
		Fall();
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
	NextMap = false;
	death = false;
	fall = false;
	if (coll)
		coll->to_delete = true;
	return true;
}

void jPlayer::OnCollision(Collider * c1, Collider * c2)
{
	switch (c2->type) {
	case COLLIDER_GROUND:
		if (position.y < c2->rect.y + c2->rect.h) {
			position.y += gravity;
			CanJump = true;
			JumpTime = 0;
			CanSwim = false;
			GoDown = false;
			CanClimb = false;
		}
		break;
	case COLLIDER_WALL_UP:
		GoUp = false;
		break;
	case COLLIDER_WALL:
		if (position.x < c2->rect.x) {
			WalkRight = false;
		}
		else if (position.x > c2->rect.x) {
			WalkLeft = false;
		}
		break;
	case COLLIDER_PLATFORM:
		if (position.y + 81 <= c2->rect.y) {
			position.y += gravity;
			CanJump = true;
			JumpTime = 0;
			CanSwim = false;
			IsJumping = false;
		}
		break;
	case COLLIDER_CLIMB:
		CanClimb = true;
		CanJump = true;
		position.y += gravity;
		break;
	case COLLIDER_WATER:
		CanSwim = true;
		CanClimb = false;
		position.y += gravity;
		break;
	case COLLIDER_NONE:
		CanClimb = false;
		CanSwim = false;
		break; 
	case COLLIDER_SPIKES:
		position.y += gravity;
		WalkLeft = false;
		WalkRight = false;
		GoUp = false;
		GoDown = false;
		CanJump = false;
		death = true;
		break;
	case COLLIDER_FALL:
		WalkLeft = false;
		WalkRight = false;
		GoUp = false;
		GoDown = false;
		fall = true;
		break;
	}
}
void jPlayer::Die()
{
	current_animation = &Death;
	if (Death.Finished()) {
		if (App->scene->KnowMap == 0) {
			CleanUp();
			App->map->CleanUp();
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Start();
		}
		if (App->scene->KnowMap == 1) {
			CleanUp();
			App->map->CleanUp();
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Start();
		}
	}
}

void jPlayer::Fall()
{
	if (App->scene->KnowMap == 0) {
		CleanUp();
		App->map->CleanUp();
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Start();
	}
	if (App->scene->KnowMap == 1) {
		CleanUp();
		App->map->CleanUp();
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Start();
	}	
}

void jPlayer::AddFx(int fx, int repeat) {
	App->audio->PlayFx(fx, repeat);
}






