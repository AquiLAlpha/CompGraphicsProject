

#include <iostream>

#define BMP_Header_Length 54

#ifdef WIN32
#include <windows.h>
#endif


#include<gl/GL.h>
#include<GL/GLU.h>
#include<GL/glut.h>

#include "Texture.h"
#include "Light.h"
#include "GLEngine.h"

#include "Control.h"
#include "Button.h"
#include "ListBox.h"
#include "Slider.h"
#include "Model.h"
#include  "Node.h"


#include "Cube.h"
#include "Plane.h"
#include "Prism.h"
#include "Sphere.h"

GLsizei windowWidth = 800;
GLsizei windowHeight = 800;
GLfloat RotateX = 45.0f;
GLfloat RotateY = 45.0f;
GLfloat zoom = -50.0f;

bool keys[255];


Texture *texture = NULL;
Light  *mainLight = NULL;

bool navigating = false;

int *seed = NULL;
Control *controlled = NULL;
MouseState state;


Button *btnCreateCube, *btnCreatePlane,*btnCreatePrism,*btnCreateSphere;

Slider *sldTranslateX, *sldTranslateY, *sldTranslateZ;
Slider *sldRotateX, *sldRotateY, *sldRotateZ;
Slider *sldScaleX, *sldScaleY, *sldScaleZ;
//调试材料
Slider *sldKaRed, *sldKaGreen, *sldKaBlue, *sldKaAlpha;
Slider *sldKdRed, *sldKdGreen, *sldKdBlue, *sldKdAlpha;
Slider *sldKsRed, *sldKsGreen, *sldKsBlue, *sldKsAlpha;
Slider *sldKeRed, *sldKeGreen, *sldKeBlue, *sldKeAlpha;
Slider *sldShininess;

//调试纹理
ListBox *lstTGA;
vector<Texture *> Textures;
Button *btnTexture;

//调试模型
ListBox *lstModels;
vector <Model*> models;
bool drawModel=false;
Button *btnDrawobj;
Button *btnNotDrawobj;

//调试模型导出
Button *btnExportobj;


Button *btnParent, *btnUnparent;

Mesh *selectedNode = NULL;

list<Mesh *>selectedItems;


//调试光照
vector <Light*> lights;
Slider *sldred, *sldgreen, *sldblue,*slda;
Slider *sldpositionx, *sldpositiony, *sldpositionz;
Slider *slddirectionx, *slddirectiony, *slddirectionz;
Slider *sldcutoff;
ListBox *lstLight;
Button *btnLighton;
bool lightOn = true;
Button *btnAddlight;

//动画
bool Aminate = false;
float translateX = 0.0f;
float translateY = 0.0f;

//球的半径
float sphereRadius;
//prismNumEdge面体 多面体底面半径prismRadius1，prismRadius2，prismHeight
int   prismNumEdge, lastprismNumEdge=100;
float prismRadius1, lastprismRadius1=5.0f;
float prismRadius2, lastprismRadius2=5.0f;
float prismHeight,  lastprismHeight=5.0f;

//选择物体是否触发
bool RenderSelection = false;

GLvoid reshape(GLsizei width, GLsizei height)
{
	windowWidth = width;
	windowHeight = height;

}

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
}

GLvoid setOrtho(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, width, height, 0);

}

