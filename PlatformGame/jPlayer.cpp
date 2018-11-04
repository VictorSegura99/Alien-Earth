#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "jPlayer.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Choose.h"
#include "j1Map.h"
#include "j1Window.h"


jPlayer::jPlayer() : j1Module()
{
	name.create("player");

}

jPlayer::~jPlayer() {}

bool jPlayer::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");
	sprites_name[0] = config.child("sprites").text().as_string();
	sprites_name[1] = config.child("sprites2").text().as_string();
	sprites_name[2] = config.child("sprites3").text().as_string();
	JumpFx = config.child("JumpFx").text().as_string();
	WaterFx = config.child("WaterFx").text().as_string();
	DeathFx = config.child("DeathFx").text().as_string();
	DeathFx2 = config.child("DeathFx2").text().as_string();
	LadderFx = config.child("LadderFx").text().as_string();
	finalmapplayer = config.child("finalmapplayer").attribute("value").as_int();
	finalmap = config.child("finalmap").attribute("value").as_int();
	startmap2 = config.child("startmap2").attribute("value").as_int();
	maxYcam = config.child("maxYcam").attribute("value").as_int();
	minYcam = config.child("minYcam").attribute("value").as_int();
	lowcam = config.child("lowcam").attribute("value").as_int();
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
	//if (App->scene->KnowMap == 0) {

	//}
/*	else if (App->scene->KnowMap == 1) {
		position.x = initialXmap2;
		position.y = initialYmap2;
	}*/

	jumpfx = App->audio->LoadFx(JumpFx.GetString());
	waterfx = App->audio->LoadFx(WaterFx.GetString());
	deathfx = App->audio->LoadFx(DeathFx.GetString());
	deathfx2 = App->audio->LoadFx(DeathFx2.GetString());
	ladderfx = App->audio->LoadFx(LadderFx.GetString());

	position.x = initialmap1.x;
	position.y = initialmap1.y;

	LoadPushbacks();

	laser.anim.PushBack({ 142,0,66,86 });
	laser.anim.speed = 0.2f;
	laser.anim.loop = true;
	laser.velocity.x = 10.0f;
	laser.life = 0;
	
	
	
	return ret;
}
bool jPlayer::PreUpdate() //Here we preload the input functions to determine the state of the player
{
	if (!NoInput) {
		WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
		WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
		GoUp = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
		GoDown = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
		Laser = App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN;
		DoDash = App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN;
		if (!God)
			Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
		else Jump = App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
		if (!WalkLeft && !WalkRight && !CanSwim && !CanClimb)
			Idle = true;
		else
			Idle = false;
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			God = !God;
	}
	return true;
}
bool jPlayer::Update(float dt)
{
	
	position.y -= gravity;
	if (!dashing) {
		GoJump();
		GoSwim();
		GoClimb();
		Move_Left_Right();
		ShootLaser();

	}
	Camera();
	if (DoDash) {
		dashing = true;
		dash.ResetDashAnims();
	}
	if (dashing) {
		if (dash.StartDash.current_frame == 0) {
		//	position.x -= 60;
			current_animation = &dash.StartDash;
		}
		if (dash.StartDash.SeeCurrentFrame() == 3) {
			++dash.DashCont;
			current_animation = &dash.Dashing;
			position.x += 20;
			if (dash.DashCont >= dash.DashTime) {
				current_animation = &dash.FinishDash;
				dash.DashCont = 0;
				dashing = false;
			}
		}
	}

	if (death && !God) {
		death = false;
		//App->audio->PlayFx(deathfx2);
		Die();
	}
	if (fall && !God) {
		fall = false;
		App->audio->PlayFx(deathfx2);
		Fall();
	}
	if (God)
		CanJump = true;
	
	coll->SetPos(position.x, position.y);
	//App->render->DrawQuad(rect, 150, 150, 150, 255, true, false);
	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	
	return true;
}

