// main.cpp : 定义控制台应用程序的入口点。

#define BITMAP_ID 0x4D42

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "glut.h"
#include "math.h"

//控制花球
GLint bSphere = 0;
//控制立方体个数
GLint bCube = 0;
//控制圆柱个数
GLint bCylinder = 0;
//控制圆锥个数
GLint bCone = 0;
//控制棱柱个数
GLint bPrism = 0;
GLint EdgePrism[10];
//控制棱台个数
GLint bPrismTable = 0;
GLint EdgePrismTable[10];

float PI = 3.14;
//纹理标识符
unsigned int texture[4];

//设置初始整个模型旋转角度为0
float fRotate = 0.0f;
//控制投影方式为正投影还是透视投影的布尔变量，初始为假，即正投影
bool bPersp = false;
//控制整个模型旋转与否，初始为假，即不旋转
bool bAnim = false;
//控制渲染后显示模式为线段还是面
bool bWire = false;

//记录当前窗口大小的高度和宽度
int wHeight = 0;
int wWidth = 0;

//观察矩阵建立所需变量
float eye[] = { 0, 0, 8 };		 //相机所在世界坐标系位置
float center[] = { 0, 0, 0 };    //观察中心坐标

//茶壶相关控制变量
float teapos[3] = { 0,0,4 + 1 }; //茶壶中心所在初始模型位置
float tearotate = 0.0f;          //茶壶绕其自身Y轴旋转的角度
bool tRotate = false;			 //控制茶壶独立旋转与否，初始设置为否，即不旋转

//桌腿建模，长宽高为1，1，3
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
FILE *filePtr;	// 文件指针
BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
unsigned char	*bitmapImage;		// bitmap图像数据
int	imageIdx = 0;		// 图像位置索引
unsigned char	tempRGB;	// 交换变量

// 以“二进制+读”模式打开文件filename
filePtr = fopen(filename, "rb");
if (filePtr == NULL) {
printf("file not open\n");
return NULL;
}
// 读入bitmap文件图
fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
// 验证是否为bitmap文件
if (bitmapFileHeader.bfType != BITMAP_ID) {
fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
return NULL;
}

// 读入bitmap信息头
fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
// 将文件指针移至bitmap数据
fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
// 为装载图像数据创建足够的内存
bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
// 验证内存是否创建成功
if (!bitmapImage) {
fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
return NULL;
}

// 读入bitmap图像数据
fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
// 确认读入成功
if (bitmapImage == NULL) {
fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
return NULL;
}

//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
for (imageIdx = 0;
imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
tempRGB = bitmapImage[imageIdx];
bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
bitmapImage[imageIdx + 2] = tempRGB;
}


// 关闭bitmap图像文件
fclose(filePtr);
return bitmapImage;
}
//加载纹理函数
BITMAPINFOHEADER TempbitmapInfoHeader;
unsigned char*   TempbitmapData;
void texload(int i, char *filename)
{

BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
unsigned char*   bitmapData;                                       // 纹理数据

bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
//保存下第一张crack的信息头和图片RGB信息地址
if (i == 1) {
TempbitmapInfoHeader = bitmapInfoHeader;
TempbitmapData = bitmapData;
};
//实现crack和spot的图片信息加权平均
if (i == 3)
{
for (int imageIdx = 0;
imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 1)
{
bitmapData[imageIdx] = 0.7*bitmapData[imageIdx] + 0.3*TempbitmapData[imageIdx];
}

}

glBindTexture(GL_TEXTURE_2D, texture[i]);
// 指定当前纹理的放大/缩小过滤方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

glTexImage2D(GL_TEXTURE_2D,
0, 	    //mipmap层次(通常为，表示最上层)
GL_RGB,	//我们希望该纹理有红、绿、蓝数据
bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2
bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2
0, //边框(0=无边框, 1=有边框)
GL_RGB,	//bitmap数据的格式
GL_UNSIGNED_BYTE, //每个颜色数据的类型
bitmapData);	//bitmap数据指针
}

void Draw_Leg()
{
glScalef(1, 1, 3);
glutSolidCube(1.0);
}

