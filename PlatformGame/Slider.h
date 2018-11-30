#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Slider : public UI_Element {
public:

	Slider(int x, int y);
	virtual ~Slider();
	bool Update(float dt);
	void LookLimits();

private:
	UI_Element* image = nullptr;
	bool repeat2 = true;
	int distance;
	iPoint mouse_pos;
	float value = -1;
};

#endif