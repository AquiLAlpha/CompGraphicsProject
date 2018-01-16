#ifndef PARTICLE_H
#define PARTICLE_H

#include "GLEngine.h"
#include <list>
#include "Vector3.h"

using std::list;


class Particle
{
public:
	Particle(int id);

	void Update(long time);
public:
	long lastTime;

	int id;

	Vector3  color;

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 rotation;

	float totalLife;;
	float life;

	float alpha;
	float size;

	float bounciness;

	bool active;
};


#endif // !PARTICLE_H