void Draw_Scene()
{
//茶壶建模
glPushMatrix();
glTranslatef(teapos[0], teapos[1], teapos[2]);//将茶壶平移至teapos坐标
glRotatef(90, 1, 0, 0);                     //茶壶绕X轴翻转
glRotatef(tearotate, 0, 1, 0);			    //先做茶壶绕当前坐标系中Y轴旋转，再去做上面一行代码中的绕X轴翻转
glutSolidTeapot(1);							//绘制单位茶壶模型
glPopMatrix();
//桌面建模，长宽厚为5，4，1
glPushMatrix();
glTranslatef(0, 0, 3.5);
glScalef(5, 4, 1);
glutSolidCube(1.0);
glPopMatrix();
//四个桌腿建模，平移至四个合适位置
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
glViewport(0, 0, width, height);						// 重置视口大小和窗口大小相同

glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
glLoadIdentity();									// 重置其为单位矩阵
//计算窗口宽高比
float whRatio = (GLfloat)width / (GLfloat)height;

//选择投影方式为透视矩阵还是正投影
if (bPersp) {
gluPerspective(45, whRatio, 0.1f, 100.0f);			//设置透视投影矩阵，视野角度为45度，视景体宽高比以及前后裁剪面位置
}
else
glOrtho(-3, 3, -3, 3, -100, 100);					//设置正投影矩阵投影面范围以及前后剪裁面位置

//选择当前矩阵模式为模型视图
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
//系统闲暇时，调用后处理显示函数，实现窗口内容改变了的图形绘制
void idle()
{
glutPostRedisplay();
}


//todo; hint: you may need another ARRAY when you operate the teapot

void key(unsigned char k, int x, int y)
{
switch (k)
{
//输入esc或者q都会退出程序
case 27:
case 'q': {exit(0); break; }
//输入p会通过改变bPersp真假值来切换正投影还是透视投影
case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; }
//输入空格来改变bAnim真假值来控制模型是否Y轴旋转
case ' ': {bAnim = !bAnim; break;}
//输入o来改变bWire真假以切换线框模型还是实体模型的渲染效果
case 'o': {bWire = !bWire; break;}

//输入a使得观察到的模型向左移动，通过使得相机位置和观察中心向右移实现
case 'a': {
eye[0] += 0.25f;   //使得每次相机x坐标增加0.25f
center[0] += 0.25f;//使得每次观察中心x坐标增加0.25f
break;
}
//输入d使得观察到的模型向右移动，通过使得相机位置和观察中心向左移实现
case 'd': {//todo
eye[0] -= 0.25f;   //使得每次相机x坐标减小0.25f
center[0] -= 0.25f;//使得每次观察中心x坐标减小0.25f
break;
}
//输入w使得观察到的模型向上移动，通过使得相机位置和观察中心向下移实现
case 'w': {//todo
eye[1] -= 0.25f;   //使得每次相机y坐标减小0.25f
center[1] -= 0.25f;//使得每次观察中心x坐标减小0.25f
break;
}
//输入s使得观察到的模型向下移动，通过使得相机位置和观察中心向上移实现
case 's': {//todo
eye[1] += 0.25f;   //使得每次相机y坐标增加0.25f
center[1] += 0.25f;//使得每次观察中心y坐标增加0.25f
break;
}
//输入z使得观察到的模型向前靠近，通过使得相机位置和观察中心向后实现
case 'z': {//todo
eye[2] -= 0.25f;   //使得每次相机z坐标减小0.25f
center[2] -= 0.25f;//使得每次观察中心x坐标减小0.25f
break;
}
//输入c使得观察到的模型向后远离，通过使得相机位置和观察中心向前实现
case 'c': {//todo
eye[2] += 0.25f;   //使得每次相机z坐标增加0.25f
center[2] += 0.25f;//使得每次观察中心z坐标增加0.25f
break;
}

//茶壶相关操作：kijl分别控制茶壶前后左右移动，e选择茶壶是否独立旋转。
case 'l': {//todo, hint:use the ARRAY that you defined, and notice the teapot can NOT be moved out the range of the table.
if (teapos[0]<2.5f) teapos[0] += 0.25f; //使得在茶壶建模时，茶壶向右移动
break;
}
case 'j': {//todo
if (teapos[0]>-2.5f) teapos[0] -= 0.25f;//使得在茶壶建模时，茶壶向左移动
break;
}
case 'i': {//todo
if (teapos[1] < 2.0f) teapos[1] += 0.2f;//使得在茶壶建模时，茶壶向上移动，在世界坐标系中得到向Z负方向运动
break;
}
case 'k': {//todo
if (teapos[1] > -2.0f) teapos[1] -= 0.2f;//使得在茶壶建模时，茶壶向下移动，在世界坐标系中得到向Z正方向运动
break;
}
case 'e': {//todo
tRotate = !tRotate;//改变tRotate真假值来选择茶壶是否旋转
break;
}
}
}
//球中心点坐标
GLfloat SpherePos[10][3];
GLfloat SphereRadius[10];
//立方体坐标和大小
GLfloat CubePos[10][3];
GLfloat CubeSca[10][3];
//圆柱体坐标和大小
GLfloat CylinderPos[10][3];
GLfloat CylinderRadius[10], CylinderHeight[10];
//圆锥坐标和大小
GLfloat ConePos[10][3];
GLfloat ConeRadius[10], ConeHeight[10];
//棱柱坐标和大小
GLfloat PrismPos[10][3];
GLfloat PrismRadius[10], PrismHeight[10];
//棱台坐标和大小
GLfloat PrismTablePos[10][3];
GLfloat PrismTableRadius[10][2], PrismTableHeight[10];