void updateControls(void)
{
	sldTranslateX->setValue(&selectedNode->getTransform().translate.x);
	sldTranslateY->setValue(&selectedNode->getTransform().translate.y);
	sldTranslateZ->setValue(&selectedNode->getTransform().translate.z);

	sldRotateX->setValue(&selectedNode->getTransform().rotation.x);
	sldRotateY->setValue(&selectedNode->getTransform().rotation.y);
	sldRotateZ->setValue(&selectedNode->getTransform().rotation.z);

	sldScaleX->setValue(&selectedNode->getTransform().scale.x);
	sldScaleY->setValue(&selectedNode->getTransform().scale.y);
	sldScaleZ->setValue(&selectedNode->getTransform().scale.z);

	//调试
	sldKaRed->setValue(&selectedNode->getMeshMaterial().Ka[0]);
	sldKaGreen->setValue(&selectedNode->getMeshMaterial().Ka[1]);
	sldKaBlue->setValue(&selectedNode->getMeshMaterial().Ka[2]);
	sldKaAlpha->setValue(&selectedNode->getMeshMaterial().Ka[3]);

	sldKdRed->setValue(&selectedNode->getMeshMaterial().Kd[0]);
	sldKdGreen->setValue(&selectedNode->getMeshMaterial().Kd[1]);
	sldKdBlue->setValue(&selectedNode->getMeshMaterial().Kd[2]);
	sldKdAlpha->setValue(&selectedNode->getMeshMaterial().Kd[3]);

	sldKsRed->setValue(&selectedNode->getMeshMaterial().Ks[0]);
	sldKsGreen->setValue(&selectedNode->getMeshMaterial().Ks[1]);
	sldKsBlue->setValue(&selectedNode->getMeshMaterial().Ks[2]);
	sldKsAlpha->setValue(&selectedNode->getMeshMaterial().Ks[3]);

	sldKeRed->setValue(&selectedNode->getMeshMaterial().Ke[0]);
	sldKeGreen->setValue(&selectedNode->getMeshMaterial().Ke[1]);
	sldKeBlue->setValue(&selectedNode->getMeshMaterial().Ke[2]);
	sldKeAlpha->setValue(&selectedNode->getMeshMaterial().Ke[3]);

	sldShininess->setValue(&selectedNode->getMeshMaterial().Shininess);

	//光源
	sldred->setValue(&lights[lstLight->getIndex() ]->diffuse[0]);
	sldgreen->setValue(&lights[lstLight->getIndex() ]->diffuse[1]);
	sldblue->setValue(&lights[lstLight->getIndex()]->diffuse[2]);
	slda->setValue(&lights[lstLight->getIndex() ]->diffuse[3]);

	sldpositionx->setValue(&lights[lstLight->getIndex() ]->position[0]);
	sldpositiony->setValue(&lights[lstLight->getIndex() ]->position[1]);
	sldpositionz->setValue(&lights[lstLight->getIndex() ]->position[2]);

	slddirectionx->setValue(&lights[lstLight->getIndex() ]->spotDirection[0]);
	slddirectiony->setValue(&lights[lstLight->getIndex() ]->spotDirection[1]);
	slddirectionz->setValue(&lights[lstLight->getIndex() ]->spotDirection[2]);

	sldcutoff->setValue(&lights[lstLight->getIndex() ]->cutoff);

}



