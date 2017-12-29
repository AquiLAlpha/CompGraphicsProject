// main.cpp : �������̨Ӧ�ó������ڵ㡣

#define BITMAP_ID 0x4D42

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "glut.h"
#include "math.h"

//���ƻ���
GLint bSphere = 0;
//�������������
GLint bCube = 0;
//����Բ������
GLint bCylinder = 0;
//����Բ׶����
GLint bCone = 0;
//������������
GLint bPrism = 0;
GLint EdgePrism[10];
//������̨����
GLint bPrismTable = 0;
GLint EdgePrismTable[10];

float PI = 3.14;
//�����ʶ��
unsigned int texture[4];

//���ó�ʼ����ģ����ת�Ƕ�Ϊ0
float fRotate = 0.0f;
//����ͶӰ��ʽΪ��ͶӰ����͸��ͶӰ�Ĳ�����������ʼΪ�٣�����ͶӰ
bool bPersp = false;
//��������ģ����ת��񣬳�ʼΪ�٣�������ת
bool bAnim = false;
//������Ⱦ����ʾģʽΪ�߶λ�����
bool bWire = false;

//��¼��ǰ���ڴ�С�ĸ߶ȺͿ��
int wHeight = 0;
int wWidth = 0;

//�۲�������������
float eye[] = { 0, 0, 8 };		 //���������������ϵλ��
float center[] = { 0, 0, 0 };    //�۲���������

//�����ؿ��Ʊ���
float teapos[3] = { 0,0,4 + 1 }; //����������ڳ�ʼģ��λ��
float tearotate = 0.0f;          //�����������Y����ת�ĽǶ�
bool tRotate = false;			 //���Ʋ��������ת��񣬳�ʼ����Ϊ�񣬼�����ת

//���Ƚ�ģ�������Ϊ1��1��3
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
FILE *filePtr;	// �ļ�ָ��
BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
unsigned char	*bitmapImage;		// bitmapͼ������
int	imageIdx = 0;		// ͼ��λ������
unsigned char	tempRGB;	// ��������

// �ԡ�������+����ģʽ���ļ�filename
filePtr = fopen(filename, "rb");
if (filePtr == NULL) {
printf("file not open\n");
return NULL;
}
// ����bitmap�ļ�ͼ
fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
// ��֤�Ƿ�Ϊbitmap�ļ�
if (bitmapFileHeader.bfType != BITMAP_ID) {
fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
return NULL;
}

// ����bitmap��Ϣͷ
fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
// ���ļ�ָ������bitmap����
fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
// Ϊװ��ͼ�����ݴ����㹻���ڴ�
bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
// ��֤�ڴ��Ƿ񴴽��ɹ�
if (!bitmapImage) {
fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
return NULL;
}

// ����bitmapͼ������
fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
// ȷ�϶���ɹ�
if (bitmapImage == NULL) {
fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
return NULL;
}

//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
for (imageIdx = 0;
imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
tempRGB = bitmapImage[imageIdx];
bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
bitmapImage[imageIdx + 2] = tempRGB;
}


// �ر�bitmapͼ���ļ�
fclose(filePtr);
return bitmapImage;
}
//����������
BITMAPINFOHEADER TempbitmapInfoHeader;
unsigned char*   TempbitmapData;
void texload(int i, char *filename)
{

BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
unsigned char*   bitmapData;                                       // ��������

bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
//�����µ�һ��crack����Ϣͷ��ͼƬRGB��Ϣ��ַ
if (i == 1) {
TempbitmapInfoHeader = bitmapInfoHeader;
TempbitmapData = bitmapData;
};
//ʵ��crack��spot��ͼƬ��Ϣ��Ȩƽ��
if (i == 3)
{
for (int imageIdx = 0;
imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 1)
{
bitmapData[imageIdx] = 0.7*bitmapData[imageIdx] + 0.3*TempbitmapData[imageIdx];
}

}

glBindTexture(GL_TEXTURE_2D, texture[i]);
// ָ����ǰ����ķŴ�/��С���˷�ʽ
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

glTexImage2D(GL_TEXTURE_2D,
0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�)
GL_RGB,	//����ϣ���������к졢�̡�������
bitmapInfoHeader.biWidth, //��������������n�����б߿�+2
bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2
0, //�߿�(0=�ޱ߿�, 1=�б߿�)
GL_RGB,	//bitmap���ݵĸ�ʽ
GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
bitmapData);	//bitmap����ָ��
}

