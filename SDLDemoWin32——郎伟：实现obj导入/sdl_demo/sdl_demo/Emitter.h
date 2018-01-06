#ifndef EMITTER_H
#define EMITTER_H

#include "GLEngine.h"
#include "Particle.h"

class Emitter
{
public:
	Emitter();
	~Emitter();

	void Update(long time);
	void setTexture(Texture *texture);

protected:
	void addParticle(void);

	float frand(float start=0, float end=1);

public:
	list <Particle *> particles;

	Texture   *texture;

	long  lastTime;
	//Atrribute
	float  emissionRate;
	float  emissionRadius;

	float life;
	float lifeRange;

	float size;
	float sizeRange;

	float saturation;
	float alpha;

	float spread;

	float gravity;

	Vector3  position;
	Vector3  wind;
	Vector3  rotation;


};

#endif
