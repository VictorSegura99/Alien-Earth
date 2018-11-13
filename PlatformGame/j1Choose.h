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

	Animation LoadGigantAliensAnimations(int playernumber, pugi::xml_node&, p2SString NameAnim) const;

public:

	SDL_Texture * yellow = nullptr;
	Animation YellowStand;
	Animation YellowWalk;

	SDL_Texture * pink = nullptr;
	Animation PinkStand;
	Animation PinkWalk;

	SDL_Texture * blue = nullptr;
	Animation BlueStand;
	Animation BlueWalk;

	p2SString ChooseFx;
	p2SString IntroFx;

	bool start = false;
	SDL_Texture * ScreenStart = nullptr;
	SDL_Texture * NoChoose = nullptr;
	SDL_Texture * choose1 = nullptr;
	SDL_Texture * choose2 = nullptr;
	SDL_Texture * choose3 = nullptr;
	p2SString file_texture[5];
	iPoint mouse;
	int playernumber = -1;
	int MinY_ChooseRect = 0;
	int MaxY_ChooseRect = 0;
	int MinX_RectChoosePlayer1 = 0;
	int MaxX_RectChoosePlayer1 = 0;
	int MinX_RectChoosePlayer2 = 0;
	int MaxX_RectChoosePlayer2 = 0;
	int MinX_RectChoosePlayer3 = 0;
	int MaxX_RectChoosePlayer3 = 0;
	int PlayerNumber1 = 0;
	int PlayerNumber2 = 0;
	int PlayerNumber3 = 0;
	bool GameOn = false;
	bool repeat = false;
	bool repeat2 = false;
	bool repeat3 = false;

	unsigned int choosefx;
	unsigned int introfx;
};

#endif //
