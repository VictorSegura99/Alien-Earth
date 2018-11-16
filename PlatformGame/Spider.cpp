#include "Spider.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Collision.h"
#include "EntityManager.h"
#include "Player.h"
#include "Brofiler/Brofiler.h"

Spider::Spider(int x, int y) : Entity(x,y)
{
	position.x = x;
	position.y = y;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		spider;

	config = App->LoadConfig(config_file);
	app_config = config.child("entity_manager").child("enemies");
	spider = app_config.child("Spider");

	sprites = spider.child("sprite").text().as_string();
	GoLeft = LoadPushbacks(spider, "GoLeft");
	GoRight = LoadPushbacks(spider, "GoRight");
	IdleLeft = LoadPushbacks(spider, "IdleLeft");
	IdleRight = LoadPushbacks(spider, "IdleRight");
	DieLeft = LoadPushbacks(spider, "DieLeft");
	DieRight = LoadPushbacks(spider, "DieRight");
	HitLeft = LoadPushbacks(spider, "HitLeft");
	HitRight = LoadPushbacks(spider, "HitRight");

	texture = App->tex->Load(sprites.GetString());
	current_animation = &IdleLeft;
	
}

Spider::~Spider()
{
}

bool Spider::Awake(pugi::xml_node & config)
{
	pugi::xml_node spider = config.child("enemies").child("Spider");

	sprites = spider.child("sprite").text().as_string();
	GoLeft = LoadPushbacks(spider, "GoLeft");
	GoRight = LoadPushbacks(spider, "GoRight");
	IdleLeft = LoadPushbacks(spider, "IdleLeft");
	IdleRight = LoadPushbacks(spider, "IdleRight");
	DieLeft = LoadPushbacks(spider, "DieLeft");
	DieRight = LoadPushbacks(spider, "DieRight");
	HitLeft = LoadPushbacks(spider, "HitLeft");
	HitRight = LoadPushbacks(spider, "HitRight");

	return true;
}

bool Spider::Start()
{

	texture = App->tex->Load(sprites.GetString());
	current_animation = &IdleLeft;
	

	return true;
}

bool Spider::PreUpdate()
{
	BROFILER_CATEGORY("Spider: PreUpdate", Profiler::Color::Green);

	
	return true;
}

bool Spider::PostUpdate()
{
	BROFILER_CATEGORY("Spider: PostUpdate", Profiler::Color::Green);
	
	if (coll->CanBeDeleted) {
		coll->CanBeDeleted = false;
		coll->to_delete = true;
		death = true;
		starttime = SDL_GetTicks();
	}
	if (death) {
		float TIME = SDL_GetTicks();
		if (current_animation == &HitLeft || current_animation == &GoLeft || current_animation == &IdleLeft)
			current_animation = &DieLeft;
		if (current_animation == &HitRight || current_animation == &GoRight || current_animation == &IdleRight)
			current_animation = &DieRight;
		if (TIME - starttime >= 2000)
			App->entitymanager->DeleteEntity(this);
	}

	return true;
}

bool Spider::Update(float dt)
{
	BROFILER_CATEGORY("Spider: Update", Profiler::Color::Green);
	return true;
}

bool Spider::Load(pugi::xml_node & spider)
{

	position.x = spider.child("position").attribute("x").as_float();
	position.y = spider.child("position").attribute("y").as_float();



	return true;
}

bool Spider::Save(pugi::xml_node & spider) const
{

	spider.append_child("position").append_attribute("x") = position.x;
	spider.child("position").append_attribute("y") = position.y;

	return true;
}

void Spider::Draw(float dt)
{

	App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
	if (coll == nullptr)
		coll = App->collision->AddCollider({ 0,0,72,53 }, COLLIDER_ENEMY, (j1Module*)App->entitymanager);
	coll->SetPos(position.x, position.y);
}

bool Spider::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Spider::OnCollision(Collider * c2)
{
	switch (c2->type) {
	case COLLIDER_PARTICLE:
		coll->CanBeDeleted = true;
	}
}

Animation Spider::LoadPushbacks(pugi::xml_node &config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "AnimationsSpider";

	for (pugi::xml_node frames = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
		rect.x = frames.attribute("x").as_int();
		rect.y = frames.attribute("y").as_int();
		rect.w = frames.attribute("w").as_int();
		rect.h = frames.attribute("h").as_int();
		anim.PushBack({ rect.x,rect.y,rect.w,rect.h });
	}
	anim.speed = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("speed").as_float();
	anim.loop = config.child(XML_Name_Player_Anims.GetString()).child(NameAnim.GetString()).attribute("loop").as_bool();

	return anim;
}
