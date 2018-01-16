#include "MeshMaterial.h"


MeshMaterial::MeshMaterial():Node()
{
	float defaultAmbient[4] = {  0.0f,0.0f,0.0f,0.5f };
	float defaultDiffuse[4] = {  1.0f,1.0f,1.0f,0.5f };
	float defaultSpecular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float defaultEmission[4] = { 0.0f,0.0f,0.0f,1.0f };
	
	memcpy(Ka, defaultAmbient, sizeof(float) * 4);
	memcpy(Kd, defaultDiffuse, sizeof(float) * 4);
	memcpy(Ks, defaultSpecular, sizeof(float) * 4);
	memcpy(Ke, defaultEmission, sizeof(float) * 4);

	Shininess = 00.0f;

	diffuseMap = NULL;
}

MeshMaterial::~MeshMaterial()
{

}

void MeshMaterial::setKa(float r, float g, float b, float a)
{
	Ka[0] = r;
	Ka[1] = g;
	Ka[2] = b;
	Ka[3] = a;
}

void MeshMaterial::setKd(float r, float g, float b, float a)
{
	Kd[0] = r;
	Kd[1] = g;
	Kd[2] = b;
	Kd[3] = a;
}

void MeshMaterial::setKs(float r, float g, float b, float a)
{
	Ks[0] = r;
	Ks[1] = g;
	Ks[2] = b;
	Ks[3] = a;
}

void MeshMaterial::setKe(float r, float g, float b, float a)
{
	Ke[0] = r;
	Ke[1] = g;
	Ke[2] = b;
	Ke[3] = a;
}

void MeshMaterial::setShininess(float shininess)
{
	Shininess = shininess;
}

void MeshMaterial::setDiffuseMap(Texture *texture)
{
	diffuseMap = texture;
}


