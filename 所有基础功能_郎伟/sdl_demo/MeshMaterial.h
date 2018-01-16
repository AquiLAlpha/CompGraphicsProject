#ifndef MESHMATERIAL_H
#define MESHMATERIAL_H

#include "Node.h"
#include "Texture.h"

class MeshMaterial :public Node
{
public:
	MeshMaterial();
	virtual ~MeshMaterial();

	void setKa(float r, float g, float b, float a);
	void setKd(float r, float g, float b, float a);
	void setKs(float r, float g, float b, float a);
	void setKe(float r, float g, float b, float a);

	void setShininess(float shininess);

	void setDiffuseMap(Texture *texture);

public:
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];

	float Shininess;

	Texture *diffuseMap;
};

#endif
