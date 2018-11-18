#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Audio.h"
#include "j1Particles.h"
#include "Player.h"
#include "j1Collision.h"
#include "PugiXml\src\pugiconfig.hpp"
#include "PugiXml\src\pugixml.hpp"
#include "EntityManager.h"

#define MARGIN 20


#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	name.create("particles");
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");

	smokeBottom.anim.PushBack({ 0,663,100,86 });
	smokeBottom.anim.PushBack({ 101,663,103,86 });
	smokeBottom.anim.PushBack({ 204,663,105,86 });
	smokeBottom.anim.PushBack({ 309,663,70,86 });
	smokeBottom.anim.speed = 5.0f;
	smokeBottom.anim.loop = false;

	laserL.anim.PushBack({ 34,582,83,27 });
	laserL.anim.PushBack({ 124,582,83,27 });
	laserL.anim.speed = 10.0f;
	laserL.speed.x = -999;

	laserR.anim.PushBack({ 34,554,83,27 });
	laserR.anim.PushBack({ 124,554,83,27 });
	laserR.anim.speed = 10.0f;
	laserR.speed.x = 999;

	//Doublejump.anim = DoubleJump;
	Doublejump.anim.PushBack({ 0,802,98,65 });
	Doublejump.anim.PushBack({ 98,802,98,65 });
	Doublejump.anim.PushBack({ 196,802,98,65 });
	Doublejump.anim.PushBack({ 294,802,98,65 });
	Doublejump.anim.PushBack({ 391,802,98,65 });
	Doublejump.anim.PushBack({ 492,802,98,65 });
	Doublejump.anim.PushBack({ 590,802,98,65 });
	Doublejump.anim.PushBack({ 0,888,98,65 });
	Doublejump.anim.PushBack({ 98,888,98,65 });
	Doublejump.anim.PushBack({ 196,888,98,65 });
	Doublejump.anim.PushBack({ 294,888,98,65 });
	Doublejump.anim.speed = 40.0f;
	Doublejump.anim.loop = false;
	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");


	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

bool j1Particles::Awake(pugi::xml_node& config) 
{
	pugi::xml_node particles = config.child("particles");
	DoubleJump = LoadPushbacks(particles, "DoubleJump");

	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(App->entitymanager->GetPlayerData()->texture, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT)));
			if (p->fx_played == false)
			{
				p->fx_played = true;
			}
		}
	}

	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			if (collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT), collider_type, this);
			active[i] = p;
			break;
		}
	}

}

// TODO 5: Make so every time a particle hits a wall it triggers an explosion particle
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	position.x += (speed.x * App->entitymanager->GetPlayerData()->DT);
	position.y += (speed.y * App->entitymanager->GetPlayerData()->DT);

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}

Animation j1Particles::LoadPushbacks(pugi::xml_node& config, p2SString NameAnim) const
{
	p2SString XML_Name_Player_Anims;
	SDL_Rect rect;
	Animation anim;
	XML_Name_Player_Anims = "particles";

	for (pugi::xml_node frames = config.child(NameAnim.GetString()).child("frame"); frames; frames = frames.next_sibling("frame")) {
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