GLvoid initGL(GLsizei width, GLsizei height)
{
	iGLEngine->Initialize(width, height);

	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);


	mainLight = new Light(LIGHT_SPOT);
	mainLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
	mainLight->setPosition(0, 30, 0);
	mainLight->setSpotDirection(0, -100, 0);
	mainLight->setAmbient(1.0, 1.0, 1.0, 1.0);
	mainLight->setCutoff(30.0);



	Plane *plane = new Plane();
	plane->setDivisions(10);
	plane->setSize(180, 180);
	plane->createPrimitive();
	plane->getTransform().setTranslate(0, -10, 0);
	selectedNode = plane;

	btnCreateCube = (Button *)addControl(new Button("Cube", 0, 0, 45, 30));
	btnCreatePrism = new Button("Prism", 55,  25, 45, 30);
	btnCreatePlane = new Button("Plane", 105, 25, 45, 30);
	btnCreateSphere = new Button("Sphere", 155, 25, 50, 30);

	sldTranslateX = (Slider *)addControl(new Slider("TranslateX", -100.0f, 100.0f, 0, 0, 200, 20));
	sldTranslateY = (Slider *)addControl(new Slider("TranslateY", -100.0f, 100.0f, 0, 0, 200, 20));
	sldTranslateZ = (Slider *)addControl(new Slider("TranslateZ", -100.0f, 100.0f, 0, 0, 200, 20));

	sldRotateX = (Slider *)addControl(new Slider("RotateX", -360.0f, 360.0f, 0, 0, 200, 20));
	sldRotateY = (Slider *)addControl(new Slider("RotateY", -360.0f, 360.0f, 0, 0, 200, 20));
	sldRotateZ = (Slider *)addControl(new Slider("RotateZ", -360.0f, 360.0f, 0, 0, 200, 20));

	sldScaleX = (Slider *)addControl(new Slider("ScaleX", 0.1f, 10.0f, 0, 0, 200, 20));
	sldScaleY = (Slider *)addControl(new Slider("ScaleY", 0.1f, 10.0f, 0, 0, 200, 20));
	sldScaleZ = (Slider *)addControl(new Slider("ScaleZ", 0.1f, 10.0f, 0, 0, 200, 20));

	btnParent = (Button *)addControl(new Button("Parent", 0, 0, 97, 30));
	btnUnparent = new Button("Unparent", 108, 285, 97, 30);

	//调试材料滑条
	sldKaRed = (Slider *)addControl(new Slider("KaR", 0.0f, 1.0f, 0, 0, 60, 20));
	sldKaGreen = new Slider("KaG", 0.0f, 1.0f, 66, 320, 60, 20);
	sldKaBlue =  new Slider("KaB", 0.0f, 1.0f, 127, 320, 60, 20);
	sldKaAlpha = new Slider("KaA", 0.0f, 1.0f, 188, 320, 60, 20);

	sldKdRed = (Slider *)addControl(new Slider("KdR", 0.0f, 1.0f, 0, 0, 60, 20));
	sldKdGreen = new Slider("KdG", 0.0f, 1.0f, 66, 345, 60, 20);
	sldKdBlue = new Slider("KdB", 0.0f, 1.0f, 127, 345, 60, 20);
	sldKdAlpha = new Slider("KdA", 0.0f, 1.0f, 188, 345, 60, 20);

	sldKsRed = (Slider *)addControl(new Slider("KsR", 0.0f, 1.0f, 0, 0, 60, 20));
	sldKsGreen = new Slider("KsG", 0.0f, 1.0f, 66, 370, 60, 20);
	sldKsBlue = new Slider("KsB", 0.0f, 1.0f, 127, 370, 60, 20);
	sldKsAlpha = new Slider("KsA", 0.0f, 1.0f, 188, 370, 60, 20);

	sldKeRed = (Slider *)addControl(new Slider("KeR", 0.0f, 1.0f, 0, 0, 60, 20));
	sldKeGreen = new Slider("KeG", 0.0f, 1.0f, 66, 395, 60, 20);
	sldKeBlue = new Slider("KeB", 0.0f, 1.0f, 127, 395, 60, 20);
	sldKeAlpha = new Slider("KeA", 0.0f, 1.0f, 188, 395, 60, 20);

	sldShininess= (Slider *)addControl(new Slider("Shininess", 0.0f, 80.0f, 0, 0, 80, 20));


	//调试纹理了
	//texture = new Texture("surface.tga", "Surface Texture");

	lstTGA = (ListBox *)addControl(new ListBox(0, 0, 160, 55));
	lstTGA->addItem("NULL");
	lstTGA->addItem("Dear.tga");
	lstTGA->addItem("sky.bmp");
	
	btnTexture = new Button("Texture", 170,445, 60, 55);
		
	

	Textures.push_back(new Texture("Dear.tga", "Dear.tga"));
	Textures.push_back(new Texture("front.bmp", "sky.bmp"));

	//调试模型导入
	lstModels = (ListBox*)addControl(new ListBox(0, 0, 160, 85));
	lstModels->addItem("Bigmax.OBJ");
	
	btnDrawobj = new Button("drawobj", 170, 505, 60, 25);
	btnNotDrawobj = new Button("clcobj", 170, 535, 60, 25);


	models.push_back(new Model("Bigmax_White_OBJ.obj"));


	zoom = models[lstModels->getIndex()]->getRadius()*-2;

	//调试模型导出
	btnExportobj = new Button("export", 170, 565, 60, 25);



	//狗屎光源
	lstLight = (ListBox *)addControl(new ListBox(0, 0, 160, 60));
	btnAddlight = new Button("Addl", 170, 595, 60, 25);
	btnLighton = new Button("on", 170, 625, 60, 25);
	

	lstLight->addItem("MainLight");
	lights.push_back(mainLight);

	sldred = (Slider *)addControl(new Slider("lR", 0.0f, 1.0f, 0, 0, 60, 20));
	sldgreen = new Slider("lG", 0.0f, 1.0f, 66, 660, 60, 20);
	sldblue = new Slider("lB", 0.0f, 1.0f, 127, 660, 60, 20);
	slda = new Slider("lA", 0.0f, 1.0f, 188, 660, 60, 20);

	sldpositionx = (Slider *)addControl(new Slider("lpx", -200.0f, 200.0f, 0, 0, 60, 20));
	sldpositiony = new Slider("lpy", -200.0f, 200.0f, 66, 685, 60, 20);
	sldpositionz = new Slider("lpz", -200.0f, 200.0f, 127, 685, 60, 20);
	sldcutoff = new Slider("cutoff", 0.0f, 90.0f, 188, 685, 60, 20);

	slddirectionx = (Slider *)addControl(new Slider("lDx", -200.0f, 200.0f, 0, 0, 60, 20));
	slddirectiony = new Slider("lDy", -200.0f, 200.0f, 66, 710, 60, 20);
	slddirectionz = new Slider("lDz", -200.0f, 200.0f, 127, 710, 60, 20);


	updateControls();


}

