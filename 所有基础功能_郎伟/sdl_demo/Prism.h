#ifndef PRISM_H
#define PRISM_H

#include "Primitive.h"

class Prism :public Primitive
{
public:
	void setSize(float Radius1, float Radius2,float Height);
	void setDivisions(int division);

	virtual void createPrimitive(void);

protected:
	float r1;
	float r2;
	float height;

	int divisions;
};

#endif // !PRISM_H

