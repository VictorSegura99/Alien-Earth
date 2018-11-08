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

	for (int numplayer = 0; numplayer < 3; ++numplayer) {
		idle[numplayer] = LoadPushbacks(numplayer, config, "idle");
		idle2[numplayer] = LoadPushbacks(numplayer, config, "idle2");
		GoRight[numplayer] = LoadPushbacks(numplayer, config, "GoRight");
		GoLeft[numplayer] = LoadPushbacks(numplayer, config, "GoLeft");
		jumpR[numplayer] = LoadPushbacks(numplayer, config, "jumpR");
		jumpL[numplayer] = LoadPushbacks(numplayer, config, "jumpL");
		Climb[numplayer] = LoadPushbacks(numplayer, config, "Climb");
		ClimbIdle[numplayer] = LoadPushbacks(numplayer, config, "ClimbIdle");
		SwimRight[numplayer] = LoadPushbacks(numplayer, config, "SwimRight");
		SwimLeft[numplayer] = LoadPushbacks(numplayer, config, "SwimLeft");
		Death[numplayer] = LoadPushbacks(numplayer, config, "Death");
	}

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
	

	laserR.anim.PushBack({ 34,554,83,27 });
	laserR.anim.PushBack({ 124,554,83,27 });
	laserR.anim.speed = 0.2f;
	laserR.velocity.x = 18.0f;
	laserR.timelife = 50;
	laserR.life = laserR.timelife;

	laserL.anim.PushBack({ 34,582,83,27 });
	laserL.anim.PushBack({ 124,582,83,27 });
	laserL.anim.speed = 0.2f;
	laserL.velocity.x = -18.0f;
	laserL.timelife = 50;
	laserL.life = laserL.timelife;
	
	
	
	return ret;
}
bool jPlayer::PreUpdate() //Here we preload the input functions to determine the state of the player
{
	if (!NoInput) {
		if (!dashing) {
			WalkLeft = App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
			WalkRight = App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
		}
		GoUp = App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
		GoDown = App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
		Hability = App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN;
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
		if (NumPlayer == 0)
			DoubleJump();
		GoJump();
		GoSwim();
		GoClimb();
		Move_Left_Right();
		
		if (NumPlayer == 1) 
			ShootLaser();
	}
	if (NumPlayer == 2)
		DoDash();
	Camera();
	
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
	if (current_animation == &dashR.FinishDash) {
		App->render->Blit(texture, position.x - playerwidth, position.y, &(current_animation->GetCurrentFrame()));
	}
	else App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame()));
	
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
	App->tex->UnLoad(ParticlesTex);
	NextMap = false;
	death = false;
	fall = false;
	God = false;
	//Death[NumPlayer].current_frame = 0.0f;
	//Death[NumPlayer].loops = 0;
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
			CanJump2 = false;
			Time = 0;
			CanSwim = false;
			GoDown = false;
			CanClimb = false;
			CanDash = true;
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
		if (dashing) {
			dashing = false;
			current_animation = &GoLeft[NumPlayer];
		}
		break;
	case COLLIDER_WALL_RIGHT:
		if (!CanSwim && !CanClimb)
			position.x -= SpeedWalk;
		if (CanSwim)
			position.x += SpeedSwimLeftRight;
		if (CanClimb) 
			position.x -= SpeedWalk;
		if (dashing) {
			dashing = false;
			current_animation = &GoRight[NumPlayer];
		}
		
		break;
	case COLLIDER_PLATFORM:
		if (position.y + playerHeight < c2->rect.y) {
			position.y += gravity;
			CanJump = true;
			CanJump2 = false;
			Time = 0;
			CanSwim = false;
			IsJumping = false;
			CanDash = true;
			if (!dashing)
				current_animation = &idle[NumPlayer];
		}
		
		break;
	case COLLIDER_CLIMB:
		App->audio->PlayFx(ladderfx);
		CanClimb = true;
		CanJump = true;
		CanJump2 = false;
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
		CanJump2 = false;
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
		CanJump2 = false;
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
	if (Death[NumPlayer].Finished()) {
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
	Death[NumPlayer].current_frame = 0.0f;
	Death[NumPlayer].loops = 0;
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

	//idle[0].PushBack({ 142,0,66,86 });

	//idle2[0].PushBack({ 353,0,66,86 });

/*	GoRight[0].PushBack({ 0,0,67,86 });
	GoRight[0].PushBack({ 69,0,70,86 });
	GoRight[0].speed = 0.1f;*/

	/*GoLeft[0].PushBack({ 285,0,67,86 });
	GoLeft[0].PushBack({ 212,0,70,87 });
	GoLeft[0].speed = 0.1f;*/

//	jumpR[0].PushBack({ 420,0,67,86 });


	//jumpL[0].PushBack({ 420,86,67,86 });


	/*Climb[0].PushBack({ 488,0,65,86 });
	Climb[0].PushBack({ 556,0,65,86 });
	Climb[0].speed = 0.1f;*/

	//ClimbIdle[0].PushBack({ 488,0,65,86 });

	/*SwimRight[0].PushBack({ 621,0,70,86 });
	SwimRight[0].PushBack({ 617,88,70,86 });
	SwimRight[0].speed = 0.1f;*/

	/*SwimLeft[0].PushBack({ 617,176,70,86 });
	SwimLeft[0].PushBack({ 617,263,70,86 });
	SwimLeft[0].speed = 0.1f;
	*/
	/*Death[0].PushBack({ 0,94,68,81 });
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
	Death[0].speed = 0.2f;
	Death[0].loop = false;
	//Death.loop = false;
	*/
	//player pink

	idle[1].PushBack({ 143,0,65,92 });

	idle2[1].PushBack({ 354,0,65,92 });

	/*GoRight[1].PushBack({ 0,0,67,93 });
	GoRight[1].PushBack({ 69,0,70,95 });
	GoRight[1].speed = 0.1f;*/

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

	Death[1].PushBack({ 0,105,68,92 });
	Death[1].PushBack({ 69,105,68,92 });
	Death[1].PushBack({ 138,105,68,92 });
	Death[1].PushBack({ 207,105,68,92 });
	Death[1].PushBack({ 276,105,68,92 });
	
	Death[1].PushBack({ 0,196,68,81 });
	Death[1].PushBack({ 69,196,68,81 });
	Death[1].PushBack({ 139,196,68,81 });
	Death[1].PushBack({ 206,196,68,81 });
	Death[1].PushBack({ 272,196,68,81 });
	Death[1].speed = 0.2f;
	Death[1].loop = false;
	//Death.loop = false;

	//player blue
	idle[2].PushBack({ 143,0,65,92 });

	idle2[2].PushBack({ 354,0,65,92 });

	/*GoRight[2].PushBack({ 0,0,67,93 });
	GoRight[2].PushBack({ 69,0,70,95 });
	GoRight[2].speed = 0.1f;*/

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

	Death[2].PushBack({ 0,104,68,92 });
	Death[2].PushBack({ 68,104,68,92 });
	Death[2].PushBack({ 136,104,68,92 });
	Death[2].PushBack({ 203,104,68,92 });
	Death[2].PushBack({ 272,104,68,92 });

	Death[2].PushBack({ 0,196,68,81 });
	Death[2].PushBack({ 69,196,68,81 });
	Death[2].PushBack({ 139,196,68,81 });
	Death[2].PushBack({ 206,196,68,81 });
	Death[2].PushBack({ 272,196,68,81 });
	Death[2].speed = 0.2f;
	Death[2].loop = false;

	dashR.StartDash.PushBack({ 0,532,67,92 });
	dashR.StartDash.PushBack({ 0,658,84,92 });
	dashR.StartDash.PushBack({ 85,658,84,92 });
	dashR.StartDash.speed = 0.5f;

	dashR.FinishDash.PushBack({ 564,532,115,92 });
	dashR.FinishDash.PushBack({ 564,625,130,92 });
	dashR.FinishDash.PushBack({ 430,625,130,92 });
	dashR.FinishDash.PushBack({ 288,625,130,92 });
	dashR.FinishDash.speed = 0.2f;
	dashR.FinishDash.loop = false;
	
	dashR.Dashing.PushBack({ 85,658,84,92 });

	dashL.StartDash.PushBack({ 84,806,67,92 });
	dashL.StartDash.PushBack({ 84,806,67,92 });
	dashL.StartDash.PushBack({ 0,806,83,92 });
	dashL.StartDash.speed = 0.5f;
	dashL.StartDash.loop = false;

	dashL.Dashing.PushBack({ 0,806,83,92 });

	dashL.FinishDash.PushBack({ 564,771,107,92 });
	dashL.FinishDash.PushBack({ 564,844,130,92 });
	dashL.FinishDash.PushBack({ 430,725,130,92 });
	dashL.FinishDash.speed = 0.2f;
	dashL.FinishDash.loop = false;
	
}