GLvoid displayFPS(GLvoid)
{
	static long lastTime = glutGet(GLUT_ELAPSED_TIME);
	static long loops = 0;
	static GLfloat fps = 0.0f;
	int newTime = glutGet(GLUT_ELAPSED_TIME);

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime)*1000.0f;

		fps = (fps + newFPS) / 2.0f;

		lastTime = newTime;

		loops = 0;
	};
	iGLEngine->drawText(5, 5, "OpenGL Demo - %0.2f", fps);

	loops++;

}

//模型导出参数
int numv = 0;
int numvt = 0;
int numvn = 0;


//调试模型导出
bool ExportObj(string modelname)
{
	FILE *objFileptr;

	if (fopen_s(&objFileptr, modelname.data(), "w"))
	{
		printf_s("Can't open");
		exit(0);
	}

	/*fprintf_s(objFileptr, "v %f %f %f\n", 0.0f, 0.0f, 0.0f);
	fprintf_s(objFileptr, "v %f %f %f\n", 0.0f, 1.0f, 0.0f);
	fprintf_s(objFileptr, "v %f %f %f\n", 0.5f, 0.5f, 0.0f);

	fprintf_s(objFileptr, "vn %f %f %f\n", 0.0f, 0.0f, 1.0f);

	fprintf_s(objFileptr, "f %i//%i %i//%i %i//%i\n", 1, 1, 2, 1, 3, 1);*/

	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	/*
	glPushMatrix();
	glTranslatef(0, 0, zoom);
	glRotated(RotateX, 1, 0, 0);
	glRotated(RotateY, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();*/

	fprintf_s(objFileptr, "mtllib material.mtl\n");

	for (list<Node*>::iterator it = Node::getNodes().begin();it != Node::getNodes().end();it++)
	{
		Mesh *mesh = dynamic_cast<Mesh *>(*it);

		//调试
		glEnable(GL_TEXTURE_2D);

		if (mesh && mesh->getParent() == NULL)
			mesh->exportMesh(objFileptr, modelview, &numv, &numvt, &numvn);
	}

	fclose(objFileptr);

	return true;
}