bool jPlayer::PostUpdate()
{
	/*if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) {
		App->scene->active = !App->scene->active;
		App->player->active = !App->player->active;
		App->collision->active = !App->collision->active;
		App->map->active = !App->map->active;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->choose->GameOn = false;
	}*/
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		ChangePlayer(0);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		ChangePlayer(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		ChangePlayer(2);
	}
	return true;
}
bool jPlayer::Load(pugi::xml_node& player)
{
	position.x = player.child("position").attribute("x").as_float();
	position.y = player.child("position").attribute("y").as_float();
	
	
	App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);


	return true;
}
bool jPlayer::Save(pugi::xml_node& player) const
{
	player.append_child("position").append_attribute("x") = position.x;
	player.child("position").append_attribute("y") = position.y;


	return true;
}
bool jPlayer::CleanUp()
{
	App->tex->UnLoad(texture);
	NextMap = false;
	death = false;
	fall = false;
	God = false;
	Death[NumPlayer].current_frame = 0.0f;
	Death[NumPlayer].loops = 0;
	if (coll)
		coll->to_delete = true;
	return true;
}

void jPlayer::OnCollision(Collider * c1, Collider * c2) //this determine what happens when the player touch a type of collider
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
	case COLLIDER_WALL_LEFT:
		if (!CanSwim && !CanClimb)
			position.x += SpeedWalk;
		if (CanSwim)
			position.x += SpeedSwimLeftRight;
		if (CanClimb)
			position.x += SpeedWalk;
		break;
	case COLLIDER_WALL_RIGHT:
		if (!CanSwim && !CanClimb)
			position.x -= SpeedWalk;
		if (CanSwim)
			position.x += SpeedSwimLeftRight;
		if (CanClimb) 
			position.x -= SpeedWalk;
		break;
	case COLLIDER_PLATFORM:
		if (position.y + playerHeight < c2->rect.y) {
			position.y += gravity;
			CanJump = true;
			Time = 0;
			CanSwim = false;
			IsJumping = false;
			current_animation = &idle[NumPlayer];
		}
		break;
	case COLLIDER_CLIMB:
		App->audio->PlayFx(ladderfx);
		CanClimb = true;
		CanJump = true;
		Time = 50;
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
		if (!God)
			NoInput = true;
		break;
	case COLLIDER_FALL:
		WalkLeft = false;
		WalkRight = false;
		GoUp = false;
		GoDown = false;
		fall = true;
		if (!God)
			NoInput = true;
		break;	
	case COLLIDER_ROPE:
		CanClimb = true;
		CanJump = true;
		position.y += gravity;
		break;
	case COLLIDER_WIN:
		CanClimb = false;
		CanSwim = false;
		App->scene->KnowMap = 0;
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Spawn();
		break;
	}
}
void jPlayer::Die()//What happens when the player die
{
	current_animation = &Death[NumPlayer];
	//App->audio->PlayFx(deathfx);
	if (Death[NumPlayer].SeeCurrentFrame() == 1) 
		App->audio->PlayFx(deathfx2);
	if (Death[NumPlayer].SeeCurrentFrame()==10) {
		if (App->scene->KnowMap == 0) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Spawn();
		}
		if (App->scene->KnowMap == 1) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
			Spawn();
		}
	}
}

void jPlayer::Fall()//What happens when the player falls
{
	if (App->scene->KnowMap == 0) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Spawn();
	}
	if (App->scene->KnowMap == 1) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		Spawn();
	}	
}

void jPlayer::Spawn()
{
	NoInput = false;
	CanJump = true;
	CanClimb = false;
	CanSwim = false;
	current_animation = &idle[NumPlayer];
	if (App->scene->KnowMap == 0) {
		position.x = initialmap1.x;
		position.y = initialmap1.y;
	}
	if (App->scene->KnowMap == 1) {
		position.x = initialmap2.x;
		position.y = initialmap2.y;
	}
	Death[NumPlayer].current_frame = 0.0f;
	Death[NumPlayer].loops = 0;
}