void Draw_Leg()
{
glScalef(1, 1, 3);
glutSolidCube(1.0);
}

void Draw_Scene()
{
//�����ģ
glPushMatrix();
glTranslatef(teapos[0], teapos[1], teapos[2]);//�����ƽ����teapos����
glRotatef(90, 1, 0, 0);                     //�����X�ᷭת
glRotatef(tearotate, 0, 1, 0);			    //��������Ƶ�ǰ����ϵ��Y����ת����ȥ������һ�д����е���X�ᷭת
glutSolidTeapot(1);							//���Ƶ�λ���ģ��
glPopMatrix();
//���潨ģ�������Ϊ5��4��1
glPushMatrix();
glTranslatef(0, 0, 3.5);
glScalef(5, 4, 1);
glutSolidCube(1.0);
glPopMatrix();
//�ĸ����Ƚ�ģ��ƽ�����ĸ�����λ��
glPushMatrix();
glTranslatef(1.5, 1, 1.5);
Draw_Leg();
glPopMatrix();

glPushMatrix();
glTranslatef(-1.5, 1, 1.5);
Draw_Leg();
glPopMatrix();

glPushMatrix();
glTranslatef(1.5, -1, 1.5);
Draw_Leg();
glPopMatrix();

glPushMatrix();
glTranslatef(-1.5, -1, 1.5);
Draw_Leg();
glPopMatrix();

}

void updateView(int width, int height)
{
glViewport(0, 0, width, height);						// �����ӿڴ�С�ʹ��ڴ�С��ͬ

glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
glLoadIdentity();									// ������Ϊ��λ����
//���㴰�ڿ�߱�
float whRatio = (GLfloat)width / (GLfloat)height;

//ѡ��ͶӰ��ʽΪ͸�Ӿ�������ͶӰ
if (bPersp) {
gluPerspective(45, whRatio, 0.1f, 100.0f);			//����͸��ͶӰ������Ұ�Ƕ�Ϊ45�ȣ��Ӿ����߱��Լ�ǰ��ü���λ��
}
else
glOrtho(-3, 3, -3, 3, -100, 100);					//������ͶӰ����ͶӰ�淶Χ�Լ�ǰ�������λ��

//ѡ��ǰ����ģʽΪģ����ͼ
glMatrixMode(GL_MODELVIEW);
}
//
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
//ϵͳ��Ͼʱ�����ú�����ʾ������ʵ�ִ������ݸı��˵�ͼ�λ���
void idle()
{
glutPostRedisplay();
}


//todo; hint: you may need another ARRAY when you operate the teapot

