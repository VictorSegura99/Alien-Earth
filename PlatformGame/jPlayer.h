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
	bool PostUpdate();

	//// Called each loop iteration
	//bool PostUpdate();

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
	Animation Jump;
	fPoint position;
	bool IsJumping = false;
	Collider* coll = nullptr;
	float initialXmap1 = 0.0f;
	float initialYmap1 = 0.0f;
	float initialXmap2 = 0.0f;
	float initialYmap2 = 0.0f;
	SDL_Texture* texture = nullptr;
	p2SString sprites_name;
	bool NextMap = false;
	bool anime = true;
	int positionWinMap1 = 10780;
	int startpointcameramap2 = -60;
	float gravity = -4.8f;
};


#endif