#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "EntityManager.h"
#include "j1Timer.h"
#include "j1Scene.h"
#include "j1Choose.h"
#include "Player.h"
#include "j1Particles.h"

#include "Brofiler/Brofiler.h"

Player::Player() : Entity()
{

}

Player::~Player() {}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Init SDL player");

	pugi::xml_node player = config.child("player");

	sprites_name[0] = player.child("sprites").text().as_string();
	sprites_name[1] = player.child("sprites2").text().as_string();
	sprites_name[2] = player.child("sprites3").text().as_string();
	JumpFx = player.child("JumpFx").text().as_string();
	WaterFx = player.child("WaterFx").text().as_string();
	DeathFx = player.child("DeathFx").text().as_string();
	DeathFx2 = player.child("DeathFx2").text().as_string();
	LadderFx = player.child("LadderFx").text().as_string();
	LaserFx = player.child("LaserFx").text().as_string();
	DashFx = player.child("DashFx").text().as_string();
	finalmapplayer = player.child("finalmapplayer").attribute("value").as_int();
	finalmap = player.child("finalmap").attribute("value").as_int();
	startmap2 = player.child("startmap2").attribute("value").as_int();
	maxYcam = player.child("maxYcam").attribute("value").as_int();
	minYcam = player.child("minYcam").attribute("value").as_int();
	lowcam = player.child("lowcam").attribute("value").as_int();
	gravity = player.child("gravity").attribute("value").as_float();
	positionWinMap1 = player.child("positionWinMap1").attribute("value").as_int();
	startpointcameramap2 = player.child("startpointcameramap2").attribute("value").as_int();
	SpeedSwimLeftRight = player.child("SpeedSwimLeftRight").attribute("value").as_float();
	SpeedSwimUp = player.child("SpeedSwimUp").attribute("value").as_float();
	SpeedClimb = player.child("SpeedClimb").attribute("value").as_float();
	SpeedWalk = player.child("SpeedWalk").attribute("value").as_float();
	playerHeight = player.child("playerHeight").attribute("value").as_int();
	SpeedSwimDown = player.child("SpeedSwimDown").attribute("value").as_float();
	JumpTime = player.child("JumpTime").attribute("value").as_int();
	JumpSpeed = player.child("JumpSpeed").attribute("value").as_float();
	AuxJumpSpeed = player.child("AuxJumpSpeed").attribute("value").as_float();
	playerwidth = player.child("playerwidth").attribute("value").as_int();
	playerheight = player.child("playerheight").attribute("value").as_int();
	laserR.velocity.x = player.child("laservelocityR").attribute("value").as_float();
	laserL.velocity.x = player.child("laservelocityL").attribute("value").as_float();
	laserL.timelife = player.child("lasertimelife").attribute("value").as_int();
	laserR.timelife = player.child("lasertimelife").attribute("value").as_int();
	BottomLeft.speed = player.child("BottomSpeed").attribute("value").as_float();
	BottomRight.speed = player.child("BottomSpeed").attribute("value").as_float();
	TutorialX = player.child("TutorialX").attribute("value").as_int();
	TutorialY1 = player.child("TutorialY1").attribute("value").as_int();
	TutorialY2 = player.child("TutorialY2").attribute("value").as_int();

	for (int numplayer = 0; numplayer < 3; ++numplayer) {
		idle[numplayer] = LoadPushbacks(numplayer, player, "idle");
		idle2[numplayer] = LoadPushbacks(numplayer, player, "idle2");
		GoRight[numplayer] = LoadPushbacks(numplayer, player, "GoRight");
		GoLeft[numplayer] = LoadPushbacks(numplayer, player, "GoLeft");
		jumpR[numplayer] = LoadPushbacks(numplayer, player, "jumpR");
		jumpL[numplayer] = LoadPushbacks(numplayer, player, "jumpL");
		Climb[numplayer] = LoadPushbacks(numplayer, player, "Climb");
		ClimbIdle[numplayer] = LoadPushbacks(numplayer, player, "ClimbIdle");
		SwimRight[numplayer] = LoadPushbacks(numplayer, player, "SwimRight");
		SwimLeft[numplayer] = LoadPushbacks(numplayer, player, "SwimLeft");
		Death[numplayer] = LoadPushbacks(numplayer, player, "Death");
		if (numplayer == 0) {
			BottomLeft.anim = LoadPushbacks(numplayer, player, "BottomLeft");
			BottomRight.anim = LoadPushbacks(numplayer, player, "BottomRight");
			doubleJump = LoadPushbacks(numplayer, player, "doubleJump");
		}
		if (numplayer == 1) {
			laserR.anim = LoadPushbacks(numplayer, player, "LaserRight");
			laserL.anim = LoadPushbacks(numplayer, player, "LaserLeft");
		}
		if (numplayer == 2) {
			dashR.StartDash = LoadPushbacks(numplayer, player, "StartDashRight");
			dashR.FinishDash = LoadPushbacks(numplayer, player, "FinishDashRight");
			dashR.Dashing = LoadPushbacks(numplayer, player, "DashingRight");
			dashL.StartDash = LoadPushbacks(numplayer, player, "StartDashLeft");
			dashL.FinishDash = LoadPushbacks(numplayer, player, "FinishDashLeft");
			dashL.Dashing = LoadPushbacks(numplayer, player, "DashingLeft");
		}
	}

	bool ret = true;
	return ret;
}

