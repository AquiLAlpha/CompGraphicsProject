#include "Prism.h"

void Prism::setSize(float Radius1, float Radius2, float Height)
{
	r1 = Radius1;
	r2 = Radius2;
	height = Height;

}
void Prism::setDivisions(int div)
{
	divisions = div;
}

void Prism::createPrimitive(void)
{
	float sita = 0.0f;
	float dsita = 2.0f * 3.14f / (float)divisions;
	float cossita[512];
	float sinsita[512];
	// calculate cos sin
	for (int i = 0;i < divisions;i++)
	{
		cossita[i] = cosf(sita);
		sinsita[i] = sinf(sita);

		sita += dsita;
	}
	    cossita[divisions] = 1.0f;
	    sinsita[divisions] = 0.0f; //回到0度

	float maga = atanf((r1 - r2) / height);
	
	float cosr1;
	float sinr1;
	float cosr2;
	float sinr2;

	//generate bottom and top node
	int vbottomcenter = addVertex(0.0f, -height / 2.0f, 0.0f);
	int vtopcenter = addVertex(0.0f, height / 2.0f, 0.0f);

	int nbottom = addNormal(0.0f, -1.0f, 0.0f);
	int ntop    = addNormal(0.0f,  1.0f, 0.0f);

	int UVcenter=addTexCoord(0.5f, 0.5f); //0

	for (int i = 0;i < divisions;i++)
	{
		//generate vertices
		cosr1 =  r1*cossita[i];
		sinr1 = -r1*sinsita[i];
		int v1=addVertex(cosr1, -height / 2.0f, sinr1); //4i+1

		cosr1 = r1*cossita[i+1];
		sinr1 = -r1*sinsita[i+1];
		int v2=addVertex(cosr1, -height / 2.0f, sinr1); //4i+2

		cosr2 = r2*cossita[i];
		sinr2 = -r2*sinsita[i];
		int v3=addVertex(cosr2, height / 2.0f, sinr2); //4i+3

		cosr2 = r2*cossita[i+1];
		sinr2 = -r2*sinsita[i+1];
		int v4=addVertex(cosr2, height / 2.0f, sinr2); //4i+4

		//generate normals
		float nx1 = cosf(maga)*cossita[i];
		float ny1 = -cosf(maga)*sinsita[i];
		float normalz = sin(maga);

		float nx2 = cosf(maga)*cossita[i+1];
		float ny2 = -cosf(maga)*sinsita[i+1];


		int n1=addNormal(nx1, ny1, normalz); //i+1
		int n2 = addNormal(nx2,ny2, normalz);

		//generate uvs
		float sx = 0.5 + 0.5*cossita[i];
		float sy = 0.5 + 0.5*sinsita[i];
		int UV1=addTexCoord(sx, sy); //6i+1

		sx = 0.5 + 0.5*cossita[i+1];
		sy = 0.5 + 0.5*sinsita[i+1];
		int UV2=addTexCoord(sx, sy); //6i+2

		sx = (float) i / (float)divisions;
		int UV3=addTexCoord(sx, 0); //6i+3
		int UV4=addTexCoord(sx, 1); //6i+4

		sx = (float)(i+1) / (float)divisions;
		int UV5=addTexCoord(sx, 0); //8i+5
		int UV6=addTexCoord(sx, 1); //6i+6


		//generate face
		//侧面
		addFace(v1, v2, v3, n1, n2, n1, UV3, UV5, UV4);
		addFace(v2, v4, v3, n2, n2, n1, UV5, UV6, UV4);
		//底面
		addFace(vbottomcenter, v1, v2, nbottom, nbottom, nbottom, UVcenter, UV1, UV2);
		//上面
		addFace(vtopcenter,    v3, v4,    ntop,    ntop,    ntop, UVcenter, UV1, UV2);
	}
}