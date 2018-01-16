#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"

class Sphere :public Primitive
{
public:
	void setSize(float Radius);
	void setDivisions(int division1,int division2);

	virtual void createPrimitive(void);

protected:
	float r;

	//γ�ȷ��򻮷ָ���
	int Nw;
	//���߷��򻮷ָ���
	int Nj;
};

#endif // !1