void key(unsigned char k, int x, int y)
{
switch (k)
{
//����esc����q�����˳�����
case 27:
case 'q': {exit(0); break; }
//����p��ͨ���ı�bPersp���ֵ���л���ͶӰ����͸��ͶӰ
case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; }
//����ո����ı�bAnim���ֵ������ģ���Ƿ�Y����ת
case ' ': {bAnim = !bAnim; break;}
//����o���ı�bWire������л��߿�ģ�ͻ���ʵ��ģ�͵���ȾЧ��
case 'o': {bWire = !bWire; break;}

//����aʹ�ù۲쵽��ģ�������ƶ���ͨ��ʹ�����λ�ú͹۲�����������ʵ��
case 'a': {
eye[0] += 0.25f;   //ʹ��ÿ�����x��������0.25f
center[0] += 0.25f;//ʹ��ÿ�ι۲�����x��������0.25f
break;
}
//����dʹ�ù۲쵽��ģ�������ƶ���ͨ��ʹ�����λ�ú͹۲�����������ʵ��
case 'd': {//todo
eye[0] -= 0.25f;   //ʹ��ÿ�����x�����С0.25f
center[0] -= 0.25f;//ʹ��ÿ�ι۲�����x�����С0.25f
break;
}
//����wʹ�ù۲쵽��ģ�������ƶ���ͨ��ʹ�����λ�ú͹۲�����������ʵ��
case 'w': {//todo
eye[1] -= 0.25f;   //ʹ��ÿ�����y�����С0.25f
center[1] -= 0.25f;//ʹ��ÿ�ι۲�����x�����С0.25f
break;
}
//����sʹ�ù۲쵽��ģ�������ƶ���ͨ��ʹ�����λ�ú͹۲�����������ʵ��
case 's': {//todo
eye[1] += 0.25f;   //ʹ��ÿ�����y��������0.25f
center[1] += 0.25f;//ʹ��ÿ�ι۲�����y��������0.25f
break;
}
//����zʹ�ù۲쵽��ģ����ǰ������ͨ��ʹ�����λ�ú͹۲��������ʵ��
case 'z': {//todo
eye[2] -= 0.25f;   //ʹ��ÿ�����z�����С0.25f
center[2] -= 0.25f;//ʹ��ÿ�ι۲�����x�����С0.25f
break;
}
//����cʹ�ù۲쵽��ģ�����Զ�룬ͨ��ʹ�����λ�ú͹۲�������ǰʵ��
case 'c': {//todo
eye[2] += 0.25f;   //ʹ��ÿ�����z��������0.25f
center[2] += 0.25f;//ʹ��ÿ�ι۲�����z��������0.25f
break;
}

//�����ز�����kijl�ֱ���Ʋ��ǰ�������ƶ���eѡ�����Ƿ������ת��
case 'l': {//todo, hint:use the ARRAY that you defined, and notice the teapot can NOT be moved out the range of the table.
if (teapos[0]<2.5f) teapos[0] += 0.25f; //ʹ���ڲ����ģʱ����������ƶ�
break;
}
case 'j': {//todo
if (teapos[0]>-2.5f) teapos[0] -= 0.25f;//ʹ���ڲ����ģʱ����������ƶ�
break;
}
case 'i': {//todo
if (teapos[1] < 2.0f) teapos[1] += 0.2f;//ʹ���ڲ����ģʱ����������ƶ�������������ϵ�еõ���Z�������˶�
break;
}
case 'k': {//todo
if (teapos[1] > -2.0f) teapos[1] -= 0.2f;//ʹ���ڲ����ģʱ����������ƶ�������������ϵ�еõ���Z�������˶�
break;
}
case 'e': {//todo
tRotate = !tRotate;//�ı�tRotate���ֵ��ѡ�����Ƿ���ת
break;
}
}
}
//�����ĵ�����
GLfloat SpherePos[10][3];
GLfloat SphereRadius[10];
//����������ʹ�С
GLfloat CubePos[10][3];
GLfloat CubeSca[10][3];
//Բ��������ʹ�С
GLfloat CylinderPos[10][3];
GLfloat CylinderRadius[10], CylinderHeight[10];
//Բ׶����ʹ�С
GLfloat ConePos[10][3];
GLfloat ConeRadius[10], ConeHeight[10];
//��������ʹ�С
GLfloat PrismPos[10][3];
GLfloat PrismRadius[10], PrismHeight[10];
//��̨����ʹ�С
GLfloat PrismTablePos[10][3];
GLfloat PrismTableRadius[10][2], PrismTableHeight[10];