void ModelCube()
{
int i, j;
const GLfloat x1 = -0.5, x2 = 0.5;
const GLfloat y1 = -0.5, y2 = 0.5;
const GLfloat z1 = -0.5, z2 = 0.5;

GLfloat v[6][4][3] = {
{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },//下底面
{ { x1,y2,z2 },{ x2,y2,z2 },{ x2,y2,z1 },{ x1,y2,z1 } },//上底面
{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } },//前面
{ { x2,y1,z1 },{ x1,y1,z1 },{ x1,y2,z1 },{ x2,y2,z1 } },//后面
{ { x2,y1,z2 },{ x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 } },//右面
{ { x1,y1,z1 },{ x1,y1,z2 },{ x1,y2,z2 },{ x2,y2,z1 } }//左面
};
GLfloat texcoord[4][2] = { { 0.0f,0.0f },{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

//贴图六个正方形面
glBegin(GL_QUADS);
for (i = 0;i<6;i++)
for (j = 0;j < 4;j++)
{
//设置纹理坐标
glTexCoord2fv(texcoord[j]);
glVertex3fv(v[i][j]);

}
glEnd();

}

void ModelCylinder(float R,float H)
{
	float nslice = 100.0;
	//圆柱

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
	//圆锥
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
//绘制棱柱和棱台（其实也可以和圆锥和圆柱合并）
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
//清除当前颜色和深度缓存
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//重设模型视图矩阵
glLoadIdentity();
// 根据相机所在世界坐标中位置eye、观察中心位置center以及相机向上方向向量（0，1，0）生成一个观察矩阵
gluLookAt(eye[0], eye[1], eye[2],
center[0], center[1], center[2],
0, 1, 0);

//选择模型渲染模式
if (bWire) {
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//前向面和后向面，显示都设置为线段
}
else {
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//前向面和后向面，显示都设置为面
}

//打开深度测试
glEnable(GL_DEPTH_TEST);
//启用光照
glEnable(GL_LIGHTING);
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; //光源颜色和透明度
GLfloat light_pos[] = { 5,5,5,1 };          //光源位置
//对0号灯进行位置放置和环境光颜色设置并打开
glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
glLightfv(GL_LIGHT0, GL_AMBIENT, white);
glEnable(GL_LIGHT0);
//打开纹理
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//和光照效果混合
//将模型变换到世界坐标系
/*glRotatef(fRotate, 0, 1.0f, 0);			// 将调整好方向的模型绕Y轴旋转fRotate角度
glRotatef(-90, 1, 0, 0);				//将模型绕X轴旋转-90至我们需要的方向
glScalef(0.2, 0.2, 0.2);                //缩放到0.2倍
Draw_Scene();						    // 调用Drew_Scene建模
//如果bAnim即要求整个模型绕Y轴旋转，则fRotate对每次加0.5f */
glColor3f(1.0, 1.0, 1.0);
glScalef(0.2, 0.2, 0.2);

//绘制长方体模型
for (int i = 0;i < bCube;i++)
{
glPushMatrix();
glTranslatef(CubePos[i][0], CubePos[i][1], CubePos[i][2]);
glScalef(CubeSca[i][0], CubeSca[i][1], CubeSca[i][2]);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture[0]);
//绘制纹理时使用
ModelCube();
glDisable(GL_TEXTURE_2D);
glPopMatrix();
}
//绘制球模型（只能加不能删除，不能直接贴纹理）
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
//绘制圆柱体
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
//绘制圆锥
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
//绘制棱柱
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
//绘制棱台
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

//如果tRotate即要求茶壶独立绕Y轴旋转，则tearotate对每次加0.5f
if (tRotate) tearotate += 0.5f;
//交换缓存
glutSwapBuffers();
}

GLint ModelsubMenu;