bool Player::Start()
{
	BROFILER_CATEGORY("Player: Start", Profiler::Color::DarkGreen)
	bool ret = true;

	auxGravity = gravity;
	jumpfx = App->audio->LoadFx(JumpFx.GetString());
	waterfx = App->audio->LoadFx(WaterFx.GetString());
	deathfx = App->audio->LoadFx(DeathFx.GetString());
	deathfx2 = App->audio->LoadFx(DeathFx2.GetString());
	ladderfx = App->audio->LoadFx(LadderFx.GetString());
	laserfx = App->audio->LoadFx(LaserFx.GetString());
	dashfx = App->audio->LoadFx(DashFx.GetString());

	position.x = App->entitymanager->positionStartMap1.x;
	position.y = App->entitymanager->positionStartMap1.y;

 	laserR.life = laserR.timelife;
	laserL.life = laserL.timelife;
	current_animation = &idle[NumPlayer];
	return ret;
}
bool Player::PreUpdate() //Here we preload the input functions to determine the state of the player
{
	BROFILER_CATEGORY("Player: PreUpdate", Profiler::Color::Green);
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
bool Player::Update(float dt)
{
	BROFILER_CATEGORY("Player: Update", Profiler::Color::Green);
	DT = dt;

	if (!TouchingGround&&!dashing) {
		acceleration.y = gravity*dt;
		LOG("Acceleration %f", acceleration.y);
	}
	else
		acceleration.y = 0;
	position.x += velocity.x;
	position.y -= velocity.y + acceleration.y;
	
	if (!dashing) {
		if (NumPlayer == 0)
			DoubleJump(dt);
		GoJump(dt);
		GoSwim(dt);
		GoClimb(dt);
		Move_Left_Right(dt);
		if (NumPlayer == 1)
			ShootLaser(dt);
	}
	if (NumPlayer == 2)
		DoDash(dt);
	if (NumPlayer == 0)
		BottomFall(dt);
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


	if (App->collision->debug)
		App->render->DrawQuad(CamRect, 150, 150, 150);

	TouchingGround = false;

	return true;
}

bool Player::PostUpdate()
{
	BROFILER_CATEGORY("Player: PostUpdate", Profiler::Color::Green);
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
bool Player::Load(pugi::xml_node& player)
{
	position.x = player.child("position").attribute("x").as_float();
	position.y = player.child("position").attribute("y").as_float();


	App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);


	return true;
}
bool Player::Save(pugi::xml_node& player) const
{
	player.append_child("position").append_attribute("x") = position.x;
	player.child("position").append_attribute("y") = position.y;


	return true;
}
void Player::Draw(float dt)
{
	if (App->scene->KnowMap == 0) {
		switch (NumPlayer) {
		case 0:
			App->render->Blit(App->scene->TutorialJeff, TutorialX, TutorialY1, NULL, SDL_FLIP_NONE, 1.0f);
			break;
		case 1:
			App->render->Blit(App->scene->TutorialJane, TutorialX, TutorialY2, NULL, SDL_FLIP_NONE, 1.0f);
			break;
		case 2:
			App->render->Blit(App->scene->TutorialJerry, TutorialX, TutorialY2, NULL, SDL_FLIP_NONE, 1.0f);
			break;
		}
	}


	if (current_animation == &dashR.FinishDash) {
		App->render->Blit(texture, position.x - playerwidth, position.y, &(current_animation->GetCurrentFrame(dt)));
	}
	else App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));

}
bool Player::CleanUp()
{
	App->tex->UnLoad(texture);
	App->tex->UnLoad(ParticlesTex);
	NextMap = false;
	death = false;
	fall = false;
	God = false;
	if (coll)
		coll->to_delete = true;
	return true;
}