void ModelCube()
{
int i, j;
const GLfloat x1 = -0.5, x2 = 0.5;
const GLfloat y1 = -0.5, y2 = 0.5;
const GLfloat z1 = -0.5, z2 = 0.5;

GLfloat v[6][4][3] = {
{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },//�µ���
{ { x1,y2,z2 },{ x2,y2,z2 },{ x2,y2,z1 },{ x1,y2,z1 } },//�ϵ���
{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } },//ǰ��
{ { x2,y1,z1 },{ x1,y1,z1 },{ x1,y2,z1 },{ x2,y2,z1 } },//����
{ { x2,y1,z2 },{ x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 } },//����
{ { x1,y1,z1 },{ x1,y1,z2 },{ x1,y2,z2 },{ x2,y2,z1 } }//����
};
GLfloat texcoord[4][2] = { { 0.0f,0.0f },{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

//��ͼ������������
glBegin(GL_QUADS);
for (i = 0;i<6;i++)
for (j = 0;j < 4;j++)
{
//������������
glTexCoord2fv(texcoord[j]);
glVertex3fv(v[i][j]);

}
glEnd();

}

void ModelCylinder(float R,float H)
{
	float nslice = 100.0;
	//Բ��

	glPushMatrix();

	glBegin(GL_QUAD_STRIP);
	double t = 0.0;double dt = 2 * PI / nslice;
	for (int j = 0; j <= nslice; ++j)
	{
		glTexCoord2f(t / (2 * PI), 1.0); glVertex3f(R*cos(t), H, -R*sin(t));
		glTexCoord2f(t / (2 * PI), 0.0); glVertex3f(R*cos(t), 0.0, -R*sin(t));
		t = t + dt;
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0, 0.0, 0.0);
	t = 0.0; dt = 2 * PI / nslice;
	for (int i = 0; i < nslice; i++)
	{
		glTexCoord2f(0.5 + 0.5*cos(t), 0.5 + 0.5*sin(t));glVertex3f(R*cos(t), 0.0, -R*sin(t));
		t = t + dt;
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5);glVertex3f(0.0, H, 0.0);
	t = 0.0; dt = 2 * PI / nslice;
	for (int i = 0; i <= nslice; i++)
	{
		glTexCoord2f(0.5 + 0.5*cos(t), 0.5 + 0.5*sin(t));glVertex3f(R*cos(t), H, -R*sin(t));
		t = t + dt;
	}
	glEnd();

	glPopMatrix();
}
void ModelCone(float R,float H)
{
	float nslice = 100.0;
	//Բ׶
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	double tt = 0.0;double dtt = 2 * PI / nslice;
	for (int j = 0; j <= nslice; ++j)
	{
		glTexCoord2f(tt / (2 * PI), 1.0);
		glVertex3f(0.0,H, 0.0);
		glTexCoord2f(tt / (2 * PI), 0.0);
		glVertex3f(R*cos(tt), 0.0, -R*sin(tt));
		tt = tt + dtt;
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0, 0.0, 0.0);
	tt = 0.0; dtt = 2 * PI / nslice;
	for (int i = 0; i < nslice; i++)
	{
		glTexCoord2f(0.5 + 0.5*cos(tt), 0.5 + 0.5*sin(tt));glVertex3f(R*cos(tt), 0.0, -R*sin(tt));
		tt = tt + dtt;
	}
	glEnd();

	glPopMatrix();
}
//������������̨����ʵҲ���Ժ�Բ׶��Բ���ϲ���
void ModelPrism(float R1,float R2, float H,float nslice)
{

	glPushMatrix();

	glBegin(GL_QUAD_STRIP);
	double t = 0.0;double dt = 2 * PI / nslice;
	for (int j = 0; j <= nslice; ++j)
	{
		glTexCoord2f(t / (2 * PI), 1.0); glVertex3f(R1*cos(t), H, -R1*sin(t));
		glTexCoord2f(t / (2 * PI), 0.0); glVertex3f(R2*cos(t), 0.0, -R2*sin(t));
		t = t + dt;
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0, 0.0, 0.0);
	t = 0.0; dt = 2 * PI / nslice;
	for (int i = 0; i < nslice; i++)
	{
		glTexCoord2f(0.5 + 0.5*cos(t), 0.5 + 0.5*sin(t));glVertex3f(R1*cos(t), 0.0, -R1*sin(t));
		t = t + dt;
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 0.5);glVertex3f(0.0, H, 0.0);
	t = 0.0; dt = 2 * PI / nslice;
	for (int i = 0; i <= nslice; i++)
	{
		glTexCoord2f(0.5 + 0.5*cos(t), 0.5 + 0.5*sin(t));glVertex3f(R2*cos(t), H, -R2*sin(t));
		t = t + dt;
	}
	glEnd();

	glPopMatrix();
}
void redraw()
{
//�����ǰ��ɫ����Ȼ���
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//����ģ����ͼ����
glLoadIdentity();
// ���������������������λ��eye���۲�����λ��center�Լ�������Ϸ���������0��1��0������һ���۲����
gluLookAt(eye[0], eye[1], eye[2],
center[0], center[1], center[2],
0, 1, 0);

//ѡ��ģ����Ⱦģʽ
if (bWire) {
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//ǰ����ͺ����棬��ʾ������Ϊ�߶�
}
else {
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//ǰ����ͺ����棬��ʾ������Ϊ��
}

//����Ȳ���
glEnable(GL_DEPTH_TEST);
//���ù���
glEnable(GL_LIGHTING);
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; //��Դ��ɫ��͸����
GLfloat light_pos[] = { 5,5,5,1 };          //��Դλ��
//��0�ŵƽ���λ�÷��úͻ�������ɫ���ò���
glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
glLightfv(GL_LIGHT0, GL_AMBIENT, white);
glEnable(GL_LIGHT0);
//������
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�͹���Ч�����
//��ģ�ͱ任����������ϵ
/*glRotatef(fRotate, 0, 1.0f, 0);			// �������÷����ģ����Y����תfRotate�Ƕ�
glRotatef(-90, 1, 0, 0);				//��ģ����X����ת-90��������Ҫ�ķ���
glScalef(0.2, 0.2, 0.2);                //���ŵ�0.2��
Draw_Scene();						    // ����Drew_Scene��ģ
//���bAnim��Ҫ������ģ����Y����ת����fRotate��ÿ�μ�0.5f */
glColor3f(1.0, 1.0, 1.0);
glScalef(0.2, 0.2, 0.2);

//���Ƴ�����ģ��
for (int i = 0;i < bCube;i++)
{
glPushMatrix();
glTranslatef(CubePos[i][0], CubePos[i][1], CubePos[i][2]);
glScalef(CubeSca[i][0], CubeSca[i][1], CubeSca[i][2]);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture[0]);
//��������ʱʹ��
ModelCube();
glDisable(GL_TEXTURE_2D);
glPopMatrix();
}
//������ģ�ͣ�ֻ�ܼӲ���ɾ��������ֱ��������
for (int i = 0;i < bSphere;i++)
{
	glPushMatrix();
	glTranslatef(SpherePos[i][0], SpherePos[i][1], SpherePos[i][2]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glutSolidSphere(SphereRadius[i], 10, 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
//����Բ����
for (int i = 0;i < bCylinder;i++)
{
glPushMatrix();
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture[2]);
glTranslatef(CylinderPos[i][0], CylinderPos[i][1], CylinderPos[i][2]);
ModelCylinder(CylinderRadius[i],CylinderHeight[i]);
glDisable(GL_TEXTURE_2D);
glPopMatrix();
}
//����Բ׶
for (int i = 0;i < bCone;i++)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(ConePos[i][0], ConePos[i][1], ConePos[i][2]);
	ModelCone(ConeRadius[i],ConeHeight[i]);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
//��������
for (int i = 0;i < bPrism;i++)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTranslatef(PrismPos[i][0], PrismPos[i][1], PrismPos[i][2]);
	ModelPrism(PrismRadius[i], PrismRadius[i], PrismHeight[i],EdgePrism[i]);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
//������̨
for (int i = 0;i < bPrismTable;i++)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(PrismTablePos[i][0], PrismTablePos[i][1], PrismTablePos[i][2]);
	ModelPrism(PrismTableRadius[i][0], PrismTableRadius[i][1], PrismTableHeight[i], EdgePrismTable[i]);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

if (bAnim) fRotate += 0.5f;

//���tRotate��Ҫ����������Y����ת����tearotate��ÿ�μ�0.5f
if (tRotate) tearotate += 0.5f;
//��������
glutSwapBuffers();
}

GLint ModelsubMenu;

void ModelSubMenu(GLint ModelOption)
{
	switch (ModelOption)
	{
	case 1:
		//����������
		printf("����������λ��&�����峤��ߣ�x y z length width height\n");
		scanf_s("%f %f %f %f %f %f", &CubePos[bCube][0], &CubePos[bCube][1], &CubePos[bCube][2],
			&CubeSca[bCube][0], &CubeSca[bCube][1], &CubeSca[bCube][2]);
		bCube++;
		break;
	case 2:
		//������
		printf("��������λ�úͰ뾶��x y z Radius\n");
		scanf_s("%f %f %f %f", &SpherePos[bSphere][0], &SpherePos[bSphere][1], &SpherePos[bSphere][2],
								&SphereRadius[bSphere]);
		bSphere++;
		break;
	case 3:
		//����Բ��
		printf("Բ��������λ��&Բ����뾶�͸ߣ�x y z Radius Height\n");
		scanf_s("%f %f %f %f %f", &CylinderPos[bCylinder][0], &CylinderPos[bCylinder][1], &CylinderPos[bCylinder][2],
			&CylinderRadius[bCylinder], &CylinderHeight[bCylinder]);
		bCylinder++;
		break;
	case 4:
		//����Բ׶
		printf("Բ׶������λ��&Բ׶�뾶�͸ߣ�x y z Radius height\n");
		scanf_s("%f %f %f %f %f", &ConePos[bCone][0], &ConePos[bCone][1], &ConePos[bCone][2],
			&ConeRadius[bCone], &ConeHeight[bCone]);
		bCone++;
		break;
	case 5:
		//���ƶ���������
		printf("����������λ��&�������Բ�뾶�͸�&������x y z Radius height Edge \n");
		scanf_s("%f %f %f %f %f %d", &PrismPos[bPrism][0], &PrismPos[bPrism][1], &PrismPos[bPrism][2],
			&PrismRadius[bPrism], &PrismHeight[bPrism],&EdgePrism[bPrism]);
		bPrism++;
		break;
	case 6:
		//���ƶ�������̨
		printf("��̨����λ��&���桢�������Բ�뾶�͸�&������x y z Radius1 Radius2 height Edge \n");
		scanf_s("%f %f %f %f %f %f %d", &PrismTablePos[bPrismTable][0], &PrismTablePos[bPrismTable][1], &PrismTablePos[bPrismTable][2],
			&PrismTableRadius[bPrismTable][0],
			&PrismTableHeight[bPrismTable], &EdgePrismTable[bPrismTable]);
		bPrismTable++;
		break;
	default:
		break;
	}
}

void MyMenu(GLint ModelOption)
{
if (ModelOption == ModelsubMenu)
printf("The current menu is Main Menu.\n");

}

static GLubyte texImage[64][64][4];//�Լ��������������
GLubyte selftex[64][64][3];

void initTex()
{
glGenTextures(4, texture);  // ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
texload(0, "Monet.bmp");
texload(1, "Crack.bmp");
//���������Զ�������
//XXXXXX(); //�ο�opengl red book����������ͺ����Ĳ��衣
//����ͼ��
//����RGB����64*64��С
const int nm = 64;

for (int i = 0; i < nm; i++) {
for (int j = 0; j < nm; j++) {
int x = 0;
if (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) //�������ϣ�����ʹ��λ������������8*8����
x = 255;
selftex[i][j][0] = (GLubyte)x;  //R
selftex[i][j][1] = (GLubyte)0;	//G
selftex[i][j][2] = (GLubyte)0;	//B
}
}
//��texture[2]�󶨵���ǰGL_TEXTURE_2D������в���
glBindTexture(GL_TEXTURE_2D, texture[2]);
//�������ش洢ģʽ��������ȡ��ͼ�����ݵ��ж��뷽ʽ.
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//glTexImage2D(GLenum target, GLint level, GLenum internalformat,
//GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
//����ָ���Ĳ���������һ��2D��������width,heightΪ�������pixelsΪָ��ͼ����Ϣ��ָ��
glTexImage2D(GL_TEXTURE_2D, 0, 3, nm, nm, 0, GL_RGB, GL_UNSIGNED_BYTE, selftex);
//����������˷�ʽ��st�����ϵ���ͼģʽ
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

texload(3, "Spot.bmp");

}


int main(int argc, char *argv[])
{
glutInit(&argc, argv); //glut��ʼ����ڲ���
glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //���ó�ʼ������ʾģʽΪRGBA��ɫģʽ����Ȼ����Լ�ָ��˫����
glutInitWindowSize(480, 480);//���ô��ڿ��Ϊ480��480
int windowHandle = glutCreateWindow("Tea With Desk");
//��������
initTex();

ModelsubMenu = glutCreateMenu(ModelSubMenu);
glutAddMenuEntry("������", 1);
glutAddMenuEntry("��", 2);
glutAddMenuEntry("Բ��", 3);
glutAddMenuEntry("Բ׶", 4);
glutAddMenuEntry("����������", 5);
glutAddMenuEntry("��������̨", 6);

glutCreateMenu(MyMenu);
glutAddSubMenu("��ģ", ModelsubMenu);
glutAttachMenu(GLUT_RIGHT_BUTTON);

glutDisplayFunc(redraw);	 //ע����ƺ���redraw
glutReshapeFunc(reshape);    //���ڸı�ʱ�ص�����reshape
glutKeyboardFunc(key);		 //ע����̴�����key
glutIdleFunc(idle);			 //ϵͳ��Ͼʱ�ͻ����idle

glutMainLoop();			     //�¼�����ѭ��
return 0;
}