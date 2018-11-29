#include "Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Image::Image(int x, int y, int type) : UI_Element(x, y)
{
	
	switch (type) {
	case 1: {
		this->type = type;
		Distance.x = position.x - (-App->render->camera.x);
		Distance.y = position.y - (-App->render->camera.y);
		NoPressedNoMouseOn = { 336, 398, 328, 103 };
		width = 328;
		height = 103;
		png_pos = NoPressedNoMouseOn;
		break; }
	case 2: {
		this->type = type;
		App->tex->UnLoad(atlas);
		atlas = App->tex->Load("textures/Settings.png");
		png_pos = { 0,0,1000,1000 };
		break; }
	}
	

}

Image::~Image()
{
}

bool Image::Update(float dt)
{
	if (type == 1) {
		position.x = (-App->render->camera.x) + Distance.x;
		position.y = (-App->render->camera.y) + Distance.y;
	}
	

	return true;
}
