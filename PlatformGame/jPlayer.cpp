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

	jumpR.PushBack({ 420,0,67,86 });


	jumpL.PushBack({ 420,88,67,86 });


	Climb.PushBack({ 488,0,64,86 });
	Climb.PushBack({ 553,0,64,86 });
	Climb.speed = 0.03f;

	ClimbIdle.PushBack({ 488,0,64,86 });

	SwimRight.PushBack({ 617,0,70,86 });
	SwimRight.PushBack({ 617,88,70,86 });
	SwimRight.speed = 0.03f;

	SwimLeft.PushBack({ 617,176,70,86 });
	SwimLeft.PushBack({ 617,263,70,86 });
	SwimLeft.speed = 0.03f;

	Death.PushBack({ 0,94,68,81 });
	Death.PushBack({ 73,105,70,70 });
	Death.speed=0.03f;

	texture = App->tex->Load(sprites_name.GetString());
	current_animation = &idle;
	if (texture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	coll = App->collision->AddCollider({ 0, 0, 65, 87 }, COLLIDER_PLAYER, this);
	return ret;

	//audio
	App->audio->LoadFx("audio/fx/Jump_fx.wav");
	
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
		App->audio->PlayFx(1, 1);
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
		position.x += 5.0f;
		if (!IsJumping && !CanSwim)
			current_animation = &GoRight;
		if(CanSwim && !CanClimb)
			current_animation = &SwimRight;

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
		position.x -= 5.0f;
		if (!IsJumping && !CanSwim)
			current_animation = &GoLeft;
		if (CanSwim && !CanClimb)
			current_animation = &SwimLeft;
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
	NextMap = false;
	if (coll)
		coll->to_delete = true;
	return true;
}

void jPlayer::OnCollision(Collider * c1, Collider * c2)
{
		if (coll == c1 && c2->type == COLLIDER_WALL_LEFT) {
			WalkLeft = false;
		}
		if (coll == c1 && c2->type == COLLIDER_GROUND && position.y + 79< c2->rect.y) {
			position.y += gravity;
			GoDown = false;
			CanJump = true;
			CanSwim = false;
			CanClimb = false;
			JumpTime = 0;
		}
		if (coll == c1 && c2->type == COLLIDER_WALL_RIGHT) {
			WalkRight = false;
		}
		if (coll == c1 && c2->type == COLLIDER_CLIMB) {
			CanClimb = true;
			CanJump = true;
			position.y += gravity;
		}
		if (coll == c1 && c2->type == COLLIDER_WALL_UP) {
			GoUp = false;
			CanClimb = false;
		}
		if (coll == c1 && c2->type == COLLIDER_WATER) {
			CanSwim = true;
			CanClimb = false;
			position.y += gravity;
		}
		if (coll == c1 && c2->type == COLLIDER_NONE) {
			CanClimb = false;
			CanSwim = false;
		}
}





