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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

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
	p2SString	WaterFx;
	p2SString DeathFx;
	p2SString DeathFx2;
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

	int startmap2 = 0;
	int maxYcam = 0;
	int minYcam = 0;
	int lowcam = 0;
	int positionWinMap1 = 0;
	int startpointcameramap2 = 0;
	int JumpTime = 0;
	int playerwidth = 0;
	int playerHeight = 0;
	int playerheight = 0;
	int finalmapplayer = 0;
	int finalmap = 0;

	unsigned int jumpfx;
	unsigned int waterfx;
	unsigned int deathfx;
	unsigned int deathfx2;
	uint Time = 0;
	
};


#endif