#include "Sphere.h"


void Sphere::createPrimitive(void)
{
	float cosfai[512];
	float sinfai[512];
	float dfai = 0.5f*3.14f / (float)Nw;
	
	float cossita[512];
	float sinsita[512];
	float dsita = 2.0f*3.14f / (float) Nj;

	//sita : cos sin
	float sita = 0.0f;
	for (int i = 0;i < Nj;i++)
	{
		cossita[i] = cosf(sita);
		sinsita[i] = sinf(sita);

		sita += dsita;
	}
	cossita[Nj] = 1.0f;
	sinsita[Nj] = 0.0f;

	//fai : cos sin
	float fai = 0.0f;
	for (int i = 0;i < Nw;i++)
	{
		cosfai[i] = cosf(fai);
		sinfai[i] = sinf(fai);

		fai += dfai;
	}
	cosfai[Nw] = 0.0f;
	sinfai[Nw] = 1.0f;

	float r1;//ÏÂÈ¦
	float r2;//ÉÏÈ¦
	float h1;
	float h2;

	for (int i = 0;i < Nw;i++)
	{
		r1 = r*cosfai[i];
		r2 = r*cosfai[i + 1];

		h1 = r*sinfai[i];
		h2 = r*sinfai[i + 1];

		float x1;
		float z1;
		float x2;
		float z2;
		float x3;
		float z3;
		float x4;
		float z4;
		float nx1;
		float nx2;
		float ny1;
		float ny2;
		float nz1;
		float nz2;
		float s1;
		float t1;
		float s2;
		float t2;
		float tpiece = 0.5f / (float)Nw;
		float spiece = 1.0f / (float)Nj;
		for (int j = 0;j < Nj;j++)
		{
			//vertex
			x1 = r1*cossita[j];
			z1 = -r1*sinsita[j];

			x2 = r1*cossita[j + 1];
			z2 = -r1*sinsita[j + 1];

			x3 = r2*cossita[j];
			z3 = -r2*sinsita[j];

			x4 = r2*cossita[j + 1];
			z4 = -r2*sinsita[j + 1];

			int v1 = addVertex(x1, h1, z1);
			int v2 = addVertex(x2, h1, z2);
			int v3 = addVertex(x3, h2, z3);
			int v4 = addVertex(x4, h2, z4);

			int v5 = addVertex(x1, -h1, z1);
			int v6 = addVertex(x2, -h1, z2);
			int v7 = addVertex(x3, -h2, z3);
			int v8 = addVertex(x4, -h2, z4);
			//normal
			nx1 = cosfai[i]*cossita[j];
			nz1 = -cosfai[i] * sinsita[j];
			ny1 = sinfai[i];

			nx2 = cosfai[i] * cossita[j+1];
			nz2= -cosfai[i] * sinsita[j+1];
			ny2 = sinfai[i];

			int normal1 = addNormal(nx1, ny1, nz1);
			int normal2 = addNormal(nx2, ny2, nz2);
			int normal5 = addNormal(nx1, -ny1, nz1);
			int normal6 = addNormal(nx2, -ny2, nz2);
				
			nx1 = cosfai[i+1] * cossita[j];
			nz1 = -cosfai[i+1] * sinsita[j];
			ny1 = sinfai[i+1];

			nx2 = cosfai[i+1] * cossita[j + 1];
			nz2 = -cosfai[i+1] * sinsita[j + 1];
			ny2 = sinfai[i+1];

			int normal3 = addNormal(nx1, ny1, nz1);
			int normal4 = addNormal(nx2, ny2, nz2);
			int normal7 = addNormal(nx1, -ny1, nz1);
			int normal8 = addNormal(nx2, -ny2, nz2);

			//texture
			s1 = (float) j*spiece;
			t1 = 0.5f + (float)i*tpiece;
			
			s2 = (float)(j + 1)*spiece;
			t2 = 0.5f +(float)(i+1)*tpiece;

			int UV1 = addTexCoord(s1, t1);
			int UV2 = addTexCoord(s2, t1);
			int UV3 = addTexCoord(s1, t2);
			int UV4 = addTexCoord(s2, t2);

			t1 = 0.5f - (float)i*tpiece;
			t2 = 0.5f - (float)(i + 1)*tpiece;
			int UV5 = addTexCoord(s1, t1);
			int UV6 = addTexCoord(s2, t1);
			int UV7 = addTexCoord(s1, t2);
			int UV8 = addTexCoord(s2, t2);

			//face
			/*addFace(v1, v2, v3, normal1, normal2, normal1, UV1, UV2, UV3);
			addFace(v2, v4, v3, normal2, normal2, normal1, UV2,UV4,UV3);
			addFace(v5, v6, v7, normal3, normal4, normal3, UV5, UV6, UV7);
			addFace(v6, v7, v8, normal4, normal3, normal4, UV6, UV7, UV8);*/

			addFace(v1, v2, v3, normal1, normal2, normal3, UV1, UV2, UV3);
			addFace(v2, v4, v3, normal2, normal4, normal3, UV2, UV4, UV3);
			addFace(v5, v6, v7, normal5, normal6, normal7, UV5, UV6, UV7);
			addFace(v6, v7, v8, normal6, normal7, normal8, UV6, UV7, UV8);
		}
	}

}

void Sphere::setSize(float Radius)
{
	r = Radius;
}
void Sphere::setDivisions(int division1, int division2)
{
	Nj = division1;
	Nw = division2;
}