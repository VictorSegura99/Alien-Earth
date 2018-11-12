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
		if (numplayer == 0) {
			BottomLeft.anim = LoadPushbacks(numplayer, config, "BottomLeft");
			BottomRight.anim = LoadPushbacks(numplayer, config, "BottomRight");
		}
		if (numplayer == 2) {
			dashR.StartDash = LoadPushbacks(numplayer, config, "StartDashRight");
			dashR.FinishDash = LoadPushbacks(numplayer, config, "FinishDashRight");
			dashR.Dashing = LoadPushbacks(numplayer, config, "DashingRight");
			dashL.StartDash = LoadPushbacks(numplayer, config, "StartDashLeft");
			dashL.FinishDash = LoadPushbacks(numplayer, config, "FinishDashLeft");
			dashL.Dashing = LoadPushbacks(numplayer, config, "DashingLeft");
		}
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
	auxGravity = gravity;
	jumpfx = App->audio->LoadFx(JumpFx.GetString());
	waterfx = App->audio->LoadFx(WaterFx.GetString());
	deathfx = App->audio->LoadFx(DeathFx.GetString());
	deathfx2 = App->audio->LoadFx(DeathFx2.GetString());
	ladderfx = App->audio->LoadFx(LadderFx.GetString());

	position.x = initialmap1.x;
	position.y = initialmap1.y;
	

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
	//DT = dt;
	Gravity(dt);
	LOG("Gravity: %.6f", gravity);
	if (!dashing) {
		if (NumPlayer == 0)
			DoubleJump(dt);
		GoJump(dt);
		GoSwim(dt);
		GoClimb(dt);
		Move_Left_Right(dt);
		if (NumPlayer == 0)
			BottomFall(dt);
		if (NumPlayer == 1) 
			ShootLaser(dt);
	}
	if (NumPlayer == 2)
		DoDash(dt);
	Camera(dt);
	
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
		App->render->Blit(texture, position.x - playerwidth, position.y, &(current_animation->GetCurrentFrame(dt)), 1.0f);
	}
	else App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)),1.0f);
	
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
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			Falling = false;
		}
		break;
	case COLLIDER_WALL_UP:
		GoUp = false;
		break;
	case COLLIDER_WALL_LEFT:
		if (!CanSwim && !CanClimb)
			position.x += SpeedWalk * DT;
		if (CanSwim)
			position.x += SpeedSwimLeftRight * DT;
		if (CanClimb)
			position.x += SpeedWalk * DT;
		if (dashing) {
			dashing = false;
			current_animation = &GoLeft[NumPlayer];
		}
		break;
	case COLLIDER_WALL_RIGHT:
		if (!CanSwim && !CanClimb)
			position.x -= SpeedWalk * DT;
		if (CanSwim)
			position.x += SpeedSwimLeftRight * DT;
		if (CanClimb) 
			position.x -= SpeedWalk * DT;
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
			Falling = false;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			if (!dashing)
				current_animation = &idle[NumPlayer];
		}
		
		break;
	case COLLIDER_CLIMB:
		App->audio->PlayFx(ladderfx);
		CanClimb = true;
		CanJump = true;
		CanJump2 = false;
		Time = 50 * DT;
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
Animation jPlayer::LoadPushbacks(int playernumber, pugi::xml_node& config, p2SString NameAnim) const
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

void jPlayer::GoJump(float dt)
{
	if (Jump && CanJump && !CanSwim && !God && !IsJumping) { //If you clicked the jump button and you are able to jump(always except you just jumpt) you can jump
		IsJumping = true;
	}
	if (IsJumping) { //if you are able to jump, determine the animation and direction of the jump
		//Time = Time * dt;
		Time+= 100 * dt;
		CanJump = false;
		if (Time * dt < 2 * dt)
			App->audio->PlayFx(jumpfx);
		if (Time * dt >= 5 * dt) {
			JumpSpeed -= 2.2f *dt;
		}
		if (Time * dt <= JumpTime * dt && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (Time * dt <= JumpTime * dt && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (Time * dt <= JumpTime * dt) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (Time * dt >= JumpTime * dt) {
			IsJumping = false;
			CanJump2 = true;
			CanJump = false;
			//JumpSpeed = 750.0f;
			Time = 0;
			if (current_animation == &jumpR[NumPlayer]) {
				current_animation = &idle[NumPlayer];
			}
			else current_animation = &idle2[NumPlayer];
		}
	}
	if (God && Jump) { //if you are in god mode and jump, you can fly
					   //App->audio->PlayFx(jumpfx);
		position.y -= JumpSpeed * dt;
	}
}

void jPlayer::GoSwim(float dt)
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
		position.y -= SpeedSwimUp * dt;
	}
	if (CanSwim && GoDown) {
		position.y += SpeedSwimDown * dt;
	}

}