void jPlayer::LoadPushbacks()
{
	// player yellow

	idle[0].PushBack({ 142,0,66,86 });

	idle2[0].PushBack({ 353,0,66,86 });

	GoRight[0].PushBack({ 0,0,67,86 });
	GoRight[0].PushBack({ 69,0,70,86 });
	GoRight[0].speed = 0.1f;

	GoLeft[0].PushBack({ 285,0,67,86 });
	GoLeft[0].PushBack({ 212,0,70,87 });
	GoLeft[0].speed = 0.1f;

	jumpR[0].PushBack({ 420,0,67,86 });


	jumpL[0].PushBack({ 420,86,67,86 });


	Climb[0].PushBack({ 488,0,65,86 });
	Climb[0].PushBack({ 556,0,65,86 });
	Climb[0].speed = 0.1f;

	ClimbIdle[0].PushBack({ 488,0,65,86 });

	SwimRight[0].PushBack({ 621,0,70,86 });
	SwimRight[0].PushBack({ 617,88,70,86 });
	SwimRight[0].speed = 0.1f;

	SwimLeft[0].PushBack({ 617,176,70,86 });
	SwimLeft[0].PushBack({ 617,263,70,86 });
	SwimLeft[0].speed = 0.1f;

	Death[0].PushBack({ 0,94,68,81 });
	Death[0].PushBack({ 73,94,68,81 });
	Death[0].PushBack({ 142,94,68,81 });
	Death[0].PushBack({ 213,94,68,81 });
	Death[0].PushBack({ 283,94,68,81 });
	Death[0].PushBack({ 351,94,68,81 });
	Death[0].PushBack({ 0,175,68,81 });
	Death[0].PushBack({ 69,175,68,81 });
	Death[0].PushBack({ 139,175,68,81 });
	Death[0].PushBack({ 206,175,68,81 });
	Death[0].PushBack({ 272,175,68,81 });
	Death[0].speed = 0.1f;
	//Death.loop = false;

	//player pink

	idle[1].PushBack({ 143,0,65,92 });

	idle2[1].PushBack({ 354,0,65,92 });

	GoRight[1].PushBack({ 0,0,67,93 });
	GoRight[1].PushBack({ 69,0,70,95 });
	GoRight[1].speed = 0.1f;

	GoLeft[1].PushBack({ 285,0,67,93 });
	GoLeft[1].PushBack({ 212,0,70,98 });
	GoLeft[1].speed = 0.1f;

	jumpR[1].PushBack({ 420,0,67,93 });


	jumpL[1].PushBack({ 420,95,67,93 });


	Climb[1].PushBack({ 488,0,65,92 });
	Climb[1].PushBack({ 556,0,65,92 });
	Climb[1].speed = 0.1f;

	ClimbIdle[1].PushBack({ 488,0,65,92 });

	SwimRight[1].PushBack({ 622,0,69,97 });
	SwimRight[1].PushBack({ 622,96,70,97 });
	SwimRight[1].speed = 0.1f;

	SwimLeft[1].PushBack({ 622,193,69,95 });
	SwimLeft[1].PushBack({ 622,289,70,97 });
	SwimLeft[1].speed = 0.1f;

	Death[1].PushBack({ 0,94,68,92 });
	
	Death[1].PushBack({ 0,186,68,81 });
	Death[1].PushBack({ 69,186,68,81 });
	Death[1].PushBack({ 139,186,68,81 });
	Death[1].PushBack({ 206,186,68,81 });
	Death[1].PushBack({ 272,186,68,81 });
	Death[1].speed = 0.1f;
	//Death.loop = false;

	//player blue
	idle[2].PushBack({ 143,0,65,92 });

	idle2[2].PushBack({ 354,0,65,92 });

	GoRight[2].PushBack({ 0,0,67,93 });
	GoRight[2].PushBack({ 69,0,70,95 });
	GoRight[2].speed = 0.1f;

	GoLeft[2].PushBack({ 285,0,67,93 });
	GoLeft[2].PushBack({ 212,0,70,98 });
	GoLeft[2].speed = 0.1f;

	jumpR[2].PushBack({ 420,0,67,93 });


	jumpL[2].PushBack({ 420,95,67,93 });


	Climb[2].PushBack({ 488,0,65,92 });
	Climb[2].PushBack({ 556,0,65,92 });
	Climb[2].speed = 0.1f;

	ClimbIdle[2].PushBack({ 488,0,65,92 });

	SwimRight[2].PushBack({ 622,0,69,97 });
	SwimRight[2].PushBack({ 622,96,70,97 });
	SwimRight[2].speed = 0.1f;

	SwimLeft[2].PushBack({ 622,193,69,95 });
	SwimLeft[2].PushBack({ 622,289,70,97 });
	SwimLeft[2].speed = 0.1f;

	Death[2].PushBack({ 0,94,68,92 });

	Death[2].PushBack({ 0,186,68,81 });
	Death[2].PushBack({ 69,186,68,81 });
	Death[2].PushBack({ 139,186,68,81 });
	Death[2].PushBack({ 206,186,68,81 });
	Death[2].PushBack({ 272,186,68,81 });
	Death[2].speed = 0.1f;

	dash.StartDash.PushBack({ 55,553,67,71 });
	dash.StartDash.PushBack({ 125,553,67,71 });
	dash.StartDash.PushBack({ 195,553,67,71 });
	dash.StartDash.PushBack({ 265,553,67,71 });
	dash.StartDash.speed = 0.05;
	dash.StartDash.loop = false;

	dash.FinishDash.PushBack({ 545,553,67,71 });
	dash.FinishDash.PushBack({ 475,553,67,71 });
	dash.FinishDash.PushBack({ 405,553,67,71 });
	dash.FinishDash.PushBack({ 335,553,67,71 });
	dash.FinishDash.PushBack({ 265,553,67,71 });
	dash.FinishDash.PushBack({ 195,553,67,71 });
	dash.FinishDash.PushBack({ 125,553,67,71 });
	dash.FinishDash.PushBack({ 55,553,67,71 });
	dash.FinishDash.speed = 0.05;
	dash.FinishDash.loop = false;
	
	dash.Dashing.PushBack({ 335,553,67,71 });
	dash.Dashing.PushBack({ 405,553,67,71 });
	dash.Dashing.PushBack({ 475,553,67,71 });
	dash.Dashing.PushBack({ 545,553,67,71 });
	dash.Dashing.speed = 0.05;
	dash.Dashing.loop = false;
	
}

