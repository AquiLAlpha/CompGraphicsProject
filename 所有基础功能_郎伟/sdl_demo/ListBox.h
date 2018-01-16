#ifndef LISTBOX_H
#define LISTBOX_H

#include "Control.h"

class ListBox:public Control
{
public:
	ListBox(int positionX, int positionY, int width, int height);

	void addItem(string item);
	void removeItem(int index);
	void setCurrent(int index);

	int getIndex(void);
	int getCount(void);

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void) ;
	virtual string getType(void) ;

protected:
	int index;
	vector<string> items;
};
#endif // !LISTBOX_H

