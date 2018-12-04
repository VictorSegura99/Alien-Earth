#include "Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "EntityManager.h"
Coin::Coin(int x, int y)
{
	position.x = x;
	position.y = y;
	this->x = x;
	this->y = y;
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		coin;


	config = App->LoadConfig(config_file);
	coin = config.child("entity_manager").child("Coin");
	sprites = coin.child("sprites").text().as_string();
	texture = App->tex->Load(sprites.GetString());

	Idle.PushBack({ 0,0,47,47 });
}

Coin::~Coin()
{
}

bool Coin::Update(float dt)
{

	return true;
}

bool Coin::Load(pugi::xml_node &)
{
	return true;
}

bool Coin::Save(pugi::xml_node &) const
{
	return true;
}

void Coin::Draw(float dt)
{
	if (firsttime) {
		coll = App->collision->AddCollider({ x,y,47,47 }, COLLIDER_COIN, (j1Module*)App->entitymanager);
		firsttime = false;
	}
	App->render->Blit(texture, position.x, position.y, &(Idle.GetCurrentFrame(dt)));

}

bool Coin::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void Coin::OnCollision(Collider * c2)
{
	coll->to_delete = true;
	App->entitymanager->DeleteEntity(this);
}