void jPlayer::GoClimb(float dt)
{
	if (CanClimb && GoUp) {
		position.y -= SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && GoDown) {
		position.y += SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle[NumPlayer];

}

void jPlayer::Move_Left_Right(float dt)
{
	if (WalkRight) { //This determine the movement to the right, depending on the state of the player
		if (!IsJumping && !CanSwim && !CanClimb) {
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
			position.x += SpeedWalk * dt;
			current_animation = &GoRight[NumPlayer];
		}
		if (IsJumping) {
			position.x += SpeedWalk * dt;
		}
		if (CanSwim && !CanClimb) { //Can Climb determine if you are in a climb collider, if you are, it's true
			position.x += SpeedSwimLeftRight * dt;
			current_animation = &SwimRight[NumPlayer];
		}
		if (CanClimb)
			position.x += SpeedWalk * dt;
	}
	if (WalkLeft) { //This determine the movement to the left, depending on the state of the player
		if (!IsJumping && !CanSwim && !CanClimb) {
			dashR.ResetDashAnims();
			dashL.ResetDashAnims();
			position.x -= SpeedWalk * dt;
			current_animation = &GoLeft[NumPlayer];
		}
		if (IsJumping) {
			position.x -= SpeedWalk * dt;
		}
		if (CanSwim && !CanClimb) {
			position.x -= SpeedSwimLeftRight * dt;
			current_animation = &SwimLeft[NumPlayer];
		}
		if (CanClimb)
			position.x -= SpeedWalk * dt;
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
		if (!BottomLeft.IsFalling && NumPlayer == 0 && current_animation == &BottomLeft.anim)
			current_animation = &idle2[NumPlayer]; 
		if (!BottomRight.IsFalling && NumPlayer == 0 && current_animation == &BottomRight.anim)
			current_animation = &idle[NumPlayer];
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

void jPlayer::Camera(float dt)
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

void jPlayer::DoDash(float dt)
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && Hability && CanDash) {
		JumpSpeed = 30.0f*dt;
		dashing = true;
		dashR.DashRight = true;
	}
	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && Hability && CanDash) {
		JumpSpeed = 30.0f*dt;
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
			position.x += 30 * dt;
			if (dashR.DashCont >= dashR.DashTime) {
				position.x -= 20 * dt;
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
			position.x -= 30 * dt;
			if (dashL.DashCont >= dashL.DashTime) {
				position.x += 20 * dt;
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


void jPlayer::ShootLaser(float dt)
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

		laserR.coll = App->collision->AddCollider(laserR.anim.GetCurrentFrame(dt), COLLIDER_PARTICLE);
	}
	if (laserR.IsShooting) {
		if (laserR.life < laserR.time) {
			laserR.life = laserR.timelife;
			laserR.IsShooting = false;
			laserR.coll->to_delete = true;
		}
		laserR.life--;
		laserR.position.x += laserR.velocity.x*dt;
		laserR.coll->SetPos(laserR.position.x - 10, laserR.position.y + 22);
		App->render->Blit(texture, laserR.position.x - 10, laserR.position.y + 22, &(laserR.anim.GetCurrentFrame(dt)));
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

		laserL.coll = App->collision->AddCollider(laserL.anim.GetCurrentFrame(dt), COLLIDER_PARTICLE);
	}
	if (laserL.IsShooting) {
		if (laserL.life < laserL.time) {
			laserL.life = laserL.timelife;
			laserL.IsShooting = false;
			laserL.coll->to_delete = true;
		}
		laserL.life--;
		laserL.position.x += laserL.velocity.x*dt;
		laserL.coll->SetPos(laserL.position.x - 10, laserL.position.y + 22);
		App->render->Blit(texture, laserL.position.x - 10, laserL.position.y + 22, &(laserL.anim.GetCurrentFrame(dt)));
	}
}

void jPlayer::DoubleJump(float dt)
{
  	if (CanJump2 && Jump && !IsJumping) {
		IsJumping2 = true;
		Time = 0;
	}
	if (Jump && IsJumping && !CanJump2) {
		IsJumping = false;
		CanJump2 = true;
 		CanJump = false;
		Time = 0;
		//JumpSpeed = 30.0f*dt;
		IsJumping2 = true;
	}
	if (IsJumping2) { //if you are able to jump, determine the animation and direction of the jump
		Time += 100 * dt;
		if (Time * dt < 2 * dt)
			App->audio->PlayFx(jumpfx);
		if (Time * dt >= 5 * dt) {
			JumpSpeed -= 2.2f*dt;
		}
		if (Time * dt <= JumpTime * dt && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (Time * dt <= JumpTime * dt && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (Time * dt <= JumpTime * dt) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (Time * dt >= JumpTime * dt) {
			IsJumping2 = false;
			CanJump2 = false;
			Falling = true;
			//JumpSpeed = 30.0f*dt;
			if (current_animation == &jumpR[NumPlayer]) {
				current_animation = &idle[NumPlayer];
			}
			else current_animation = &idle2[NumPlayer];
		}
	}
}

void jPlayer::BottomFall(float dt)
{
	if (Hability && IsJumping2) {
		if (current_animation == &GoRight[NumPlayer] || current_animation==&idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			BottomLeft.IsFalling = true;
		}

		IsJumping2 = false;
		CanJump2 = false;
	}
	if (Hability && Falling) {
		if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			BottomLeft.IsFalling = true;
		}
		IsJumping2 = false;
		CanJump2 = false;
	}
	if (BottomLeft.IsFalling) {
		current_animation = &BottomLeft.anim;
		position.y += BottomLeft.speed * dt;
	}
	if (BottomRight.IsFalling) {
		current_animation = &BottomRight.anim;
		position.y += BottomRight.speed * dt;
	}

}

void jPlayer::Gravity(float dt)
{
	gravity = auxGravity;
	gravity = gravity * dt;
	position.y -= gravity;
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
