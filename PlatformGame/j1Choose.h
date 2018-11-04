#ifndef __J1CHOOSE_H__
#define __J1CHOOSE_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct SDL_Rect;

class j1Choose : public j1Module
{
public:

	j1Choose();

	// Destructor
	virtual ~j1Choose();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


public:
	SDL_Texture * yellow = nullptr;
	Animation YellowStand;

	SDL_Texture * pink = nullptr;
	Animation PinkStand;

	SDL_Texture * blue = nullptr;
	Animation BlueStand;

	bool start = false;
	SDL_Texture * ScreenStart = nullptr;
	SDL_Texture * texture = nullptr;
	SDL_Texture * choose1 = nullptr;
	SDL_Texture * choose2 = nullptr;
	SDL_Texture * choose3 = nullptr;
	p2SString file_texture;
	iPoint mouse;
	int playernumber = 1;
	bool GameOn = false;
};

#endif //
