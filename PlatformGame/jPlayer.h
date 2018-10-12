#ifndef __JPLAYER_H_
#define __JPLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Collision.h"
struct SDL_Texture;
struct SDL_Rect;

enum States {
	IDLE,
	GORIGHT,
	GOLEFT,
	CLIMB,
	SWIMRIGHT,
	SWIMLEFT
};

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
	bool WalkLeft = false;
	bool WalkRight = false;
	bool Idle = false;
	bool Jump = false;
	bool CanClimb = false;
	fPoint position;
	bool IsJumping = false;
	uint KnowState;
	Collider* coll = nullptr;
	float initialXmap1 = 0.0f;
	float initialYmap1 = 0.0f;
	float initialXmap2 = 0.0f;
	float initialYmap2 = 0.0f;
	SDL_Texture* texture = nullptr;
	p2SString sprites_name;
	bool NextMap = false;
	int positionWinMap1 = 10780;
	int startpointcameramap2 = -60;
	float gravity = -4.8f;
	int jumpfx;
	bool GoUp = false;
	bool GoDown = false;
	bool CanJump = true;
	bool CanSwim = false;
	int JumpTime = 0;
	bool doublejump = false;
};


#endif