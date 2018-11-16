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
#include "EntityManager.h"

#define MARGIN 20

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	//animations for particles, avoiding repeat animations pushback rects
	smokeBottom.anim.PushBack({ 0,663,100,86 });
	smokeBottom.anim.PushBack({ 101,663,103,86 });
	smokeBottom.anim.PushBack({ 204,663,105,86 });
	smokeBottom.anim.PushBack({ 309,663,70,86 });
	smokeBottom.anim.speed = 1.0f;
	smokeBottom.life = 2100;
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");

	//unloading graphics

	//removing active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	//removing particles FX audio

	return true;
}

// Update: draw background
bool j1Particles::Update()
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

			//p->r = p->anim->GetCurrentFrame(); ---
			App->render->Blit(p->texture, p->position.x, p->position.y, &(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT)));
			if (p->fx_played == false && p->fx != nullptr)
			{
				p->fx_played = true;


			}
		}
	}

	return true;
}

//void ModuleParticles::AddParticle(const Particle& particle, Animation& sourceAnim, int x, int y, Uint32 delay, iPoint speed, Uint32 life, char* name)
void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, iPoint speed, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			if (speed.x != 0 || speed.y != 0) //if we send specific speed, defines it
			{
				p->speed = speed;
			}
			if (collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(App->entitymanager->GetPlayerData()->DT), collider_type, this);
			active[i] = p;
			break;
		}
	}
}

// -------------------------------------------------------------
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			//AddParticle(explosion, active[i]->position.x, active[i]->position.y);
			// checks if we are collisioning on walls, or anything else

			if (c2->type == COLLIDER_WALL_LEFT&& c2->type == COLLIDER_WALL_RIGHT) // if collider wall...
			{
				if (active[i]->onCollisionWallParticle != nullptr)
				{
					if (active[i]->impactPosition.x != NULL || active[i]->impactPosition.y != NULL)
					{
						AddParticle(*active[i]->onCollisionWallParticle, active[i]->position.x + active[i]->impactPosition.x,
							active[i]->position.y + active[i]->impactPosition.y, COLLIDER_NONE, { 0,0 }, 0);
					}
					else
						AddParticle(*active[i]->onCollisionWallParticle, active[i]->position.x, active[i]->position.y, COLLIDER_NONE, { 0,0 }, 0);
				}
			}
			//if (c2->type != COLLIDER_WALL) // anything collider
			//{
			if (active[i]->onCollisionGeneralParticle != nullptr)
				AddParticle(*active[i]->onCollisionGeneralParticle, active[i]->position.x + active[i]->impactPosition.x,
					active[i]->position.y + active[i]->impactPosition.y, COLLIDER_NONE, { 0,0 }, 0);
			//}

			//active[i]->texture = nullptr;
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed), fx(p.fx), born(p.born), life(p.life), texture(p.texture),
	damage(p.damage), onCollisionGeneralParticle(p.onCollisionGeneralParticle), onCollisionWallParticle(p.onCollisionWallParticle),
	impactPosition(p.impactPosition), deathParticle(p.deathParticle)
{}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update()
{
	bool ret = true;

	position.x += speed.x;
	position.y += speed.y;

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}