Animation jPlayer::LoadPushbacks(int playernumber, pugi::xml_node& config, p2SString NameAnim)
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
		if (Time >= 5) {
			JumpSpeed -= 2.2f;
		}
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
		if (Time >= JumpTime) {
			IsJumping = false;
			CanJump2 = true;
			CanJump = false;
			JumpSpeed = 30.0f;
			Time = 0;
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
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
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
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
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
		if (dashL.FinishDash.Finished()) {
			current_animation = &idle2[NumPlayer];
			dashL.ResetDashAnims();
			dashR.ResetDashAnims();
		}
		if (dashR.FinishDash.Finished()) {
			current_animation = &idle[NumPlayer];
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
		}
			
		
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

void jPlayer::DoDash()
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && Hability && CanDash) {
		JumpSpeed = 30.0f;
		dashing = true;
		dashR.DashRight = true;
	}
	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && Hability && CanDash) {
		JumpSpeed = 30.0f;
		dashing = true;
		dashL.DashLeft = true;
	}
	if (dashing && dashR.DashRight) {
		position.y += gravity;
		if (dashR.StartDash.current_frame == 0) {
			//	position.x -= 60;
			current_animation = &dashR.StartDash;
		}
		if (dashR.StartDash.Finished()) {
			++dashR.DashCont;
			current_animation = &dashR.Dashing;
			position.x += 30;
			if (dashR.DashCont >= dashR.DashTime) {
				position.x -= 20;
				current_animation = &dashR.FinishDash;
				if (dashR.FinishDash.Finished()) {
					dashR.DashCont = 0;
					dashing = false;
					IsJumping = false;
					CanJump = false;
					CanDash = false;
				}

			}
		}
	}
	if (dashing && dashL.DashLeft) {
		position.y += gravity;
		if (dashL.StartDash.current_frame == 0) {
			//	position.x -= 60;
			current_animation = &dashL.StartDash;
		}
		if (dashL.StartDash.Finished()) {
			++dashL.DashCont;
			current_animation = &dashL.Dashing;
			position.x -= 30;
			if (dashL.DashCont >= dashL.DashTime) {
				position.x += 20;
				current_animation = &dashL.FinishDash;
				if (dashL.FinishDash.Finished()) {
					dashL.DashCont = 0;
					dashing = false;
					IsJumping = false;
					CanJump = false;
					CanDash = false;
				}

			}
		}
	}

}


