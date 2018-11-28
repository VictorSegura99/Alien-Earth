#ifndef __LABEL_H__
#define __LABEL_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Label : public UI_Element {
public:

	Label(int x, int y, p2SString name, bool CanBeMoved);
	virtual ~Label();
	bool Update(float dt);
public:
	p2SString name;
	iPoint Distance;
	bool CanBeMoved = false;
};

#endif