void ModelSubMenu(GLint ModelOption)
{
	switch (ModelOption)
	{
	case 1:
		//绘制立方体
		printf("立方体中心位置&立方体长宽高：x y z length width height\n");
		scanf_s("%f %f %f %f %f %f", &CubePos[bCube][0], &CubePos[bCube][1], &CubePos[bCube][2],
			&CubeSca[bCube][0], &CubeSca[bCube][1], &CubeSca[bCube][2]);
		bCube++;
		break;
	case 2:
		//绘制球
		printf("球体中心位置和半径：x y z Radius\n");
		scanf_s("%f %f %f %f", &SpherePos[bSphere][0], &SpherePos[bSphere][1], &SpherePos[bSphere][2],
								&SphereRadius[bSphere]);
		bSphere++;
		break;
	case 3:
		//绘制圆柱
		printf("圆柱体中心位置&圆柱体半径和高：x y z Radius Height\n");
		scanf_s("%f %f %f %f %f", &CylinderPos[bCylinder][0], &CylinderPos[bCylinder][1], &CylinderPos[bCylinder][2],
			&CylinderRadius[bCylinder], &CylinderHeight[bCylinder]);
		bCylinder++;
		break;
	case 4:
		//绘制圆锥
		printf("圆锥体中心位置&圆锥半径和高：x y z Radius height\n");
		scanf_s("%f %f %f %f %f", &ConePos[bCone][0], &ConePos[bCone][1], &ConePos[bCone][2],
			&ConeRadius[bCone], &ConeHeight[bCone]);
		bCone++;
		break;
	case 5:
		//绘制多面体棱柱
		printf("棱柱体中心位置&底面外接圆半径和高&边数：x y z Radius height Edge \n");
		scanf_s("%f %f %f %f %f %d", &PrismPos[bPrism][0], &PrismPos[bPrism][1], &PrismPos[bPrism][2],
			&PrismRadius[bPrism], &PrismHeight[bPrism],&EdgePrism[bPrism]);
		bPrism++;
		break;
	case 6:
		//绘制多面体棱台
		printf("棱台中心位置&顶面、底面外接圆半径和高&边数：x y z Radius1 Radius2 height Edge \n");
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

static GLubyte texImage[64][64][4];//自己定义的纹理数组
GLubyte selftex[64][64][3];

void initTex()
{
glGenTextures(4, texture);  // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
texload(0, "Monet.bmp");
texload(1, "Crack.bmp");
//下面生成自定义纹理
//XXXXXX(); //参考opengl red book，理解后请解释函数的步骤。
//生成图像
//生成RGB数组64*64大小
const int nm = 64;

for (int i = 0; i < nm; i++) {
for (int j = 0; j < nm; j++) {
int x = 0;
if (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) //查阅资料，可以使用位与和亦或来生成8*8方格
x = 255;
selftex[i][j][0] = (GLubyte)x;  //R
selftex[i][j][1] = (GLubyte)0;	//G
selftex[i][j][2] = (GLubyte)0;	//B
}
}
//将texture[2]绑定到当前GL_TEXTURE_2D对象进行操作
glBindTexture(GL_TEXTURE_2D, texture[2]);
//设置像素存储模式控制所读取的图像数据的行对齐方式.
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//glTexImage2D(GLenum target, GLint level, GLenum internalformat,
//GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
//根据指定的参数，生成一个2D纹理，其中width,height为数组宽长，pixels为指向图像信息的指针
glTexImage2D(GL_TEXTURE_2D, 0, 3, nm, nm, 0, GL_RGB, GL_UNSIGNED_BYTE, selftex);
//设置纹理过滤方式和st方向上的贴图模式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

texload(3, "Spot.bmp");

}


int main(int argc, char *argv[])
{
glutInit(&argc, argv); //glut初始化入口参数
glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //设置初始窗口显示模式为RGBA颜色模式，深度缓存以及指定双缓存
glutInitWindowSize(480, 480);//设置窗口宽高为480，480
int windowHandle = glutCreateWindow("Tea With Desk");
//生成纹理
initTex();

ModelsubMenu = glutCreateMenu(ModelSubMenu);
glutAddMenuEntry("立方体", 1);
glutAddMenuEntry("球", 2);
glutAddMenuEntry("圆柱", 3);
glutAddMenuEntry("圆锥", 4);
glutAddMenuEntry("多面体棱柱", 5);
glutAddMenuEntry("多面体棱台", 6);

glutCreateMenu(MyMenu);
glutAddSubMenu("建模", ModelsubMenu);
glutAttachMenu(GLUT_RIGHT_BUTTON);

glutDisplayFunc(redraw);	 //注册绘制函数redraw
glutReshapeFunc(reshape);    //窗口改变时回调函数reshape
glutKeyboardFunc(key);		 //注册键盘处理函数key
glutIdleFunc(idle);			 //系统闲暇时就会调用idle

glutMainLoop();			     //事件处理循环
return 0;
}