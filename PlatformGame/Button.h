#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

#define IDLE 349,102,190,49 
#define MOUSEON 349,50,190,49 
#define PRESSED  349,0,190,49 
#define WIDTH 190
#define HEIGHT 49


class Button : public UI_Element {
public:

	Button(int x, int y, int type, UI_Element* parent = nullptr, p2SString name = "NONE", int size = 0);
	virtual ~Button();
	bool Update(float dt);

public:
	
	int type = -1;
	unsigned int fXOn;
	int label_pos;

};

#endif
