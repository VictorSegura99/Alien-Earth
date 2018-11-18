#include "MovingPlatform.h"
#include "Spider.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1PerfTimer.h"
#include "EntityManager.h"
#include "Player.h"
#include "Brofiler/Brofiler.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

MovingPlatform::MovingPlatform(int x, int y)
{
	position.x = x;
	position.y = y;
	original_pos.x = x;
	original_pos.y = y;

}

MovingPlatform::~MovingPlatform()
{
}

bool MovingPlatform::PreUpdate()
{

	return true;
}

bool MovingPlatform::Update(float dt)
{

	if (Start) {
		Start = false;
		StartTime = SDL_GetTicks();
		App->entitymanager->GetPlayerData()->MoveRight = true;
	}
	if (App->entitymanager->GetPlayerData()->MoveRight) {
		position.x += Speed * dt;
		if (SDL_GetTicks() - StartTime >= 700) {
			App->entitymanager->GetPlayerData()->MoveRight = false;
			App->entitymanager->GetPlayerData()->MoveLeft = true;
			StartTime = SDL_GetTicks();
		}
	}
	if (App->entitymanager->GetPlayerData()->MoveLeft) {
		position.x -= Speed * dt;
		if (SDL_GetTicks() - StartTime >= 700) {
			App->entitymanager->GetPlayerData()->MoveRight = true;
			App->entitymanager->GetPlayerData()->MoveLeft = false;
			StartTime = SDL_GetTicks();
		}
	}

	return true;
}

bool MovingPlatform::PostUpdate()
{
	return true;
}

void MovingPlatform::Draw(float dt)
{

	if (coll == nullptr)
		coll = App->collision->AddCollider({ 0,0,72,53 }, COLLIDER_MOVINGPLATFORM, (j1Module*)App->entitymanager);
	coll->SetPos(position.x, position.y);

}

bool MovingPlatform::CleanUp()
{
	App->tex->UnLoad(texture);
	if (coll)
		coll->to_delete = true;
	return true;
}

void MovingPlatform::OnCollision(Collider * c2)
{

}
