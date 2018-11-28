#include "Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Image::Image(int x, int y)
{
	
	position.x = x;
	position.y = y;
	original_pos.x = x;
	original_pos.y = y;
	Distance.x = position.x - (-App->render->camera.x);
	Distance.y = position.y - (-App->render->camera.y);
	NoPressedNoMouseOn = { 336, 398, 328, 103 };
	weight = 328;
	height = 103;
	png_pos = NoPressedNoMouseOn;

}

Image::~Image()
{
}

bool Image::Update(float dt)
{

	position.x = (-App->render->camera.x) + Distance.x;
	position.y = (-App->render->camera.y) + Distance.y;

	return true;
}
