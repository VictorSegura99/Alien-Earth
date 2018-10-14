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
	JumpFx = config.child("JumpFx").text().as_string();
	WaterFx = config.child("WaterFx").text().as_string();
	DeathFx = config.child("DeathFx").text().as_string();
	initialXmap1 = config.child("positionXmap1").attribute("x").as_float();
	initialYmap1 = config.child("positionYmap1").attribute("y").as_float();
	initialXmap2 = config.child("positionXmap2").attribute("x").as_float();
	initialYmap2 = config.child("positionYmap2").attribute("y").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	positionWinMap1 = config.child("positionWinMap1").attribute("value").as_int();
	startpointcameramap2 = config.child("startpointcameramap2").attribute("value").as_int();
	SpeedSwimLeftRight = config.child("SpeedSwimLeftRight").attribute("value").as_float();
	SpeedSwimUp = config.child("SpeedSwimUp").attribute("value").as_float();
	SpeedClimb = config.child("SpeedClimb").attribute("value").as_float();
	SpeedWalk = config.child("SpeedWalk").attribute("value").as_float();
	playerHeight = config.child("playerHeight").attribute("value").as_int();
	SpeedSwimDown = config.child("SpeedSwimDown").attribute("value").as_float();
	JumpTime = config.child("JumpTime").attribute("value").as_int();
	JumpSpeed = config.child("JumpSpeed").attribute("value").as_float();
	playerwidth = config.child("playerwidth").attribute("value").as_int();
	playerheight = config.child("playerheight").attribute("value").as_int();
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

	jumpfx = App->audio->LoadFx(JumpFx.GetString());
	waterfx = App->audio->LoadFx(WaterFx.GetString());
	deathfx = App->audio->LoadFx(DeathFx.GetString());

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
	

	texture = App->tex->Load(sprites_name.GetString());
	current_animation = &idle;	
	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	coll = App->collision->AddCollider({ 0, 0, playerwidth, playerheight }, COLLIDER_PLAYER, this);
	return ret;


	
	
}
bool jPlayer::PreUpdate()
{
	WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	GoUp = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	GoDown = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	if (!God) 
		Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	else Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
	if (!WalkLeft && !WalkRight && !CanSwim && !CanClimb)
		Idle = true;
	else
		Idle = false;
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		God = !God;

	return true;
}
bool jPlayer::Update(float dt)
{
	position.y -= gravity;
	if (Jump && CanJump && !CanSwim && !God && !IsJumping) {
		IsJumping = true;
		App->audio->PlayFx(jumpfx);
	}
	if (IsJumping) {
		Time += 1;
		CanJump = false;
		if (Time <= JumpTime && WalkRight) {
			current_animation = &jumpR;
			position.y -= JumpSpeed;
		}
		else if (Time <= JumpTime && WalkLeft) {
			current_animation = &jumpL;
			position.y -= JumpSpeed;
		}
		else if (Time <= JumpTime) {
			if (current_animation==&idle)
				current_animation = &jumpR;
			if (current_animation == &idle2)
				current_animation = &jumpL;
			position.y -= JumpSpeed;
		}
		else
			IsJumping = false;
	}
	if (God && Jump) {
		//App->audio->PlayFx(jumpfx);
		position.y -= JumpSpeed;
	}
	if (CanSwim && GoUp) {
		position.y -= SpeedSwimUp;
	}
	if (CanSwim && GoDown) {
		position.y += SpeedSwimDown;
	}
	if (WalkRight) {
		if (!IsJumping && !CanSwim) {
			position.x += SpeedWalk;
			current_animation = &GoRight;
		}
		if (IsJumping) {
			position.x += SpeedWalk;
		}
		if (CanSwim && !CanClimb) {
			position.x += SpeedSwimLeftRight;
			current_animation = &SwimRight;
		}

	}
	if (Idle) {
		if (current_animation == &GoRight)
			current_animation = &idle;
		if (current_animation == &GoLeft)
			current_animation = &idle2;
	}

	if (CanClimb && GoUp) {
		position.y -= SpeedClimb;
		current_animation = &Climb;
	}
	if (CanClimb && GoDown) {
		position.y += SpeedClimb;
		current_animation = &Climb;
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle;
	if (WalkLeft) {
		if (!IsJumping && !CanSwim) {
			position.x -= SpeedWalk;
			current_animation = &GoLeft;
		}
		if (IsJumping) {
			position.x -= SpeedWalk;
		}
		if (CanSwim && !CanClimb) {
			position.x -= SpeedSwimLeftRight;
			current_animation = &SwimLeft;
		}
	}
	if (WalkRight && WalkLeft) {
		if (!CanSwim)
			current_animation = &idle;
		if (CanSwim)
			current_animation = &SwimRight;
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
	if (death && !God) 
		Die();
	if (fall && !God)
		Fall();
	if (God)
		CanJump = true;
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
	God = false;
	Death.Reset();
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
			Time = 0;
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
		if (position.y + playerHeight < c2->rect.y) {
			position.y += gravity;
			CanJump = true;
			Time = 0;
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
		App->audio->PlayFx(waterfx);
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
	App->audio->PlayFx(deathfx);
	if (Death.SeeCurrentFrame()==10) {
		if (App->scene->KnowMap == 0) {
			CleanUp();
			Disable();
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Start();
		}
		if (App->scene->KnowMap == 1) {
			CleanUp();
			Disable();
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Start();
		}
	}
}

void jPlayer::Fall()
{
	if (App->scene->KnowMap == 0) {
		CleanUp();
		Disable();
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Start();
	}
	if (App->scene->KnowMap == 1) {
		CleanUp();
		Disable();
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Start();
	}	
}

void jPlayer::AddFx(int fx, int repeat) {
	App->audio->PlayFx(fx, repeat);
}






