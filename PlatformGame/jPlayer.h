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
	bool PreUpdate();

	//// Called each loop iteration
	//bool PostUpdate();

	//Called every loop iteration
	bool Update(float dt);

	//Called before quitting
	bool CleanUp();

public:
	
	Animation* current_animation = nullptr;
	Animation idle;
	fPoint position;
	p2SString		folder;
	p2SString		texture_path;
	const char*		tryy=nullptr;
	SDL_Texture* texture;
	

	bool LoadPushbacks();

};


#endif