GLvoid drawControls(GLvoid)
{
	for (list<Control*>::iterator it = Control::controls.begin();it != Control::controls.end();it++)
	{
		Control *control = (*it);

		control->drawControl();

		if ((controlled != NULL && controlled != control) || navigating)
			continue;

		if (control->updateControl(state))
		{
			controlled = control;

			if (control == btnCreateCube)
			{
				Cube *cube = new Cube();
				cube->createPrimitive();

				selectedNode = cube;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			else if (control == btnCreatePlane)
			{
				Plane *plane = new Plane();

				plane->setDivisions(10);
				plane->setSize(10, 10);
				plane->createPrimitive();


				selectedNode = plane;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			else if (control== btnCreatePrism)
			{
				Prism *prism = new Prism();

				printf_s("Note:if wanted a cylinder, set NumEdge to 100\n");
				printf_s("Please Input: NumEdge bottomRadius topRadius Height\n");

				if (scanf_s("%d %f %f %f", &prismNumEdge, &prismRadius1, &prismRadius2, &prismHeight)!=4)
				{
					prismNumEdge = lastprismNumEdge;
					prismRadius1 = lastprismRadius1;
					prismRadius2 = lastprismRadius2;
					prismHeight  = lastprismHeight;
				}
				else
				{
					lastprismNumEdge = prismNumEdge;
					lastprismRadius1 = prismRadius1;
					lastprismRadius2 = prismRadius2;
					lastprismHeight = prismHeight;
				}
				prism->setDivisions(prismNumEdge);
				prism->setSize(prismRadius1, prismRadius2, prismHeight);
				prism->createPrimitive();

				selectedNode = prism;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			else if (control == btnCreateSphere)
			{
				Sphere *sphere = new Sphere();

			
				printf_s("Please input the Radius of the new Sphere:\n");
				if (!scanf_s("%f", &sphereRadius))
					sphereRadius = 3.0f;

				sphere->setDivisions(20,10);
				sphere->setSize(sphereRadius);
				sphere->createPrimitive();

				selectedNode = sphere;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			//调试纹理
			else if (control == btnTexture)    
			{
				if (lstTGA->getIndex() == 0)
					selectedNode->getMeshMaterial().setDiffuseMap(NULL);
				else
					selectedNode->getMeshMaterial().setDiffuseMap(Textures[lstTGA->getIndex()-1]);
			}
			//绘制模型
			else if (control == btnDrawobj)
			{
				drawModel = true;

				zoom = models[lstModels->getIndex()]->getRadius() * -3;
			}
			else if (control == btnNotDrawobj)
			{
				drawModel = false;
			}
			else if (control == btnParent && (int)selectedItems.size() > 1)
			{
				Mesh *parent = (*(--selectedItems.end()));

				for (list<Mesh*>::iterator it = selectedItems.begin();it != selectedItems.end();it++)
				{
					Mesh  *mesh = (*it);

					if (mesh && mesh != parent)
						mesh->setParent(NULL);
				}

				for (list<Mesh*>::iterator it = selectedItems.begin();it != selectedItems.end();it++)
				{
					Mesh  *mesh = (*it);

					if (mesh && mesh != parent)
						mesh->setParent(parent);
				}

			}
			else if (control == btnUnparent)
			{
				for (list<Mesh*>::iterator it = selectedItems.begin();it != selectedItems.end();it++)
				{
					Mesh  *mesh = (*it);

					if (mesh)
						mesh->setParent(NULL);
				}
			}
			//导出obj
			else if (control == btnExportobj)
			{
				numv = 0;
				numvt = 0;
				numvn = 0;
				char newModelname[14];
				sprintf_s(newModelname, "NewModel%d.obj", (int)models.size());
			   
				if (!ExportObj(newModelname))
					printf_s("Export failed\n");
				else 
				{
					lstModels->addItem(newModelname);
					
					models.push_back(new Model(newModelname));
				}
			}
			else if (control == btnAddlight)
			{
				char newLightname[8];
				sprintf_s(newLightname, "Light%d", (int)lights.size());
				
				printf_s("Choose Light type:0--SPOT 1--POINT 2--DIRECTION\n");

				int lightType;
				if (scanf_s("%d", &lightType) == 1)
				{
					Light *newLight;
					if (lightType == 0)
					{
						newLight = new Light(LIGHT_SPOT);

						newLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
						newLight->setPosition(0, 30, 0);
						newLight->setSpotDirection(0, -100, 0);
						newLight->setCutoff(30.0);

						lights.push_back(newLight);

					}
					else if (lightType == 1)
					{
						newLight = new Light(LIGHT_POINT);

						newLight->setDiffuse(0.0, 1.0, 0.0, 1.0);
						newLight->setPosition(0, 30, 0);
						newLight->setSpotDirection(0, -100, 0);

						lights.push_back(newLight);
					}
					else if (lightType == 2)
					{
						newLight = new Light(LIGHT_DIRECTIONAL);

						newLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
						newLight->setPosition(0, 30, 0);

						lights.push_back(newLight);
					}

					lstLight->addItem(newLightname);
				}
				else printf_s("Can't add light\n");
			}
			else if (control == btnLighton)
			{
				lights[lstLight->getIndex()]->visible = !lights[lstLight->getIndex()]->visible;
			}
			//zoom = models[lstModels->getIndex()]->getRadius() * -3;
			//Handle events
		}
		else if (control == controlled)
		{
			controlled = NULL;
		}
		else
		{
			//Control has been updated,but no message
		}
	}
}

void deleteNode(Node *node)
{
	list <Node *> children = node->getChildren();
	for (list<Node *>::iterator it = children.begin();it != children.end();it++)
	{
		deleteNode((*it));
	}
	delete node;
}


GLboolean updateNavigation(GLvoid)
{
	const GLfloat RotateX_speed = 0.5f;
	const GLfloat RotateY_speed = 0.5f;
	const GLfloat zoom_speed = 1.0f;

	static int lastX = -1;
	static int lastY = -1;


	if (lastX == -1 && lastY == -1)
	{
		lastX = state.x;
		lastY = state.y;
	}

	int changeX = lastX - state.x;
	int changeY = lastY - state.y;

	lastX = state.x;
	lastY = state.y;

	if (state.LeftButtonDown &&controlled == NULL)
	{
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		RotateX -= (float)changeY*RotateX_speed;
		RotateY -= (float)changeX*RotateY_speed;

		if (RotateX < 1.0f)
			RotateX = 1.0f;


		navigating = true;
	}
	else if (state.RightButtonDown&&controlled == NULL)
	{

		zoom -= (float)changeX*zoom_speed;

		navigating = true;
	}
	else
	{
			navigating = false;
	}

	//F1键删除
	if (keys[GLUT_KEY_F1])
	{
		for (list<Mesh *>::iterator it = selectedItems.begin();it != selectedItems.end(); it++)
		{
			Mesh *mesh = dynamic_cast <Mesh*>(*it);

			if (mesh == NULL)
				continue;

			mesh->setParent(NULL);
		}

		for (list<Mesh *>::iterator it = selectedItems.begin();it != selectedItems.end(); it++)
		{
			Mesh *mesh = dynamic_cast <Mesh*>(*it);

			if (mesh == NULL)
				continue;

			deleteNode(mesh);
		}

		selectedItems.clear();
	}



	//esc键
	if (keys[27])
		return true;

	return false;

}

void drawScene(void);

void handleSelection(void)
{
	static bool buttonDown = false;

	if (state.MiddleButtonDown)
		buttonDown = true;
	else if (buttonDown)
	{
		buttonDown = false;

		GLuint buffer[512];
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(512, buffer);

		glRenderMode(GL_SELECT);

		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();

		gluPickMatrix(state.x, viewport[3] - state.y, 1.0f, 1.0f, viewport);

		gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 500.0f);


		RenderSelection = true;
		drawScene();
		RenderSelection = false;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		int totalHits = glRenderMode(GL_RENDER);

		if (totalHits > 0)
		{
			int choose = buffer[3];
			int depth = buffer[1];

			for (int i = 1;i < totalHits;i++)
			{
				if (buffer[i * 4 + 1] < (GLuint)depth)
				{
					choose = buffer[i * 4 + 3];
					depth = buffer[i * 4 + 1];
				}
			}

			selectedNode = reinterpret_cast<Mesh*>(choose);

			//同时按F2和middleButton才能选择多个
			if (!keys[GLUT_KEY_F2])
				selectedItems.clear();

			bool exists = false;
			for (list<Mesh*>::iterator it = selectedItems.begin();it != selectedItems.end();it++)
			{
				if ((*it) == selectedNode)
					exists = true;
			}

			if (!exists)
				selectedItems.push_back(selectedNode);

			updateControls();
		}
		else selectedItems.clear();
	}
}


float R = 50.0f;

GLvoid timerLoop(int value)
{
	if (!Aminate)
	{
		translateX = 0.0f;
		translateY = 0.0f;

		return;
	}

	float time = (float)glutGet(GLUT_ELAPSED_TIME);

	translateX = cosf(time / 120.0f)*R;
	translateY = -sinf(time / 120.0f)*R;

	RotateX = 0.0f;
	RotateY = 0.0f;
	//glutPostRedisplay();

	//glutTimerFunc(1, timerLoop, 0);
}


GLvoid drawScene()
{
	if (RenderSelection == false)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		establishProjectionMatrix(windowWidth, windowHeight);

	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//动画调试
	if (Aminate == true)
		glutTimerFunc(1, timerLoop, 0);

	glTranslatef(translateX, translateY, zoom);

	glRotated(RotateX, 1, 0, 0);
	glRotated(RotateY, 0, 1, 0);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);


	//glBindTexture(GL_TEXTURE_2D, texture->texID);
	/*glPushMatrix();
	glScalef(30, 30, 30);
	drawCube();
	glPopMatrix();*/
	glEnable(GL_LIGHTING);
	for (int i = 0;i < (int)Light::lights.size();i++)
	{
		/*double randomNumber = (double)SDL_GetTicks() + seed[i];

		float x = (float)sin(randomNumber / 1600.0f)*(float)cos(randomNumber / 1200.0f)*50.0f;
		float y = (float)sin(randomNumber / 900.0f)*(float)cos(randomNumber / 1400.0f)*50.0f;

		Light::lights[i]->setPosition(x, 30.0f, y);*/
		Light::lights[i]->updateLight();
	}

	//if (lstModels != NULL)
	//调试模型
	if (drawModel)
	{
		models[lstModels->getIndex()]->drawModel();
	}


	//调试
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	glDisable(GL_BLEND);
	
	//调试
	//glEnable(GL_BLEND);

	for (list<Node*>::iterator it = Node::getNodes().begin();it != Node::getNodes().end();it++)
	{
		Mesh *mesh = dynamic_cast<Mesh *>(*it);

		//调试
		glEnable(GL_TEXTURE_2D);

		if (mesh && mesh->getParent() == NULL)
			mesh->drawMesh();
	}

	list<Mesh *>items = selectedItems;
	while (items.size() > 0)
	{
		for (list<Node*>::iterator it = Node::getNodes().begin();it != Node::getNodes().end();it++)
		{
			Mesh *mesh = dynamic_cast<Mesh *>(*it);

			if (mesh && mesh->getParent() == NULL)
			{
				for (list<Mesh *>::iterator mit = items.begin();mit != items.end();mit++)
				{
					Mesh *insideMesh = (*mit);

					//glDisable(GL_LIGHTING);
					//调试
					//glDisable(GL_TEXTURE_2D);
					glEnable(GL_TEXTURE_2D);

					glColor4f(1.0f, 0.0f, 0.0f, 0.2f);

					if (mesh->drawMesh(true, insideMesh))
					{
						items.remove(insideMesh);
						break;
					}
				}

			}
		}
	}

	glEnable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);



	if (RenderSelection == true)
		return;

	//调试
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//pass2
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	setOrtho(windowWidth, windowHeight);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	displayFPS();
	drawControls();

	glDisable(GL_TEXTURE_2D);

	glFlush();

	glutSwapBuffers();

	handleSelection();


	if (updateNavigation() == true)
		exit(0);
}

void grab()
{
	FILE* pTempFile;

	FILE* pAimFile;

	GLubyte *pPixelData=0;

	GLubyte BMP_header[54];

	GLint i, j;

	GLint PixelDataLength;

	i = windowWidth * 3;

	while (i % 4 != 0)
		++i;


	PixelDataLength = i*windowHeight;

	//pPixelData = new GLubyte[PixelDataLength];
	pPixelData = (GLubyte *)malloc(PixelDataLength);
	memset(pPixelData, 0, PixelDataLength);

	if (pPixelData==0)
		return;

	if (fopen_s(&pTempFile, "Monet.bmp", "rb") != 0)
	{
		printf("Can't open reference file\n");
		return;
	}

	if (fopen_s(&pAimFile, "grab.bmp", "wb") != 0)
	{
		printf_s("Grab failed\n");
		return;
	}

	glFlush();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//glReadPixels(0, 0, windowWidth, windowHeight, GL_UNSIGNED_BYTE, GL_BGR_EXT, pPixelData);
	glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);


	//fread_s(BMP_header, sizeof(BMP_header), sizeof(BMP_header), 1, pTempFile);
	fread(BMP_header,  sizeof(BMP_header), 1, pTempFile);

	fwrite(BMP_header, sizeof(BMP_header), 1, pAimFile);

	fseek(pAimFile, 0x0012, SEEK_SET);

	GLint pixelWidth = windowWidth;
	GLint pixelHeight= windowHeight;

	fwrite(&pixelWidth, sizeof(pixelWidth), 1, pAimFile);
	fwrite(&pixelHeight, sizeof(pixelHeight), 1, pAimFile);



	fseek(pAimFile, 0, SEEK_END);

	fwrite(pPixelData, PixelDataLength, 1, pAimFile);


	fclose(pAimFile);
	fclose(pTempFile);

	free(pPixelData);
}