void Player::OnCollision(Collider * c2) //this determine what happens when the player touch a type of collider
{
	switch (c2->type) {
	case COLLIDER_GROUND:
		if (position.y < c2->rect.y + c2->rect.h) {
			velocity.y = 0;
			TouchingGround = true;
			CanJump = true;
			CanJump2 = false;
			CanSwim = false;
			GoDown = false;
			CanClimb = false;
			CanDash = true;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			Falling = false;
			FallingJump2 = false;
			cameraon = true;
			CanDoAnotherJump = true;
			if (current_animation == &jumpR[NumPlayer])
				current_animation = &idle[NumPlayer];
			if (current_animation == &jumpL[NumPlayer])
				current_animation = &idle2[NumPlayer];
		}
		break;
	case COLLIDER_WALL_UP:
		AnimDoubleJump = false;
		velocity.y = 0;
		IsJumping = false;
		if (Jump2Complete)
			IsJumping2 = false;
		Jump2Complete = false;
		GoUp = false;
		break;
	case COLLIDER_WALL_LEFT:

		if (!CanSwim && !CanClimb && WalkLeft)
			position.x += SpeedWalk * DT;
		if (CanSwim && WalkLeft)
			position.x += SpeedSwimLeftRight * DT;
		if (CanClimb && WalkLeft)
			position.x += SpeedWalk * DT;
		if (dashing) {
			dashing = false;
			current_animation = &GoLeft[NumPlayer];
		}
		break;
	case COLLIDER_WALL_RIGHT:
		if (!CanSwim && !CanClimb && WalkRight)
			position.x -= SpeedWalk * DT;
		if (CanSwim && WalkRight)
			position.x -= SpeedSwimLeftRight * DT;
		if (CanClimb && WalkRight)
			position.x -= SpeedWalk * DT;
		if (dashing) {
			dashing = false;
			current_animation = &GoRight[NumPlayer];
		}

		break;
	case COLLIDER_PLATFORM:
		if (position.y + 70 < c2->rect.y)
		{
			velocity.y = 0;
			//IsJumping = false;
			//IsJumping2 = false;
			TouchingGround = true;
			CanDoAnotherJump = true;
			CanJump = true;
			CanClimb = false;
			FallingJump2 = false;
			CanJump2 = false;
			GoDown = false;
			CanSwim = false;
			CanDash = true;
			Falling = false;
			BottomLeft.IsFalling = false;
			BottomRight.IsFalling = false;
			if (!dashing)
				if (current_animation == &GoRight[NumPlayer] || current_animation == &jumpR[NumPlayer])
					current_animation = &idle[NumPlayer];
				else if (current_animation == &GoLeft[NumPlayer] || current_animation == &jumpL[NumPlayer])
					current_animation = &idle2[NumPlayer];
		}

		break;
	case COLLIDER_CLIMB:
		App->audio->PlayFx(ladderfx);
		IsJumping = false;
		IsJumping2 = false;
		CanDoAnotherJump = false;
		Falling = false;
		TouchingGround = true;
		AnimDoubleJump = false;
		CanClimb = true;
		CanJump = true;
		CanJump2 = false;
		velocity.y = 0;
		if (current_animation == &jumpR[NumPlayer] || current_animation == &jumpL[NumPlayer])
			current_animation = &ClimbIdle[NumPlayer];
		break;
	case COLLIDER_WATER:
		App->audio->PlayFx(waterfx);
		CanSwim = true;
		TouchingGround = true;
		CanClimb = false;
		velocity.y = 0;
		break;
	case COLLIDER_NONE:
		CanClimb = false;
		CanSwim = false;
		break;
	case COLLIDER_SPIKES:
		velocity.y = 0;
		TouchingGround = true;
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
		App->scene->SpawnEnemies();
		break;
	case COLLIDER_ROPE:
		TouchingGround = true;
		CanClimb = true;
		CanJump = true;
		CanJump2 = false;
		velocity.y = 0;
		break;
	case COLLIDER_WIN:
		TouchingGround = true;
		App->entitymanager->DeleteEnemies();
		App->scene->active = false;
		App->entitymanager->ActiveGame = false;
		App->collision->active = false;
		App->map->active = false;
		App->choose->start = false;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->choose->GameOn = false;
		break;
	}
}

