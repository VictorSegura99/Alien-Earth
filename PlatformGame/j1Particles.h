#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 500

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	SDL_Texture* texture = nullptr;
	char* fx = nullptr;
	iPoint position;
	iPoint impactPosition = { NULL,NULL };
	//bool impactPositioning = false;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	Uint32 damage = 0;
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
	// variables to instantiate collision particle
	Particle* onCollisionWallParticle = nullptr;
	Particle* onCollisionGeneralParticle = nullptr;
	Particle* deathParticle = nullptr;

};

class j1Particles : public j1Module
{
public:

	j1Particles();
	~j1Particles();

	bool Start();
	bool Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	//void AddParticle(const Particle& particle, Animation& sourceAnim, int x, int y, Uint32 delay = 0, iPoint speed = { 0,0 }, Uint32 life = 0, char* fx = nullptr);
	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, iPoint speed = { 0,0 }, Uint32 delay = 0);

private:


	Particle* active[MAX_ACTIVE_PARTICLES];


	SDL_Texture* test = nullptr;

public:

};

#endif // __MODULEPARTICLES_H__