void jPlayer::ShootLaser()
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && (Hability && !laserR.IsShooting)) {
		laserR.StartShooting = true;
	}
	if (laserR.StartShooting) {
		laserR.StartShooting = false;
		laserR.IsShooting = true;
		if (laserR.coll != nullptr) {
			laserR.coll->to_delete = true;
		}

		laserR.position.x = position.x;
		laserR.position.y = position.y;

		laserR.coll = App->collision->AddCollider(laserR.anim.GetCurrentFrame(), COLLIDER_PARTICLE);
	}
	if (laserR.IsShooting) {
		if (laserR.life < laserR.time) {
			laserR.life = laserR.timelife;
			laserR.IsShooting = false;
			laserR.coll->to_delete = true;
		}
		laserR.life--;
		laserR.position.x += laserR.velocity.x;
		laserR.coll->SetPos(laserR.position.x - 10, laserR.position.y + 22);
		App->render->Blit(texture, laserR.position.x - 10, laserR.position.y + 22, &(laserR.anim.GetCurrentFrame()));
	}

	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && (Hability && !laserL.IsShooting)) {
		laserL.StartShooting = true;
	}
	if (laserL.StartShooting) {
		laserL.StartShooting = false;
		laserL.IsShooting = true;
		if (laserL.coll != nullptr) {
			laserL.coll->to_delete = true;
		}

		laserL.position.x = position.x;
		laserL.position.y = position.y;

		laserL.coll = App->collision->AddCollider(laserL.anim.GetCurrentFrame(), COLLIDER_PARTICLE);
	}
	if (laserL.IsShooting) {
		if (laserL.life < laserL.time) {
			laserL.life = laserL.timelife;
			laserL.IsShooting = false;
			laserL.coll->to_delete = true;
		}
		laserL.life--;
		laserL.position.x += laserL.velocity.x;
		laserL.coll->SetPos(laserL.position.x - 10, laserL.position.y + 22);
		App->render->Blit(texture, laserL.position.x - 10, laserL.position.y + 22, &(laserL.anim.GetCurrentFrame()));
	}
}

void jPlayer::DoubleJump()
{
  	if (CanJump2 && Jump && !IsJumping) {
		IsJumping2 = true;
	}
	if (Jump && IsJumping && !CanJump2) {
		IsJumping = false;
		CanJump2 = true;
 		CanJump = false;
		Time = 0;
		JumpSpeed = 30.0f;
		IsJumping2 = true;
	}
	if (IsJumping2) { //if you are able to jump, determine the animation and direction of the jump
		Time += 1;
		if (Time < 2)
			App->audio->PlayFx(jumpfx);
		if (Time >= 5) {
			JumpSpeed -= 2.2f;
		}
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
		if (Time >= JumpTime) {
			IsJumping2 = false;
			CanJump2 = false;
			JumpSpeed = 30.0f;
			if (current_animation == &jumpR[NumPlayer]) {
				current_animation = &idle[NumPlayer];
			}
			else current_animation = &idle2[NumPlayer];
		}
	}


}

void Dash::ResetDashAnims()
{
	StartDash.current_frame = 0.0f;
	StartDash.loops = 0;

	FinishDash.current_frame = 0.0f;
	FinishDash.loops = 0;

	DashLeft = false;
	DashRight = false;
	DashCont = 0;
}
