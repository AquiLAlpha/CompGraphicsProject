#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
#include <windows.h>
#endif

#include<gl/GL.h>
#include<GL/GLU.h>

#include <string>
#include <vector>
#include <fstream>



using std::string;
using std::vector;
using namespace std;

struct TGA_Header
{
	GLubyte  ID_Length;
	GLubyte  ColorMapType;
	GLubyte	 ImageType;
	GLubyte  colorMapSpecification[5];
	GLshort	 xOrigin;
	GLshort	 yOrigin;
	GLshort	 ImageWidth;
	GLshort	 ImageHeight;
	GLubyte  PixelDepth;
	GLubyte  ImageDescriptor;
};

class Texture
{
	//Functions
public:
	Texture(string in_filename, string in_name);
	/*{
			

			loadTGA(in_filename);

			name = in_name;

			textures.push_back(this);
	}*/

	~Texture();
	/*{
		for (vector<Texture*>::iterator it = textures.begin();it != textures.end(); it++)
			if ((*it) == this)
				textures.erase(it);

		if (imageData)
			delete imageData;
	}*/
	//Variables
public:
    unsigned char *imageData;
	unsigned int  bpp;
	unsigned int width;
	unsigned int height;
	unsigned int texID;

	string   name;

    static vector<Texture *> textures;
private:
	bool loadTGA(string filename);
	/*{
		{
			//vector<Texture *> textures;

			TGA_Header TGAheader;

			ifstream file(filename.data(), std::ios_base::binary);

			if (!file.is_open())
				return false;

			if (!file.read((char *)&TGAheader, sizeof(TGAheader)))
				return false;

			if (TGAheader.ImageType != 2)
				return false;

			width = TGAheader.ImageWidth;
			height = TGAheader.ImageHeight;
			bpp = TGAheader.PixelDepth;

			if (width <= 0 ||
				height <= 0 ||
				(bpp != 24 && bpp != 32))
			{
				return false;
			}
			GLuint type = GL_RGBA;

			if (bpp == 24)
				type = GL_RGB;

			GLuint bytesPerPixel = bpp / 8;
			GLuint imageSize = width*height*bytesPerPixel;

			imageData = (unsigned char*)(new GLubyte[imageSize]);

			if (imageData == NULL)
				return false;
	

			if (!file.read((char *)imageData, imageSize))
			{
				delete imageData;

				return false;
			}

			//BGR RGB
			for (GLuint i = 0; i < (int)imageSize;i += bytesPerPixel)
			{
				GLuint temp = imageData[i];
				imageData[i] = imageData[i + 2];
				imageData[i + 2] = temp;
			}

			createTexture(imageData, width, height, type);

			return true;
		}
	}*/
	bool createTexture(unsigned char *imageData, int width, int height, int type);
	/*{
		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

		return true;
	}*/


};

#endif
