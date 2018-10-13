#ifndef __JPLAYER_H_
#define __JPLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Collision.h"
struct SDL_Texture;
struct SDL_Rect;

class jPlayer : public j1Module
{
public:
	jPlayer();

	//Destructor
	virtual ~jPlayer();

	//Called before player is aviable
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	//// Called each loop iteration
	bool PreUpdate();
	
	//// Called each loop iteration
	bool PostUpdate();


	//Called every loop iteration
	bool Update(float dt);

	//Called before quitting
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void Die();
	void Fall();
	void AddFx(int fx, int repeat);

public:
	
	Animation* current_animation = nullptr;
	Animation idle;
	Animation idle2;
	Animation GoLeft;
	Animation GoRight;
	Animation jumpR;
	Animation jumpL;
	Animation SwimRight;
	Animation SwimLeft;
	Animation Climb;
	Animation ClimbIdle;
	Animation Death;

	SDL_Texture* texture = nullptr;

	p2SString sprites_name;
	p2SString	JumpFx;
	p2SString	DeathFx;

	Collider* coll = nullptr;

	fPoint position;

	bool WalkLeft = false;
	bool WalkRight = false;
	bool GoUp = false;
	bool GoDown = false;
	bool Idle = false;
	bool Jump = false;
	bool IsJumping = false;
	bool CanJump = true;
	bool CanClimb = false;
	bool CanSwim = false;
	bool death=false;
	bool fall = false;
	bool God = false;
	bool NextMap = false;

	float initialXmap1 = 0.0f;
	float initialYmap1 = 0.0f;
	float initialXmap2 = 0.0f;
	float initialYmap2 = 0.0f;
	float gravity = 0.0f;
	float JumpSpeed = 0.0f;
	float SpeedWalk = 0.0f;
	float SpeedClimb = 0.0f;
	float SpeedSwimUp = 0.0f;
	float SpeedSwimDown = 0.0f;
	float SpeedSwimLeftRight = 0.0f;

	int positionWinMap1 = 0;
	int startpointcameramap2 = 0;
	int jumpfx;
	int JumpTime = 0;

	uint Time = 0;
	
};


#endif