#ifndef BUTTUON_H
#define BUTTUON_H

#include "Control.h"

class Button:public Control
{
public:
	Button(string label, int psotionX, int positonY, int width, int height);

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void) ;
	virtual string getType(void);
protected:
	bool down;
	
	string label;

};
#endif 

