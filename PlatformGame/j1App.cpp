#include <iostream> 
#include <sstream> 
#include "p2Defs.h"
#include "p2Log.h"
#include "SDL/include/SDL.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Menu.h"
#include "j1Particles.h"
#include "j1Pathfinding.h"
#include "UI_Manager.h"
#include "j1FadeToBlack.h"
#include "j1Fonts.h"
#include "p2Point.h"

#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	collision = new j1Collision();
	entitymanager = new EntityManager();
	menu = new j1Menu();
	particles = new j1Particles();
	pathfinding = new j1PathFinding();
	ui_manager = new UI_Manager();
	fade = new j1FadeToBlack();
	fonts = new j1Fonts();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	
	AddModule(map);
	AddModule(scene);
	
	
	AddModule(pathfinding);
	AddModule(fonts);
	AddModule(collision);

	

	AddModule(particles);
	AddModule(entitymanager);
	AddModule(ui_manager);
	AddModule(menu);
	AddModule(fade);
	// render last to swap buffer
	AddModule(render);
	

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		// TODO 1: Read from config file your framerate cap

		int cap = app_config.attribute("framerate_cap").as_int();
		if (cap > 0)
		{
			framerate_cap = 1000 / cap;
		}



	}

	if (ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	BROFILER_CATEGORY("App: Start", Profiler::Color::PeachPuff);
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	App->render->surface=App->tex->LoadMouse("textures/pointer.png");
	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("App: Update", Profiler::Color::PeachPuff);
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->capactivated = !App->capactivated;
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file, char* file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file(file);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	BROFILER_CATEGORY("App: PrepareUpdate", Profiler::Color::PeachPuff);
	frame_count++;
	last_sec_frame_count++;
	dt = frame_time.ReadSec();
	frame_time.Start();
	ptimer.Start();
	LOG("dt is: %.6f", dt);
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("App: FinishUpdate", Profiler::Color::PeachPuff);
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow(WantToLoad);

	// Framerate calculations --

	if(last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;
	if (capactivated&&App->render->vsync) {
		static char title[256];
		sprintf_s(title, 256, "Alien Earth v1.0 | FPS: %i | Avg.FPS: %.2f | Ms Last Frame %02u | 30FPSCap: On | Vsync: On", frames_on_last_update, avg_fps, last_frame_ms);
		App->win->SetTitle(title);
	}
	else if (capactivated&&!App->render->vsync) {
		static char title[256];
		sprintf_s(title, 256, "Alien Earth v1.0 | FPS: %i | Avg.FPS: %.2f | Ms Last Frame %02u | 30FPSCap: On | Vsync: Off", frames_on_last_update, avg_fps, last_frame_ms);
		App->win->SetTitle(title);
	}
	else if (!capactivated&&App->render->vsync) {
		static char title[256];
		sprintf_s(title, 256, "Alien Earth v1.0 | FPS: %i | Avg.FPS: %.2f | Ms Last Frame %02u | 30FPSCap: Off | Vsync: On", frames_on_last_update, avg_fps, last_frame_ms);
		App->win->SetTitle(title);
	}
	else {
		static char title[256];
		sprintf_s(title, 256, "Alien Earth v1.0 | FPS: %i | Avg.FPS: %.2f | Ms Last Frame %02u | 30FPSCap: Off | Vsync: Off", frames_on_last_update, avg_fps, last_frame_ms);
		App->win->SetTitle(title);
	}

	if (framerate_cap > 0 && last_frame_ms < framerate_cap&&capactivated)
	{
		j1PerfTimer time;
		float delaytimestart = time.ReadMs();
		SDL_Delay(framerate_cap - last_frame_ms);
		float delaytimefinish = time.ReadMs();
		LOG("We waited for %i milliseconds and got back in %.6f", framerate_cap - last_frame_ms, delaytimefinish - delaytimestart);
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate", Profiler::Color::Green);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	iPoint mouseposition;
	App->input->GetMousePosition(mouseposition.x, mouseposition.y);
	SDL_CreateColorCursor(App->render->surface, mouseposition.x, mouseposition.y);

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("Update", Profiler::Color::Orchid);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate", Profiler::Color::Red);
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		if (item!=NULL)
			ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame(const char* file, bool WantToLoad)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	this->WantToLoad = WantToLoad;
	want_to_load = true;
	load_game.create(file);
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.create(file);
}

// ---------------------------------------


bool j1App::LoadGameNow(bool WantToLoad)
{
	bool ret = false;
	CanLoad = false;
	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());
	if (WantToLoad) {
		if (result != NULL)
		{
			ret = true;
			CanLoad = true;

			LOG("Loading new Game State from %s...", load_game.GetString());

			root = data.child("game_state");

			p2List_item<j1Module*>* item = modules.start;


			while (item != NULL && ret == true)
			{
				ret = item->data->Load(root.child(item->data->name.GetString()));
				item = item->next;
			}

			data.reset();
			if (ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");


		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());
	}
	else {
		if (result == NULL) {
			CanLoad = false;
		}
		else CanLoad = true;
	}
	

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}