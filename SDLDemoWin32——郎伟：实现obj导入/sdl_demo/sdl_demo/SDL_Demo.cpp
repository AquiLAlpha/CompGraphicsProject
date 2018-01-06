/*#include <SDL.h>

//#ifdef WIN32
#include <Windows.h>
//#endif

#include <GL\glut.h>



const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Spit out error
	}
	SDL_Window *screen = SDL_CreateWindow("OpenGL SDL Demo",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		(int)windowWidth, (int)windowHeight,
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	if (screen==NULL)

	{
		//Spit out error
		return -1;

	}

	int done = 0;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
		}
	}
	SDL_Quit();
	return 1;
}
*/

#include <iostream>


#include <SDL.h> 

#ifdef WIN32
 #include <windows.h>
#endif

#include<gl/GL.h>
#include<GL/GLU.h>

#include "Texture.h"
#include "Light.h"
#include "GLEngine.h"

#include "Control.h"
#include "Button.h"
#include "ListBox.h"
#include "Slider.h"
#include "Model.h"

const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;
GLfloat RotateX = 45.0f;
GLfloat RotateY = 45.0f;
GLfloat zoom = -5.0f;

SDL_Window *win;
SDL_GLContext mainGLContext;
const Uint8 *keys;

Texture *texture = NULL;
Light  *mainLight = NULL;

bool navigating = false;

int *seed = NULL;
Control *controlled = NULL;
MouseState state;

ListBox *lstModels = NULL;
vector <Model*> models;

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

GLvoid setOrtho(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, width, height, 0);

}
GLvoid initGL(GLsizei width, GLsizei height)
{
	iGLEngine->Initialize(width, height);

	//iGLEngine->drawText();
	//Light::Initialize();

	establishProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	
	/*mainLight = new Light(LIGHT_SPOT);
	mainLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
	mainLight->setPosition(0,0,-170);
	mainLight->setSpotDirection(0,0,170);
	mainLight->setAmbient(1.0, 1.0, 1.0, 1.0);
	mainLight->setCutoff(90.0);*/

	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	mainLight->setPosition(13.34f, 18.857, 27.25f);

	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(0.555f, 0.514f, 0.728f, 1.0f);
	mainLight->setPosition(-14.675f, 21.56f, -13.97f);

	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(0.255f, 0.214f, 0.528f, 1.0f);
	mainLight->setPosition(1.142f, -45.365f, 3.695f);


	texture = new Texture("surface.tga", "Surface Texture");
	lstModels = (ListBox*)addControl(new ListBox(0, 0, 200, 200));
	lstModels->addItem("Bigmax.OBJ");
	lstModels->addItem("Male.OBJ");

	models.push_back(new Model("Bigmax_White_OBJ.obj"));
	models.push_back(new Model("Male.OBJ"));

	zoom = models[lstModels->getIndex()]->getRadius()*-2;

	/*addControl(new Button("Run away!", 0, 0, 200, 50));
	ListBox *lstNames=(ListBox *)addControl(new ListBox( 0, 0, 200, 200));
	lstNames->addItem("langwei");
	lstNames->addItem("dinghao");
	lstNames->addItem("binggou");
	lstNames->addItem("tonggou");
	lstNames->addItem("jiangou");
	lstNames->addItem("xugou");

	Slider *testSlider = (Slider*)addControl(new Slider("test Value", -5.0f, 5.0f, 0, 0, 300, 40));
	testSlider->setValue(&testValue);*/
	glDisable(GL_LIGHTING);
	

}

GLvoid displayFPS(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;
	int newTime = SDL_GetTicks();

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime)*1000.0f;

		fps = (fps + newFPS) / 2.0f;

		/*char title[80];
		sprintf_s(title, "OpenGL Demo - %0.2f", fps);

		SDL_SetWindowTitle(win,title);*/


		lastTime = newTime;

		loops = 0;
	};
	iGLEngine->drawText(5, 5, "OpenGL Demo - %0.2f", fps);

	loops++;

}

GLvoid drawGrid(GLvoid)
{
	const float width = 80.0f;
	const float height = 80.0f;

	const int division =100;

	float incX = width / (float)division;
	float incY = height / (float)division;

	glColor3f(0, 0, 0);
	glNormal3f(0, 1, 0);
	for (float x = -width / 2;x < width / 2;x += incX)
	{
		for (float y = -height / 2;y < height / 2;y += incY)
		{
			glBegin(GL_TRIANGLE_STRIP);
				glVertex3f(x + incX,0, y + incY);
				glVertex3f(x , 0, y + incY);
				glVertex3f(x + incX, 0, y);
				glVertex3f(x , 0, y );
			glEnd();
		}
	}
}

GLvoid drawCube()
{
	glBegin(GL_QUADS);
	//top face
	//glColor3f(1.0f, 0.5f, 0.0f);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, 1.0f, 1.0f);
	//bottom face
	//glColor3f(0.0f, 1.0f, 0.0f);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, 1.0f);
	//Front face
	//glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, 1.0f);
	//Back face
	//glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, -1.0f);
	//Left face
	//glColor3f(0.0f, 0.0f, 1.0f);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
	//Right face
	//glColor3f(1.0f, 0.0f, 1.0f);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

}