void jPlayer::ChangePlayer(const int playernumber) 
{
	if (NumPlayer != playernumber) {
		App->tex->UnLoad(texture);
		texture = App->tex->Load(sprites_name[playernumber].GetString());
		App->collision->ColliderCleanUpPlayer();
		NumPlayer = playernumber;
		current_animation = &idle[NumPlayer];
		switch (playernumber) {
		case 0:
			coll = App->collision->AddCollider({ 0, 0, playerwidth, playerheight }, COLLIDER_PLAYER, this);
			break;
		case 1:
			position.y -= 17;
			coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, this);
			break;
		case 2:
			position.y -= 17;
			coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, this);
			break;
		}
	}
}

void jPlayer::GoJump()
{
	if (Jump && CanJump && !CanSwim && !God && !IsJumping) { //If you clicked the jump button and you are able to jump(always except you just jumpt) you can jump
		IsJumping = true;
	}
	if (IsJumping) { //if you are able to jump, determine the animation and direction of the jump
		Time += 1;
		CanJump = false;
		if (Time < 2)
			App->audio->PlayFx(jumpfx);
		if (Time <= JumpTime && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed;
		}
		else if (Time <= JumpTime && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed;
		}
		else if (Time <= JumpTime) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed;
		}
		else {
			IsJumping = false;
			if (current_animation == &jumpR[NumPlayer]) {
				current_animation = &idle[NumPlayer];
			}
			else current_animation = &idle2[NumPlayer];
		}
	}
	if (God && Jump) { //if you are in god mode and jump, you can fly
					   //App->audio->PlayFx(jumpfx);
		position.y -= JumpSpeed;
	}
}

void jPlayer::GoSwim()
{
	if (CanSwim) {
		if (current_animation == &SwimLeft[NumPlayer]) {
			current_animation = &SwimLeft[NumPlayer];
		}
		else {
			current_animation = &SwimRight[NumPlayer];
		}
	}
	if (CanSwim && GoUp) { //Can Swim determine if you are in a water collider, if you are, it's true
		position.y -= SpeedSwimUp;
	}
	if (CanSwim && GoDown) {
		position.y += SpeedSwimDown;
	}

}

