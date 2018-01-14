


#include <stdlib.h>
#include "glut.h"
#include<stdio.h>
#include<math.h>
#include<Windows.h>
#include"Light.h"
#include"GLEngine.h"


float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f
bool bPersp = false;
bool bAnim = false;
bool bWire = false;
int wHeight = 0;
int wWidth = 0;
float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
float x = 0.0f, y = 1.0f, z = 8.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float angle = 0.0;
Light *mainLight = NULL;
Light *addLight = NULL;
//parameters for light 0
GLfloat position0[] = { 10,1,0,1 };
GLfloat SpecularLight[] = { 1.0,1.0,0.0,1.0 };
GLfloat SmoothLight[] = { 1.0,1.0,0.0,1.0 };
GLfloat DiffuseLight[] = { 1.0,1.0,0.0,1.0 };
GLfloat ambientLight[] = { 1.0,1.0,1.0,1.0 };
//light for the menu to add
GLfloat lightposition[10][4];
GLfloat lightdiffuse[4];
GLfloat lightspecular[4];
//number of light added
GLint bLight = 0;
//光照子菜单
GLint LightsubMenu;
//建模子菜单
GLint ModelsubMenu;
void Draw_Triangle() // This function draws a triangle with RGB colors
{

	glPushMatrix();
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();//draw a teapot

}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
	}
	else {
		glOrtho(-3, 3, -3, 3, -100, 100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 1, 0);				// the zoom view of the subject

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//材质反光性设置
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  //镜面反射参数
	GLfloat mat_shininess[] = { 50.0 };               //高光指数


	glClearColor(0.0, 0.0, 0.0, 0.0);  //背景色
	glShadeModel(GL_SMOOTH);           //多变性填充模式

	//材质属性
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(1.0, 1.0, 0.0, 1.0);
	mainLight->setPosition(0,1,0,1);
	mainLight->setAmbient(1, 1, 1, 1);
	mainLight->setSpecular(1.0, 1.0, 0.0, 1.0);
	mainLight->Visible(true);

	/*glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_SMOOTH, SpecularLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glEnable(GL_LIGHT0);*/

	//设置探照灯光照1位置及参数
	mainLight = new Light(LIGHT_SPOT);
	mainLight->setDiffuse(1.0, 1.0, 0.0, 1.0);
	mainLight->setPosition(0, 6, 0,1);
	mainLight->setSpotDirection(0, 5, 0);
	mainLight->setAmbient(1.0, 0.0, 0.0, 1.0);
	mainLight->setCutoff(180.0);

	GLfloat position1[] = { 0,6,0,1 };
	GLfloat light1specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1diffuse[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1ambition[] = { 1.0,1.0,1.0,1.0 };
	GLfloat  direction[] = { 0,5,0,1 };


	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1ambition);
	//菜单增加的探照灯
	for (int i = 2;i < bLight + 2;i++) {
		glLightfv(GL_LIGHT0 + i, GL_POSITION, lightposition[bLight]);
		glLightfv(GL_LIGHT0 + i, GL_SMOOTH, lightspecular);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lightdiffuse);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambientLight);
		glEnable(GL_LIGHT0 + i);

	}

	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate += 0.5f;
	glutSwapBuffers();
	glutPostRedisplay();
}

void LightSubMenu(GLint LightOption)
{
	switch (LightOption)
	{

	case 1: {
		//光源参数
		printf("光源位置：\n");
		scanf_s("%f %f %f %f", &lightposition[bLight][0], &lightposition[bLight][1], &lightposition[bLight][2], &lightposition[bLight][3], &lightposition[4]);

		printf("漫反射光强度：\n");
		scanf_s("%f %f %f %f", &lightdiffuse[0], &lightdiffuse[1], &lightdiffuse[2], &lightdiffuse[3], &lightdiffuse[4]);

		printf("镜面反射光强度：\n");
		scanf_s("%f %f %f %f", &lightspecular[0], &lightspecular[1], &lightspecular[2], &lightspecular[3], &lightspecular[4]);

		bLight++;

		break;
	}

	default:
		break;
	}
}
void MyMenu(GLint LightOption)
{
	if (LightOption == LightsubMenu)
		printf("The current menu is Main Menu.\n");

}
void orientMe(float ang) {

	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	glutPostRedisplay();

}

void moveMeFlat(int direction) {

	x = x + direction*(lx)*0.1;
	z = z + direction*(lz)*0.1;
	glLoadIdentity();
	glutPostRedisplay();

}
void key(unsigned char k, int x, int y)
{

	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'j': {
		angle -= 0.01f;
		orientMe(angle);
		break;
	}
	case 'l': {
		angle += 0.01f;
		orientMe(angle);
		break;
	}
	case 'i': {
		moveMeFlat(1);
		break;
	}
	case 'k': {
		moveMeFlat(-1);
		break;
	}
	case 'o': {
		glEnable(GL_LIGHT1);
		break;
	}
	case 'f': {

		glDisable(GL_LIGHT1);
		break;
	}
	case 'b': {
		if (position0[0] >= 0) {
			position0[0] -= 0.5f;
			position0[1] += 0.3f;
			SpecularLight[2] += 0.1;
			SmoothLight[2] += 0.1;
			DiffuseLight[2] += 0.1;

		}
		else if (position0[0] < 0 && position0[0]>-10) {
			position0[0] -= 0.5f;
			position0[1] -= 0.3f;
			SpecularLight[2] -= 0.1;
			SmoothLight[2] -= 0.1;
			DiffuseLight[2] -= 0.1;

		}
		else if (position0[0] == -10) {
			position0[0] = 10;
		}
		break;

	}

	case 'd': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
	}
	case 'a': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
	}
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
	}
	case 's': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
	}
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
	}
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
	}
	}

	updateView(wHeight, wWidth);
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Light");


	LightsubMenu = glutCreateMenu(LightSubMenu);
	glutAddMenuEntry("添加光源", 1);

	glutCreateMenu(MyMenu);
	glutAddSubMenu("建模", ModelsubMenu);
	glutAddSubMenu("光照", LightsubMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


