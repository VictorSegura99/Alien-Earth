#ifndef __JPLAYER_H_
#define __JPLAYER_H_

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"

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

public:
	
	Animation* current_animation = nullptr;
	Animation idle;
	Animation idle2;
	Animation GoLeft;
	Animation GoRight;
	fPoint position;
	float initialX = 0.0f;
	float initialY = 0.0f;
	SDL_Texture* texture = nullptr;
	p2List<p2SString*> sprites_name;
	p2SString * data = new p2SString;
	bool NextMap = false;
	bool anime = true;



};


#endif