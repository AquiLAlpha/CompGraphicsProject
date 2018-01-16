#include "Mesh.h"


Mesh::Mesh():Node()
{

}
 Mesh::~Mesh()
{

}

 bool Mesh::drawMesh(bool drawChildren , Mesh *drawSelected)
 {
	 bool drawn = false;
	 glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();

	 glTranslatef(transform.translate.x, transform.translate.y, transform.translate.z);
	 glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
	 glRotatef(transform.rotation.x, 1, 0, 0);
	 glRotatef(transform.rotation.y, 0,1, 0);
	 glRotatef(transform.rotation.z, 0, 0, 1);


	 //调试 1.要在这里设置材质； 2.要在这里加bind哪个纹理
	 //材质
	 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)material.Ka);
;	 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)material.Kd);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)material.Ks);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)material.Ke);
	 glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat)  material.Shininess);

	 if (material.diffuseMap != NULL)
		 glBindTexture(GL_TEXTURE_2D, material.diffuseMap->texID);
	 else glDisable(GL_TEXTURE_2D);
	 
	 if (drawSelected == NULL || drawSelected == this)
	 {
		 glPushName((GLuint)this);

		 for (int i = 0;i < (int)faces.size();i++)
		 {
			
			 drawFace(faces[i]);
		 }

		 glPopName();

		 drawn = true;
	 }

	 if (drawChildren == true)
	 {
		 for (list<Node*>::iterator it = children.begin();it != children.end();it++)
		 {
			 Mesh *mesh = dynamic_cast<Mesh *>(*it);

			 if (mesh != NULL)
				 drawn=mesh->drawMesh(true, drawSelected)|drawn;

		 }
	 }
	 glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();

	 return drawn;
}

 Transform &Mesh::getTransform(void)
 {
	 return transform;
}

 //调试
 MeshMaterial &Mesh::getMeshMaterial(void)
 {
	 return material;
 }

 //调试
 void Mesh::exportMesh(FILE *ptr,float *Parentmodelview,int *prenumv,int *prenumvt,int *prenumvn)
 {
	 //生成局部变换矩阵
	 float templocalmodelview[16];

	 glPushMatrix();
	 glLoadIdentity();

	 glTranslatef(transform.translate.x, transform.translate.y, transform.translate.z);
	 glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
	 glRotatef(transform.rotation.x, 1, 0, 0);
	 glRotatef(transform.rotation.y, 0, 1, 0);
	 glRotatef(transform.rotation.z, 0, 0, 1);

	 glGetFloatv(GL_MODELVIEW_MATRIX, templocalmodelview);
	 glPopMatrix();

	 float localmodelview[16];
	 for (int i = 0;i < 4;i++)
		 for (int j = 0;j < 4;j++)
			 localmodelview[4 * i + j] = templocalmodelview[4 * j + i];

	 //左乘 父变换矩阵
	 float modelview[16];

	 for (int i = 0;i < 4;i++)
	 {
		 for (int j = 0;j < 4;j++)
			 //计算modelview[4*i+j]
		 {
			 float temp = 0.0f;
			 for (int k = 0;k < 4;k++)
				 temp = temp + Parentmodelview[4 * i + k] * localmodelview[4 * k + j];
			 modelview[4 * i + j] = temp;
		 }
		
	 }

	 /*float currentmodelview[16];
	 float modelview[16];

	 glMatrixMode(GL_MODELVIEW);
	 glGetFloatv(GL_MODELVIEW_MATRIX, currentmodelview);

	 glPushMatrix();
	 glLoadMatrixf(currentmodelview);

	 glTranslatef(transform.translate.x, transform.translate.y, transform.translate.z);
	 glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
	 glRotatef(transform.rotation.x, 1, 0, 0);
	 glRotatef(transform.rotation.y, 0, 1, 0);
	 glRotatef(transform.rotation.z, 0, 0, 1);

	 glPushMatrix();
	 glLoadMatrixf(currentmodelview);
	 glTranslatef(transform.translate.x, transform.translate.y, transform.translate.z);
	 glScalef(transform.scale.x, transform.scale.y, transform.scale.z);
	 glRotatef(transform.rotation.x, 1, 0, 0);
	 glRotatef(transform.rotation.y, 0, 1, 0);
	 glRotatef(transform.rotation.z, 0, 0, 1);

	 glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	 glPopMatrix();*/

	 //左乘顶点 法向量  计数
	 float v[4];
	 float vn[4];
	 int numv =  *prenumv;
	 int numvt = *prenumvt;
	 int numvn = *prenumvn;

	 //Mesh中的faces
	 for (int i = 0;i < (int)faces.size();i++)
	 {
		 //处理一个面中的三个顶点
		 for (int j = 0;j < 3;j++)
		 {
			 v[0] = faces[i]->vertices[j]->x;
			 v[1] = faces[i]->vertices[j]->y;
			 v[2]= faces[i]->vertices[j]->z;
			 v[3] = 1.0f;
			


			 vn[0] = faces[i]->normals[j]->x;
			 vn[1] = faces[i]->normals[j]->y;
			 vn[2] = faces[i]->normals[j]->z;
			 vn[3] = 1.0f;


			 float tempv[4];
			 float tempvn[4];

			 for (int k = 0;k < 4;k++)
			 {
				 tempv[k] = 0;
				 tempvn[k] = 0;
			 }

			 for (int k = 0;k < 4;k++)
			 {
				for (int m = 0;m < 4;m++)
				{
					//顶点左乘开始了
					tempv[k] = tempv[k]+modelview[4 * k + m] * v[m];
					//法向量左乘开始了
					tempvn[k] = tempvn[k]+modelview[4 * k + m] * vn[m];
				}
			 }

			 for (int k = 0;k < 3;k++)
			 {
				 tempv[k] = tempv[k] / tempv[3];

				 tempvn[k] = tempvn[k]  / tempvn[3];
			 }

			 //输出 顶点 纹理坐标 法向量 
			 fprintf_s(ptr, "v %f %f %f\n", tempv[0], tempv[1], tempv[2]);
			 fprintf_s(ptr, "vt %f %f %f\n", faces[i]->UVs[j]->x, faces[i]->UVs[j]->y, faces[i]->UVs[j]->z);
			 fprintf_s(ptr, "vn %f %f %f\n", tempvn[0], tempvn[1], tempvn[2]);

		 }

		 //输出面
		 fprintf_s(ptr, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", numv+1, numvt+1, numvn + 1, numv + 2, numvt + 2, numvn + 2, numv + 3, numvt + 3, numvn + 3);
		 numv += 3;
		 numvt += 3;
		 numvn += 3;
				
	 }
	 *prenumv = numv;
	 *prenumvt = numvt;
	 *prenumvn = numvn;

	 for (list<Node*>::iterator it = children.begin();it != children.end();it++)
	 {
		 Mesh *mesh = dynamic_cast<Mesh *>(*it);

		 if (mesh != NULL)
			 mesh->exportMesh(ptr, modelview, prenumv, prenumvt, prenumvn);

	 }

	 glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();

 }



 void Mesh::drawFace(MeshFace *face)
 {
	 glBegin( GL_TRIANGLES);

	 
	 for (int i = 0;i < 3;i++)
	 {
		 glTexCoord2f(face->UVs[i]->x, face->UVs[i]->y);
		 glNormal3f(face->normals[i]->x, face->normals[i]->y, face->normals[i] ->z);
		 glVertex3f(face->vertices[i]->x, face->vertices[i]->y, face->vertices[i]-> z);
	 }

	glEnd();
}

 int Mesh::addVertex(float x, float y, float z)
 {
	 vertices.push_back(new Vector3(x, y, z));

	 return ((int)vertices.size() - 1);

}

 int Mesh::addNormal(float x, float y, float z)
 {
	 normals.push_back(new Vector3(x, y, z));

	 return ((int)normals.size() - 1);
}
 int Mesh::addTexCoord(float s, float t)
 {
	 UVs.push_back(new Vector3(s,t, 0));

	 return ((int)UVs.size() - 1);
}

void Mesh::addFace(int v1, int v2, int v3,
					int n1, int n2, int n3,
					int uv1, int uv2, int uv3)
{
	faces.push_back(new MeshFace(vertices[v1], vertices[v2], vertices[v3],
							normals[n1], normals[n2], normals[n3],
							UVs[uv1], UVs[uv2], UVs[uv3]));

}

