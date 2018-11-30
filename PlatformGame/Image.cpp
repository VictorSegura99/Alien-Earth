#include "Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "UI_Manager.h"
#include "j1Audio.h"
#include "j1Input.h"

Image::Image(int x, int y, bool WantToBeMoved) : UI_Element(x, y)
{
	if (!WantToBeMoved) {
		Distance.x = position.x - (-App->render->camera.x);
		Distance.y = position.y - (-App->render->camera.y);
	}
	this->WantToBeMoved = WantToBeMoved;
	width = WIDTH;
	height = HEIGHT;
	png_pos = {IDLE};

}

Image::~Image()
{
}

bool Image::Update(float dt)
{
	if (!WantToBeMoved) {
		position.x = (-App->render->camera.x) + Distance.x;
		position.y = (-App->render->camera.y) + Distance.y;
	}

	return true;
}
