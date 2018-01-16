#include "Slider.h"

const int tickSize = 5;

Slider::Slider(string lbl, float in_min, float in_max, int positionX, int positionY, int width, int height):
			Control(positionX, positionY, width, height)
{
	defaultValue = 0.0f;

	current = NULL;

	max = in_max;
	min = in_min;

	label = lbl;

	dragging = false;
}

void Slider::setValue(float *value)
{
	current = value;

	if (current != NULL)
		defaultValue = *current;
}

 bool Slider::updateControl(MouseState &state)
{
	 Control::updateControl(state);

	 int x = state.x;
	 int y = state.y;

	 if (inside == true)
	 {
		 if (state.LeftButtonDown)
		 {
			 dragging = true;
		 }
	    if 	 (state.RightButtonDown)
		 {
			 *current=defaultValue;
		 }
		if ((!(state.LeftButtonDown)))
			dragging = false;

		if (dragging == true)
		{
			*current = (float)(x - posX) / (float)width*(max - min) + min;

			if (*current > max)
				*current = max;
			else if (*current < min)
				*current = min;
		}
	 }

	 return dragging;
}

 void Slider::drawControl(void)
{
	 glEnable(GL_BLEND);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 glDisable(GL_TEXTURE_2D);

	 glColor4f(0.7f, 0.7f, 0.7f, 0.8f);
	 glBegin(GL_QUADS);
	 glVertex2d(posX + width, posY + height);
	 glVertex2d(posX, posY + height);
	 glVertex2d(posX, posY);
	 glVertex2d(posX + width, posY);
	 glEnd();

	 if (inside == true)
	 {
		 glColor4f(0.2f, 0.2f, 0.7f, 0.8f);
		 glLineWidth(2.0f);
	 }
	 else
	 {
		 glColor4f(0.2f, 0.2f, 0.7f, 0.5f);
		 glLineWidth(1.0f);
	 }

	 glBegin(GL_LINE_STRIP);
	 glVertex2d(posX + width, posY + height);
	 glVertex2d(posX, posY + height);
	 glVertex2d(posX, posY);
	 glVertex2d(posX + width, posY);
	 glVertex2d(posX + width, posY + height);
	 glEnd();

	 int currentX = (int)(*current - min) / (max - min)*(width - tickSize);
	
     glColor4f(0.3f, 0.3f, 1.0f, 0.5f);
	 glBegin(GL_QUADS);
	 glVertex2d(currentX + tickSize, posY + height);
	 glVertex2d(currentX, posY + height);
	 glVertex2d(currentX, posY);
	 glVertex2d(currentX + tickSize, posY);
	 glEnd();

	 glColor4f(0.7f, 0.7f, 0.7f, 1.0f);

	 char newlabel[256];
	 strcpy_s(newlabel, label.data());
	 strcat_s(newlabel, " %0.1f");

	 iGLEngine->drawText(posX + 2, posY + 2, newlabel, *current);
	 //iGLEngine->drawText(posX + 2, posY + 2, label.data(),*current);
}

 string Slider::getType(void)
{
	 return "slider";
}
