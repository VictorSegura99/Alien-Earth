#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Scene.h"

Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{

}



void Entity::Draw()
{


	
}

void Entity::OnCollision(Collider* collider)
{

}
