#include <math.h>
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Particles.h"
#include "jPlayer.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
	
	/*
	boss.anim.PushBack({ 320,367,32,3 });
	boss.speed.x -= 3;
	boss.life = BULLET_ENEMY_LIFE;
	

	boss2.anim.PushBack({ 283,255,6,6 });
	boss2.anim.PushBack({ 283,255,6,6 });
	boss2.anim.PushBack({ 283,255,6,6 });
	boss2.anim.PushBack({ 283,255,6,6 });
	boss2.anim.speed = 0.2f;
	boss2.speed.x -= 3;
	boss2.anim.loop = true;
	boss2.life = BULLET_ENEMY_LIFE;

	minibossdead.anim.PushBack({ 0,520,80,725 });
	minibossdead.life = 50000;
	*/

	laser.anim.PushBack({ 142,0,66,86 });
	laser.anim.speed = 0.3f;
	laser.speed.x = 5.0f;
	laser.anim.loop = true;
	laser.life = 10000;


}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	
	graphics = App->tex->Load("textures/Alien.png");

	laser.anim.PushBack({ 142,0,66,86 });
	laser.anim.speed = 0.3f;
	laser.speed.x = 5.0f;
	laser.anim.loop = true;
	laser.life = 10000;




	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{

	App->tex->UnLoad(graphics);

	// Unload fx

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
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// play the audio SFX
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
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[i] = p;
			break;
		}
	}
}

void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			//AddParticle(explosion, active[i]->position.x, active[i]->position.y);
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
	if (collider != nullptr) {
		collider->to_delete = true;
		collider = nullptr;
	}

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

	position.x += speed.x;
	position.y += speed.y;

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;

	return true;
}