GLvoid drawControls(GLvoid)
{
	for (list<Control*>::iterator it = Control::controls.begin();it != Control::controls.end();it++)
	{
		Control *control = (*it);

		control->drawControl();

		if ((controlled != NULL && controlled != control)||navigating)
			continue;

		if (control->updateControl(state))
		{
			//controlled = control;
			zoom = models[lstModels->getIndex()]->getRadius() * -3;
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

GLvoid drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	establishProjectionMatrix(windowWidth, windowHeight);

	glDisable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();




	glTranslatef(0, 0, zoom);

	glRotated(RotateX, 1, 0, 0);
	glRotated(RotateY, 0, 1, 0);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

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

	if (lstModels != NULL)
		models[lstModels->getIndex()]->drawModel();
	

	/*
	if (gridList == 0)
	{
		gridList = glGenLists(1);
		glNewList(gridList, GL_COMPILE_AND_EXECUTE);

		for (GLfloat x = -20.0f;x < 20.0f;x += 2.5f)
		{
			for (GLfloat y = -20.0f;y < 20.0f;y += 2.5f)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();

				glTranslatef(x, y, 0.0f);
				drawCube();

				glPopMatrix();
			}
		}
		
		glEndList();
	}
	else
	{
		glCallList(gridList);
	}*/
	//draw cube


	glEnable(GL_LIGHTING);
	//drawGrid();




	//pass2
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	setOrtho(windowWidth, windowHeight);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	 displayFPS();
	 drawControls();

	glFlush();

	SDL_GL_SwapWindow(win);

	
}

/*GLboolean checkKeys(GLvoid)
{
	const GLfloat speed = 1.0f;
	if (keys[SDL_SCANCODE_ESCAPE])
		return true;

	if (keys[SDL_SCANCODE_LEFT])
		cubeRotateY -= speed;

	if (keys[SDL_SCANCODE_RIGHT])
		cubeRotateY += speed;

	if (keys[SDL_SCANCODE_UP])
		cubeRotateX -= speed;

	if (keys[SDL_SCANCODE_DOWN])
		cubeRotateX += speed;
	return false;
}*/

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
		SDL_SetRelativeMouseMode(SDL_TRUE);
		RotateX -= (float)changeY*RotateX_speed;
		RotateY -= (float)changeX*RotateY_speed;

		if (RotateX < 1.0f)
			RotateX = 1.0f;


		navigating = true;
	}  
	else if (state.RightButtonDown&&controlled == NULL)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		zoom -= (float)changeX*zoom_speed;

		navigating = true;
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		navigating = false;
	}
	if (keys[SDL_SCANCODE_ESCAPE])
		return true;

	return false;
}


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)!=0)//初始化音频和视频SDL库 
	{
		std::cout<<"can not init SDL.\n"<<SDL_GetError()<<std::endl;
		return 1;
	}

    win = SDL_CreateWindow("Hellow World", 100, 100, windowWidth, windowHeight, SDL_WINDOW_SHOWN| SDL_WINDOW_OPENGL);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error" << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	mainGLContext = SDL_GL_CreateContext(win);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


	initGL(windowWidth, windowHeight);
	
	/*seed = new int[Light::numLights];
	for (int i = 0;i < Light::numLights;i++)
	{
		Light *light = new Light(LIGHT_SPOT);

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;

		light->setDiffuse(r, g, b, 1);
		light->setAmbient(0,0,0, 1);
		light->setSpotDirection(0, -1, 0);

		float cutoff = 20.0f + (float)rand() / (float)RAND_MAX*60.0f;

		light->setCutoff(cutoff);
		light->setExponent(20.0f*cutoff);

		seed[i] = rand();
	}*/
	
	int done = 0;
	while (!done) 
	{
		SDL_GetMouseState(&state.x, &state.y);

		state.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
		state.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
		state.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);

		drawScene();
		

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
			
			keys = SDL_GetKeyboardState(NULL);
			
		}
		if (updateNavigation())
			done = 1;
	}
	/*
	for (int i = 0;i < (int)Light::lights.size();i++)
		delete Light::lights[i];

	delete seed;*/

	for (list<Control*>::iterator it = Control::controls.begin();it != Control::controls.end();it++)
	{
		delete (*it);

		it = Control::controls.begin();
	}
	GLEngine::Uninitialize();

	
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		std::cout << "SDL_CreateRender Error" << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string imagePath =  "icon.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout<<"SDL_LoadBMP Error:"<< SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);

	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error:" << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}


	for (int i = 0;i < 3;i++)
	{
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex, NULL, NULL);

		SDL_RenderPresent(ren);

		SDL_Delay(1000);
	}
	SDL_GL_DeleteContext(mainGLContext);
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}