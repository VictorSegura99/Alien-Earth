#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "p2Animation.h"
#include "p2Point.h"
#include "UI_Element.h"
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class Image : public UI_Element {
public:

	Image(int x, int y);
	virtual ~Image();
	bool Update(float dt);

private:
	
	iPoint Distance;
};


#endif