void Player::Die()//What happens when the player die
{
	current_animation = &Death[NumPlayer];
	//App->audio->PlayFx(deathfx);
	if (Death[NumPlayer].SeeCurrentFrame() == 1)
		App->audio->PlayFx(deathfx2);
	if (Death[NumPlayer].Finished()) {
		if (App->scene->KnowMap == 0) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		}
		if (App->scene->KnowMap == 1) {
			App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
		}
		Spawn();
	}
}

void Player::Fall()//What happens when the player falls
{
	if (App->scene->KnowMap == 0) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
	}
	if (App->scene->KnowMap == 1) {
		App->map->ChangeMap(App->scene->map_name[App->scene->KnowMap]);
	}
	Spawn();
}

void Player::Spawn()
{
	NoInput = false;
	CanJump = true;
	CanClimb = false;
	CanSwim = false;
	Death[NumPlayer].current_frame = 0.0f;
	Death[NumPlayer].loops = 0;
	current_animation = &idle[NumPlayer];
	if (App->scene->KnowMap == 0) {
		position.x = App->entitymanager->positionStartMap1.x;
		position.y = App->entitymanager->positionStartMap1.y;
	}
	if (App->scene->KnowMap == 1) {
		position.x = App->entitymanager->positionStartMap2.x;
		position.y = App->entitymanager->positionStartMap2.y;
	}
	Death[NumPlayer].current_frame = 0.0f;
	Death[NumPlayer].loops = 0;
}
Animation Player::LoadPushbacks(int playernumber, pugi::xml_node& config, p2SString NameAnim) const
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

