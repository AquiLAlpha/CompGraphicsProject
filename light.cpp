

#include <stdlib.h>
#include "glut.h"
#include<stdio.h>



GLfloat position0[] = { 10,1,0,1 };
GLfloat SpecularLight[] = { 1.0,1.0,0.0,1.0 };
GLfloat DiffuseLight[] = { 1.0,1.0,0.0,1.0 };
GLfloat ambientLight[] = { 1.0,1.0,1.0,1.0 };



void light()
{


	//���ʷ���������
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //���淴�����
	GLfloat mat_shininess[] = { 50.0 };               //�߹�ָ��
	

	glClearColor(0.0, 0.0, 0.0, 0.0);  //����ɫ
	glShadeModel(GL_SMOOTH);           //��������ģʽ

									   //��������
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	//����̽�յƹ���1λ�ü�����
	GLfloat position1[] = { 0,4,0,1 };
	GLfloat light1s[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1d[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1a[] = { 1.0,1.0,1.0,1.0};
	GLfloat  direction[] = { 0,-60,0,1 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1s);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1d);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1a);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);


	glutSwapBuffers();
}

void key(unsigned char k, int x, int y)
{
	
	switch (k)
	{
	case 27:
	
	case 'b': {
		if (position0[0] >= 0) {
			position0[0] -= 0.5f;
			position0[1] += 0.3f;
			//SpecularLight[0] -= 0.02;
			SpecularLight[2] += 0.1;
			//DiffuseLight[0] -= 0.02;
			DiffuseLight[2] += 0.1;

		}
		else if (position0[0] < 0&&position0[0]>-10) {
			position0[0] -= 0.5f;
			position0[1] -= 0.3f;
			//SpecularLight[0] += 0.02;
			SpecularLight[2] -= 0.1;
			//DiffuseLight[0] += 0.02;
			DiffuseLight[2] -= 0.1;

		}
		else if (position0[0] == -10) {
			position0[0] = 10;
		}
		break;

	}


	}

	updateView(wHeight, wWidth);
}



int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Ex6");

	glutDisplayFunc(light);
	
	glutMainLoop();
	return 0;
}