void jPlayer::GoClimb()
{
	if (CanClimb && GoUp) {
		position.y -= SpeedClimb;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && GoDown) {
		position.y += SpeedClimb;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle[NumPlayer];

}

void jPlayer::Move_Left_Right()
{
	if (WalkRight) { //This determine the movement to the right, depending on the state of the player
		if (!IsJumping && !CanSwim && !CanClimb) {
			position.x += SpeedWalk;
			current_animation = &GoRight[NumPlayer];
		}
		if (IsJumping) {
			position.x += SpeedWalk;
		}
		if (CanSwim && !CanClimb) { //Can Climb determine if you are in a climb collider, if you are, it's true
			position.x += SpeedSwimLeftRight;
			current_animation = &SwimRight[NumPlayer];
		}
		if (CanClimb)
			position.x += SpeedWalk;
	}
	if (WalkLeft) { //This determine the movement to the left, depending on the state of the player
		if (!IsJumping && !CanSwim && !CanClimb) {
			position.x -= SpeedWalk;
			current_animation = &GoLeft[NumPlayer];
		}
		if (IsJumping) {
			position.x -= SpeedWalk;
		}
		if (CanSwim && !CanClimb) {
			position.x -= SpeedSwimLeftRight;
			current_animation = &SwimLeft[NumPlayer];
		}
		if (CanClimb)
			position.x -= SpeedWalk;
	}
	if (WalkRight && WalkLeft) {
		if (!CanSwim)
			current_animation = &idle[NumPlayer];
		if (CanSwim)
			current_animation = &SwimRight[NumPlayer];
		if (CanClimb) {
			current_animation = &Climb[NumPlayer];
		}

	}
	if (Idle) {
		if (current_animation == &GoRight[NumPlayer])
			current_animation = &idle[NumPlayer];
		if (current_animation == &GoLeft[NumPlayer])
			current_animation = &idle2[NumPlayer];
		if (dash.FinishDash.Finished())
			current_animation = &idle[NumPlayer];
	}
}

void jPlayer::Camera()
{
	if (App->scene->KnowMap == 0 && position.x >= positionWinMap1) {//knowmap it's a varibable that let us know in which map we are. //Knowmap=0, level 1 //knowmap=2, level 2
		NextMap = true;
	}
	if (position.x <= startmap2 && App->scene->KnowMap == 1) { //If player is in a position where the camera would print out of the map, camera stops
		App->render->camera.x = startpointcameramap2;

	}
	else if (position.x >= finalmapplayer) {
		App->render->camera.x = finalmap;
	}
	else {
		App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	if (position.y <= minYcam) { //If player is in a position where the camera would print out of the map, camera stops
		App->render->camera.y = 0;
	}
	else if (position.y >= maxYcam) {
		App->render->camera.y = lowcam;//lowcam is the bottom part of the map, when the player is too low, the camera follows a constant height to don't get out of the map
	}
	else {
		App->render->camera.y = -position.y + (App->render->camera.h / 2);
	}
	/*if (App->scene->KnowMap == 0) {
		if (App->render->camera.x <= 0) {
			App->render->camera.x = -300;
		}
	}
	else {
		if (App->render->camera.x <= 0) {
			App->render->camera.x = -45;
		}
	}*/
	/*bool PlayerInside = false;
	int Right = rect.x + rect.w;
	int PRight = coll->rect.x + coll->rect.w;
	int Left = rect.x;
	int PLeft = coll->rect.x;
 
	if (PLeft >= Left && Right >= PRight) {
		PlayerInside = true;
	}
	if (!PlayerInside) {
		App->render->camera.x -= SpeedWalk;
	}
	*/
}


void jPlayer::ShootLaser()
{
	if (Laser && !laser.IsShooting) {
		laser.StartShooting = true;
	}
	if (laser.StartShooting) {
		laser.StartShooting = false;
		laser.IsShooting = true;
		if (laser.coll != nullptr) {
			laser.coll->to_delete = true;
		}

		laser.position.x = position.x;
		laser.position.y = position.y;

		laser.coll = App->collision->AddCollider(laser.anim.GetCurrentFrame(), COLLIDER_PARTICLE);
	}
	if (laser.IsShooting) {
		if (laser.life > laser.time) {
			laser.life = 0;
			laser.IsShooting = false;
			laser.coll->to_delete = true;
		}
		laser.life++;
		laser.position.x += laser.velocity.x;
		laser.coll->SetPos(laser.position.x, laser.position.y);
		App->render->Blit(texture, laser.position.x, laser.position.y, &(laser.anim.GetCurrentFrame()));
	}
}

void Dash::ResetDashAnims()
{
	Dashing.current_frame = 0.0f;
	Dashing.loops = 0;

	StartDash.current_frame = 0.0f;
	StartDash.loops = 0;

	FinishDash.current_frame = 0.0f;
	FinishDash.loops = 0;


}
