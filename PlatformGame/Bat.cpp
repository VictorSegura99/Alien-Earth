#include "Bat.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Collision.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"
#include "Player.h"
#include "p2DynArray.h"
#include "j1Map.h"
#include "Player.h"
#include "Brofiler/Brofiler.h"

Bat::Bat(int x, int y) : Entity(x, y)
{
	position.x = x;
	position.y = y;
	gravity = 433.333f;
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		Bat;

	config = App->LoadConfig(config_file);
	app_config = config.child("entity_manager").child("enemies");
	Bat = app_config.child("Bat");

	sprites = Bat.child("sprite").text().as_string();
	GoLeft = LoadPushbacks(Bat, "GoLeft");
	GoRight = LoadPushbacks(Bat, "GoRight");
	Idle = LoadPushbacks(Bat, "Idle");
	DieLeft = LoadPushbacks(Bat, "DieLeft");
	DieRight = LoadPushbacks(Bat, "DieRight");
	HitLeft = LoadPushbacks(Bat, "HitLeft");
	HitRight = LoadPushbacks(Bat, "HitRight");

	texture = App->tex->Load(sprites.GetString());

}

Bat::~Bat()
{
}

bool Bat::PreUpdate()
{
	BROFILER_CATEGORY("Bat: PreUpdate", Profiler::Color::Green);


	return true;
}

bool Bat::PostUpdate()
{
	BROFILER_CATEGORY("Bat: PostUpdate", Profiler::Color::Green);

	if (coll->CanBeDeleted) {
		coll->CanBeDeleted = false;
		coll->type = COLLIDER_PLAYER;
		death = true;
		starttime = SDL_GetTicks();
	}
	if (death) {
		float TIME = SDL_GetTicks();
		//position.y -= App->entitymanager->GetPlayerData()->gravity*App->entitymanager->GetPlayerData()->DT;
		if (TIME - starttime >= 2000)
			App->entitymanager->DeleteEntity(this);
	}

	return true;
}

bool Bat::Update(float dt)
{
	BROFILER_CATEGORY("Bat: Update", Profiler::Color::Green);

	AnimationLogic();

	float x = App->entitymanager->GetPlayerData()->position.x;
	float y = App->entitymanager->GetPlayerData()->position.y;
	
	if (position.x - x < 400 && x - position.x < 400 && !death) {
		iPoint origin = App->map->WorldToMap(position.x, position.y);
		iPoint player_position = App->map->WorldToMap(App->entitymanager->GetPlayerData()->position.x, App->entitymanager->GetPlayerData()->position.y - App->entitymanager->GetPlayerData()->coll->rect.h);
		if (position.DistanceTo(App->entitymanager->GetPlayerData()->position)) {
			App->pathfinding->CreatePath(origin, player_position);
			const p2DynArray<iPoint>* entity_path = App->pathfinding->GetLastPath();
			for (int i = 0; i < entity_path->Count(); i++) {
				pathfinding_path.PushBack(*entity_path->At(i));
			}
		}
		if (pathfinding_path.Count() > 1) {
			velocity.x = (pathfinding_path[1].x - pathfinding_path[0].x) * 60;
		}
		if (App->entitymanager->GetPlayerData()->position.x < position.x) {
			position.x += velocity.x * dt;
		}
		else {
			position.x += -velocity.x * dt;
		}
		if (App->entitymanager->GetPlayerData()->position.y > position.y) {
			position.y += 50 * dt;
		}
		if (App->entitymanager->GetPlayerData()->position.y < position.y) {
			position.y -= 50 * dt;
		}
	}
	if (death && coll->type == COLLIDER_PLAYER) {
		position.y += 433.33f * dt;
	}
	if (position.x + 100< -App->render->camera.x)
		App->entitymanager->DeleteEntity(this);

	return true;
}

bool Bat::Load(pugi::xml_node & Bat)
{

	position.x = Bat.child("position").attribute("x").as_float();
	position.y = Bat.child("position").attribute("y").as_float();



	return true;
}

bool Bat::Save(pugi::xml_node & Bat) const
{

	Bat.append_child("position").append_attribute("x") = position.x;
	Bat.child("position").append_attribute("y") = position.y;

	return true;
}

void Bat::Draw(float dt)
{
	if (current_animation == &DieLeft || current_animation == &DieRight)
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)), SDL_FLIP_VERTICAL);
	else
		App->render->Blit(texture, position.x, position.y, &(current_animation->GetCurrentFrame(dt)));
	if (coll == nullptr)
		coll = App->collision->AddCollider({ 0,0,70,47 }, COLLIDER_ENEMY, (j1Module*)App->entitymanager);
	coll->SetPos(position.x, position.y);
}

bool Bat::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Bat::OnCollision(Collider * c2)
{
	switch (c2->type) {
	case COLLIDER_PARTICLE:
		coll->CanBeDeleted = true;
		break;
	case COLLIDER_GROUND:
		if (death) {
			coll->type = COLLIDER_NONE;
		}
		
		
	}
}

Animation Bat::LoadPushbacks(pugi::xml_node &config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "AnimationsBat";

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


void Bat::AnimationLogic() {
	if (App->entitymanager->GetPlayerData()->position.x + 5 <= position.x&&!death) {
		current_animation = &GoLeft;
	}
	if (App->entitymanager->GetPlayerData()->position.x > position.x&&!death) {
		current_animation = &GoRight;
	}
	if (death) {
		if (current_animation == &HitLeft || current_animation == &GoLeft)
			current_animation = &DieLeft;
		if (current_animation == &HitRight || current_animation == &GoRight)
			current_animation = &DieRight;
	}
}