#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"

struct SDL_Texture;
struct Collider;

class Entity
{
public:

	Entity(int x, int y);
	virtual ~Entity();


	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual void Draw();
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };
	virtual void OnCollision(Collider* collider);

	void UsePlayerFunctions(uint NumFun) const;

public:


	int type = 0;
	iPoint position;
	

};

#endif