void Player::ChangePlayer(const int playernumber)
{
	BROFILER_CATEGORY("Player: ChangePlayer", Profiler::Color::Blue);
	if (NumPlayer != playernumber) {
		App->tex->UnLoad(texture);
		texture = App->tex->Load(sprites_name[playernumber].GetString());
		App->collision->ColliderCleanUpPlayer();
		NumPlayer = playernumber;
		current_animation = &idle[NumPlayer];
		switch (playernumber) {
		case 0:
			coll = App->collision->AddCollider({ 0, 0, playerwidth, playerheight }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
			break;
		case 1:
			position.y -= 17;
			coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
			break;
		case 2:
			position.y -= 17;
			coll = App->collision->AddCollider({ 0, 0, 67, 93 }, COLLIDER_PLAYER, (j1Module*)App->entitymanager);
			break;
		}
	}
}

void Player::GoJump(float dt)
{
	if (Jump && CanJump && !CanSwim && !God && !IsJumping) { //If you clicked the jump button and you are able to jump(always except you just jumpt) you can jump
		IsJumping = true;
		TouchingGround = false;
		Time = 0;
		JumpSpeed = AuxJumpSpeed;
		starttime = SDL_GetTicks();
		App->particles->AddParticle(App->particles->smokeBottom, position.x, position.y, COLLIDER_NONE, { 0,0 });
	}
	if (IsJumping) { //if you are able to jump, determine the animation and direction of the jump
					 //Time = Time * dt;

		float TIME = SDL_GetTicks();
		Time += 1;
		LOG("TIME %f", TIME);
		CanJump = false;
		if (TIME - starttime == 0)
			App->audio->PlayFx(jumpfx);
		if (TIME - starttime > 20 && TIME - starttime < 300) {
			JumpSpeed -= 2750.0f *dt;
		}
		if (TIME - starttime <= 300 && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300 && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (TIME - starttime >= 300) {
			IsJumping = false;
			CanJump2 = true;
			CanJump = false;
			Falling = true;
			JumpSpeed = AuxJumpSpeed;
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

void Player::GoSwim(float dt)
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
		position.y -= -SpeedSwimDown * dt;
	}

}

void Player::GoClimb(float dt)
{
	if (CanClimb && GoUp) {
		position.y -= SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && GoDown) {
		position.y -= -SpeedClimb * dt;
		current_animation = &Climb[NumPlayer];
	}
	if (CanClimb && !GoUp && !GoDown)
		current_animation = &ClimbIdle[NumPlayer];

}

void Player::Move_Left_Right(float dt)
{
	if (!BottomLeft.IsFalling && !BottomRight.IsFalling) {
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
			if (Falling)
				current_animation = &jumpR[NumPlayer];
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
			if (Falling)
				current_animation = &jumpL[NumPlayer];
		}
		if (WalkRight && WalkLeft) {
			if (!CanSwim)
				current_animation = &idle[NumPlayer];
			if (CanSwim)
				current_animation = &SwimRight[NumPlayer];
			if (CanClimb) {
				current_animation = &Climb[NumPlayer];
			}
			if (Falling)
				current_animation = &jumpR[NumPlayer];
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
		if (Falling&&current_animation == &idle[NumPlayer])
			current_animation = &jumpR[NumPlayer];
		if (Falling&&current_animation == &idle2[NumPlayer])
			current_animation = &jumpL[NumPlayer];
	}
}

void Player::Camera(float dt)
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

	/*if (CamRect.x + CamRect.w <= position.x + playerwidth) { //WHEN THE PLAYER MOVES RIGHT
	CamRect.x += (SpeedWalk + 1000*dt) * dt;
	App->render->camera.x -= (SpeedWalk + 2000 * dt) * dt;
	//App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	if (CamRect.x >= position.x) { //WHEN THE PLAYER MOVES LEFT
	CamRect.x -= (SpeedWalk + 1000 * dt) * dt;
	App->render->camera.x += (SpeedWalk + 1000 * dt) * dt;
	//App->render->camera.x = -position.x + (App->render->camera.w / 2);
	}
	if (position.y <= CamRect.y && cameraon) { //WHEN THE PLAYER GOES UP
	CamRect.y -= (JumpSpeed + 1000 * dt) * dt;
	//App->render->camera.y += 100 * dt;
	App->render->camera.y += (SpeedWalk + 1000 * dt) * dt;
	}
	if (position.y + playerHeight >= CamRect.y + CamRect.h) { //WHEN THE PLAYER GOES DOWN
	//App->render->camera.y = -position.y + (App->render->camera.h / 2);
	if (CanClimb)
	CamRect.y += SpeedClimb * dt;
	else CamRect.y -= gravity;
	App->render->camera.y += (gravity);

	}*/
	if (-App->render->camera.x >= position.x) //PLAYER CAN NOT GO BACK
		position.x += SpeedWalk * dt;










}

void Player::DoDash(float dt)
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && Hability && CanDash) {
		App->audio->PlayFx(dashfx);
		JumpSpeed = AuxJumpSpeed;
		dashing = true;
		dashR.DashRight = true;
	}
	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && Hability && CanDash) {
		App->audio->PlayFx(dashfx);
		JumpSpeed = AuxJumpSpeed;
		dashing = true;
		dashL.DashLeft = true;
	}
	if (dashing && dashR.DashRight) {
		velocity.y = 0;
		if (dashR.StartDash.current_frame == 0) {
			//	position.x -= 60;
			current_animation = &dashR.StartDash;
		}
		if (dashR.StartDash.Finished()) {
			++dashR.DashCont;
			current_animation = &dashR.Dashing;
			position.x += 1800 * dt;
			if (dashR.DashCont >= dashR.DashTime) {
				position.x -= 1600 * dt;
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
		velocity.y = 0;
		if (dashL.StartDash.current_frame == 0) {
			current_animation = &dashL.StartDash;
		}
		if (dashL.StartDash.Finished()) {
			++dashL.DashCont;
			current_animation = &dashL.Dashing;

			position.x -= 1800 * dt;
			if (dashL.DashCont >= dashL.DashTime) {
				position.x += 1600 * dt;
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


void Player::ShootLaser(float dt)
{
	if ((current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) && (Hability && !laserR.IsShooting)) {
		laserR.StartShooting = true;
		App->audio->PlayFx(laserfx);
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
		laserR.position.x += laserR.velocity.x * dt;
		laserR.coll->SetPos(laserR.position.x - 10, laserR.position.y + 22);
		App->render->Blit(texture, laserR.position.x - 10, laserR.position.y + 22, &(laserR.anim.GetCurrentFrame(dt)));
	}

	if ((current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) && (Hability && !laserL.IsShooting)) {
		laserL.StartShooting = true;
		App->audio->PlayFx(laserfx);
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
		laserL.position.x += laserL.velocity.x * dt;
		laserL.coll->SetPos(laserL.position.x - 10, laserL.position.y + 22);
		App->render->Blit(texture, laserL.position.x - 10, laserL.position.y + 22, &(laserL.anim.GetCurrentFrame(dt)));
	}
}

void Player::DoubleJump(float dt)
{
	if (CanJump2 && Jump && !IsJumping && CanDoAnotherJump) {
		AnimDoubleJump = true;
		IsJumping2 = true;
		Falling = false;
		Jump2Complete = true;
		Time = 0;
		starttime = SDL_GetTicks();
		JumpSpeed = AuxJumpSpeed;
	}
	if (Jump && IsJumping && !CanJump2 && CanDoAnotherJump) {
		AnimDoubleJump = true;
		IsJumping = false;
		CanJump2 = true;
		Falling = false;
		CanJump = false;
		Jump2Complete = true;
		Time = 0;
		starttime = SDL_GetTicks();
		JumpSpeed = AuxJumpSpeed;
		IsJumping2 = true;
	}
	if (AnimDoubleJump) {
		if (current_animation == &jumpL[NumPlayer] || current_animation == &GoLeft[NumPlayer])
			App->render->Blit(texture, position.x + 13, position.y + playerHeight, &(doubleJump.GetCurrentFrame(dt)));
		if (current_animation == &jumpR[NumPlayer] || current_animation == &GoRight[NumPlayer])
			App->render->Blit(texture, position.x + 13, position.y + playerHeight, &(doubleJump.GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL);
	}
	if (IsJumping2) { //if you are able to jump, determine the animation and direction of the jump
		CanDoAnotherJump = false;
		Time += 100 * dt;
		float TIME = SDL_GetTicks();
		if (TIME - starttime == 0)
			App->audio->PlayFx(jumpfx);
		if (TIME - starttime > 20 && TIME - starttime < 300) {
			JumpSpeed -= 2750.0f *dt;
		}
		if (TIME - starttime <= 300 && WalkRight) {
			current_animation = &jumpR[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300 && WalkLeft) {
			current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		else if (TIME - starttime <= 300) {
			if (current_animation == &idle[NumPlayer])
				current_animation = &jumpR[NumPlayer];
			if (current_animation == &idle2[NumPlayer])
				current_animation = &jumpL[NumPlayer];
			position.y -= JumpSpeed * dt;
		}
		if (TIME - starttime >= 300) {
			FallingJump2 = true;
			AnimDoubleJump = false;
			IsJumping2 = false;
			CanJump2 = false;
			Falling = true;
			JumpSpeed = AuxJumpSpeed;
			Jump2Complete = false;
		}
	}
}

void Player::BottomFall(float dt)
{
	if (Hability && IsJumping2) {
		if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			AnimDoubleJump = false;
			BottomRight.anim.loops = 0;
			BottomRight.anim.current_frame = 0.0f;
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			AnimDoubleJump = false;
			BottomLeft.anim.loops = 0;
			BottomLeft.anim.current_frame = 0.0f;
			BottomLeft.IsFalling = true;
		}

		IsJumping2 = false;
		CanJump2 = false;
	}
	if (Hability && FallingJump2) {
		if (current_animation == &GoRight[NumPlayer] || current_animation == &idle[NumPlayer] || current_animation == &jumpR[NumPlayer]) {
			BottomRight.anim.loops = 0;
			BottomRight.anim.current_frame = 0.0f;
			BottomRight.IsFalling = true;
		}
		else if (current_animation == &GoLeft[NumPlayer] || current_animation == &idle2[NumPlayer] || current_animation == &jumpL[NumPlayer]) {
			BottomLeft.anim.loops = 0;
			BottomLeft.anim.current_frame = 0.0f;
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

void Player::Gravity(float dt)
{
	gravity = auxGravity;
	gravity = gravity * dt;
	position.y -= gravity;
}

void Player::SetCamera()
{
	App->render->camera.x = -300;
	CamRect.x = 580;
	CamRect.y = 480;
	CamRect.w = 300;
	CamRect.h = playerHeight + playerHeight;
}

void Player::Dash::ResetDashAnims()
{
	StartDash.current_frame = 0.0f;
	StartDash.loops = 0;

	FinishDash.current_frame = 0.0f;
	FinishDash.loops = 0;

	DashLeft = false;
	DashRight = false;
	DashCont = 0;

}