GLvoid keyboardCB(unsigned char key, int x, int y)
{
	keys[key] = true;

	//按下时连续绘制
	/*if (key == 'a')
	{
		Aminate = true;
	}
	else if (key == 's')
	{
		Aminate = false;
	}*/

}

GLvoid keyboardUpCB(unsigned char key, int x, int y)
{
	keys[key] = false;
}

GLvoid keyboardSpecialCB(int key, int x, int y)
{
	keys[key] = true;
}

GLvoid keyboardSpecialUpCB(int key, int x, int y)
{
	keys[key] = false;
}



GLvoid key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'g':
		grab();
		break;
	case 'a':
		Aminate =! Aminate;
		break;
	default:break;
	}
}




void MouseMotionCB(int x, int y)
{
	state.x = x;
	state.y = y;
}

void MousePassiveCB(int x, int y)
{
	state.x = x;
	state.y = y;
}

void MouseCB(int Mousebutton,int Mousestate,int x,int y)
{

	if (Mousestate == GLUT_DOWN)
	{
		if (Mousebutton == GLUT_LEFT_BUTTON)
			state.LeftButtonDown = 1;

		else if (Mousebutton == GLUT_RIGHT_BUTTON)
			state.RightButtonDown = 1;

		else if (Mousebutton == GLUT_MIDDLE_BUTTON)
			state.MiddleButtonDown = 1;
	}
	else {
		state.LeftButtonDown = 0;

		state.RightButtonDown = 0;

		state.MiddleButtonDown = 0;
	}
}



void idle()
{
	glutPostRedisplay();
}


int  main(int argc, char *argv[])
{


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);

	int windowID = glutCreateWindow("OpenGL Cube Demo");
	glutReshapeWindow(windowWidth, windowHeight);


	initGL(windowWidth, windowHeight);

	glutMouseFunc(MouseCB);
	glutPassiveMotionFunc(MousePassiveCB);
	glutMotionFunc(MouseMotionCB);

	glutDisplayFunc(drawScene);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboardCB);
	glutKeyboardUpFunc(keyboardUpCB);
	glutKeyboardFunc(key);
	glutSpecialFunc(keyboardSpecialCB);
	glutSpecialUpFunc(keyboardSpecialUpCB);
	glutIdleFunc(idle);


	glutMainLoop();



	return 0;
}