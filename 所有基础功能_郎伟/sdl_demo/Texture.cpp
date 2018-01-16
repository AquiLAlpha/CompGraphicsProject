#include "Texture.h"


vector<Texture *> Texture::textures;

Texture::Texture(string in_filename, string in_name )
{
	imageData = NULL;
	char filename[512];
	strcpy_s(filename, in_filename.data());
	
	if (strstr(filename, "bmp"))
		loadBmp(in_filename);

	loadTGA(in_filename);

	name = in_name;

	textures.push_back(this);
}

Texture::~Texture()
{
	for (vector<Texture*>::iterator it = textures.begin();it != textures.end(); it++)
		if ((*it) == this)
			textures.erase(it);

	if (imageData)
	    delete imageData;
}

bool Texture::loadTGA(string filename)
{
	{
		//vector<Texture *> textures;

		TGA_Header TGAheader;

		ifstream file(filename.data(), std::ios_base::binary);

		if (!file.is_open())
		{
		  return false;
		}

		if (!file.read((char *)&TGAheader, sizeof(TGAheader)))
		{
				return false;
		}

		if (TGAheader.ImageType != 2)
		{
			return false;
		}

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
}

bool Texture::loadBmp(string in_filename)
{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	FILE *filePtr;

	
	fopen_s(&filePtr,in_filename.data(), "rb");

	if (filePtr == NULL)
		return false;

	fread_s(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	if (bitmapFileHeader.bfType != BITMAP_ID)
		return false;

	fread_s(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER), 1, filePtr);
	
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;
	int imageSize = bitmapInfoHeader.biSizeImage;
	imageData = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	fread_s(imageData, imageSize, 1, imageSize, filePtr);

	if (imageData==NULL)
		return false;


	for (int i = 0; i < (int)imageSize; i += 3)
	{
		GLuint temp = imageData[i];
		
		imageData[i] = imageData[i + 2];

		imageData[i + 2] = temp;
	}

	fclose(filePtr);

	createTexture(imageData, width, height, GL_RGB);

	return true;
}


bool Texture::createTexture(unsigned char *imageData, int width, int height, int type)
{
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	return true;

}