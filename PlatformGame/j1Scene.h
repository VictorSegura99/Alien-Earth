#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Entity.h"

struct SDL_Texture;
class UI_Element;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void SpawnEnemies();

	void CreatePauseMenu();
	void PauseMenu(float dt);
	void DeletePauseMenu();




public:
	int KnowMap = 0; // KnowMap == 0 Map1 is working // KnowMap == 1 Map2 is working  
	p2List<p2SString*> map_name;
	p2SString Song;
	p2SString SongMenu;
	//Entity* player = nullptr;
	SDL_Texture* TutorialJeff;
	SDL_Texture* TutorialJane;
	SDL_Texture* TutorialJerry;
	p2SString tutorial[3];
	bool CanStart = false;
	Entity* bat1 = nullptr;
	bool WantToSpawnEnemies = true;
	bool GamePaused = false;
	
	//MENU PAUSA

	UI_Element* imagePAUSE = nullptr;
	UI_Element* buttonRESUME = nullptr;
	UI_Element* buttonGOMAINMENU = nullptr;
	UI_Element* sliderVOLUMEMUSIC = nullptr;
	UI_Element* labelMUSICVOLUME = nullptr;
	UI_Element* sliderVOLUMEFX = nullptr;
	UI_Element* labelPAUSE = nullptr;
	UI_Element* labelVOLUMEFX = nullptr;
	UI_Element* sliderGENERALSOUND = nullptr;
	UI_Element* labelGENERALSOUND = nullptr;
	UI_Element* checkboxSOUND = nullptr;
	UI_Element* labelSOUND = nullptr;
	UI_Element* image = nullptr;
	int x = 170;
	int y = 950;
	bool Positioned = false;
	bool MenuDone = false;

};

#endif // __j1SCENE_H__