#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Button : public UI_Element {
public:

	Button(int x, int y, int type, p2SString name = "NONE", int size = 0);
	virtual ~Button();
	bool Update(float dt);

private:
	UI_Element * label = nullptr;
	int type = -1;
	unsigned int fXOn;

};

#endif
