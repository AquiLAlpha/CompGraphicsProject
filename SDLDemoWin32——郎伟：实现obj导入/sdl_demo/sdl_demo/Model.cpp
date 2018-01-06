#include "Model.h"

Model::Model(string name)
{
	objectLoaded = false;

	if (name != "")
		loadObject(name);

	displayList = 0;
}
Model::~Model()
{
	deleteObjects();
}

void   Model::drawModel(void)
{
	if (!objectLoaded)
		return;

	if (displayList != 0)
	{
		glCallList(displayList);
		return;
	}

	displayList = glGenLists(1);

	glNewList(displayList, GL_COMPILE_AND_EXECUTE);

	drawObject(false);
	drawObject(true);

	glEndList();

}
void   Model::drawObject(bool transparency )
{
	Material *lastMaterial = NULL;

	for (int i = 0;i < (int)objects.size();i++)
	{
		GroupObject *object = objects[i];

		for (int n = 0;n < (int)object->faces.size();n++)
		{
			Face *face = object->faces[n];

			Material *material = face->material;

			if (material != lastMaterial)
			{
				if(transparency == false && material->alpha < 1.0f)
					continue;

				material->Kd[3] = material->alpha;
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)material->Kd);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)material->Ks);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat *)material->Ke);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat )material->shininess);

				if (material->diffuseMap != NULL)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, material->diffuseMap->texID);
				}
				else 
					glDisable(GL_TEXTURE_2D);
				lastMaterial = material;
			}

			drawFace(*face);
		}
	}
}
void   Model::drawFace(Face &face)
{
	if ((int)face.numVertices <= 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_POLYGON);

	for (int v = 0; v < (int)face.numVertices;v++)
	{
		if ((int)face.numUVWs > v&&face.UVWs != NULL)
			glTexCoord2f(face.UVWs[v]->x, face.UVWs[v]->y);
		if ((int)face.numNormals > v&&face.normals != NULL)
			glNormal3f(face.normals[v]->x, face.normals[v]->y, face.normals[v]->z);
		if ((int)face.numVertices > v&&face.vertices != NULL)
			glVertex3f(face.vertices[v]->x, face.vertices[v]->y, face.vertices[v]->z);
	}

	glEnd();
}

void   Model::deleteObjects(void)
{
	for (int m = 0;m < (int)materials.size();m++)
	{
		if (materials[m]->ambientMap != NULL)
			delete materials[m]->ambientMap;

		if (materials[m]->diffuseMap != NULL)
			delete materials[m]->diffuseMap;

		if (materials[m]->specularMap != NULL)
			delete materials[m]->specularMap;

		if (materials[m]->emissionMap != NULL)
			delete materials[m]->emissionMap;

		if (materials[m]->shininessMap != NULL)
			delete materials[m]->shininessMap;

		if (materials[m]->transparencyMap != NULL)
			delete materials[m]->transparencyMap;

		if (materials[m]->bumpMap != NULL)
			delete materials[m]->bumpMap;

		delete materials[m];
	}
	for (int m = 0;m < (int)vertices.size();m++)
		delete vertices[m];

	for (int m = 0;m < (int)normals.size();m++)
		delete normals[m];

	for (int m = 0;m < (int)UVWs.size();m++)
		delete UVWs[m];

	for (int i = 0;i < (int)objects.size();i++)
	{
		GroupObject *object = objects[i];

		for (int f = 0;f < (int)object->faces.size();f++)
		{
			delete object->faces[f]->vertices;
			delete object->faces[f]->normals;
			delete object->faces[f]->UVWs;

			delete object->faces[f];
		}
	}
	UVWs.clear();
	normals.clear();
	vertices.clear();
	objects.clear();
	materials.clear();
}

bool   Model::loadObject(string name)
{
	filename = name;
	ifstream istr(filename.data());

	if (!istr) 
		return false;

	deleteObjects();

	displayList = 0;
	objectLoaded = false;

	GroupObject *defaultObject = new GroupObject;
	GroupObject *currentGroup = defaultObject;

	objects.push_back(defaultObject);

	/*char path[512];
	strcpy_s(path, filename.data());

	for (int i = (int)filename.length();path[i] != '\\'&& path[i] != '/';i--)
		path[i] = 0;*/

	Material *currentMaterial = NULL;
	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);

		string firstWord;
		newLine >> firstWord;

		if (firstWord == "#")
		{

		}
		else if (firstWord == "mtllib")
		{
			string materialFilename;

			while (newLine >> materialFilename)
			{
				//loadMaterials(string(path)+materialFilename);
				loadMaterials( materialFilename);
			}
		}
		else if (firstWord == "usemtl")
		{
			string materialName;
			newLine >> materialName;

			for (int i = 0;i < (int)materials.size();i++)
			{
				if (materials[i]->name == materialName)
				{
					currentMaterial = materials[i];
					break;
				}
			}
		}
		else if (firstWord == "v")
		{
			Vector3 *vertex = new Vector3;

			newLine >> vertex->x >> vertex->y >> vertex->z;

			vertices.push_back(vertex);

		}
		else if (firstWord == "vt")
		{
			Vector3 *uvw = new Vector3;

			newLine >> uvw->x >> uvw->y >> uvw->z;

			UVWs.push_back(uvw);
		}
		else if (firstWord == "vn")
		{
			Vector3 *normal = new Vector3;

			newLine >> normal->x >> normal->y >> normal->z;

			normals.push_back(normal);
		}
		else if (firstWord == "g")
		{
			string objectName;

			newLine >> objectName;

			if (objectName == "default")
				currentGroup = defaultObject;
			else
			{
				GroupObject *object = new GroupObject;

				object->objectName = objectName;
				newLine >> object->groupName;

				currentGroup = object;

				objects.push_back(object);
			}
		}
		else if (firstWord == "f")
		{
			Face *newFace = new Face;
			newFace->material = currentMaterial;

			currentGroup->faces.push_back(newFace);

			vector<Vector3 *> tempVertices;
			vector<Vector3 *> tempNormals;
			vector<Vector3 *> tempUVWs;
	
			bool noUV = false;

			char temp1='f';
	
			while ( !newLine.eof() )
			{
				int normal = 0;
				int vertex = 0;
				int uvw = 0;	

				if ((int)vertices.size() > 0)
				{
					newLine >> vertex;

					while (newLine.get(temp1))
						if (temp1 == '/' || temp1 == ' ')
							break;

					newLine.get(temp1);

					if (temp1 == '/')
						noUV = true;
					else
						newLine.unget();

					if (vertex == 0) break;

					tempVertices.push_back(vertices[vertex-1]);
				}

				if ((int)UVWs.size() > 0 && noUV == false)
				{
					newLine >> uvw;

					while (newLine.get(temp1))
						if (temp1 == '/' || temp1 == ' ')
							break;

					if (uvw>=1) tempUVWs.push_back(UVWs[uvw-1]);
				}
				if ((int)normals.size() > 0)
				{
					newLine >> normal;

					while (newLine.get(temp1))
						if (temp1 == '/' || temp1 == ' ')
							break;

					if (normal>=1) tempNormals.push_back(normals[normal-1]);
				}
			
			}
			newFace->numVertices = (int)tempVertices.size();
			newFace->numNormals = (int)tempNormals.size();
			newFace->numUVWs = (int)tempUVWs.size();

			newFace->vertices = new Vector3 *[newFace->numVertices];
			newFace->normals = new Vector3 *[newFace->numNormals];
			newFace->UVWs = new Vector3 *[newFace->numUVWs];

			for (int v = 0; v < newFace->numVertices; v++)
				newFace->vertices[v] = tempVertices[v];

			for (int v = 0; v < newFace->numNormals; v++)
				newFace->normals[v] = tempNormals[v];

			for (int v = 0; v < newFace->numUVWs; v++)
				newFace->UVWs[v] = tempUVWs[v];

			for (int v = 0; v < newFace->numVertices; v++)
				newFace->faceCenter   += (*tempVertices[v]);

			newFace->faceCenter = newFace->faceCenter /(float)newFace->numVertices;

			if (newFace->numVertices >= 3)
			{
				Vector3 vector1 = ( (*newFace->vertices[0]) - (*newFace->vertices[1]) ).Normalize();
				Vector3 vector2 = ( (*newFace->vertices[0]) - (*newFace->vertices[2]) ).Normalize();
				newFace->faceNormal = vector1*vector2;
			}
		}

	}

	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;

	center = Vector3(0, 0, 0);

	for (int n = 0;n < (int)vertices.size();n++)
	{
		if (n == 0)
		{
			xmin = xmax = vertices[n]->x;
			ymin = ymax = vertices[n]->y;
			zmin = zmax = vertices[n]->z;
			continue;
		}
		if (vertices[n]->x < xmin)
			xmin = vertices[n]->x;
		if (vertices[n]->x > xmax)
			xmax = vertices[n]->x;

		if (vertices[n]->y < ymin)
			ymin = vertices[n]->y;
		if (vertices[n]->y > ymax)
			ymax = vertices[n]->y;

		if (vertices[n]->z < zmin)
			zmin = vertices[n]->z;
		if (vertices[n]->z > zmax)
			zmax = vertices[n]->z;

		center += (*vertices[n]);

	}
	center = center / (float)vertices.size();

	boundingPoints[0] = Vector3(xmax, ymin, zmin);
	boundingPoints[1] = Vector3(xmin, ymax, zmin);
	boundingPoints[2] = Vector3(xmax, ymax, zmin);
	boundingPoints[3] = Vector3(xmin, ymin, zmin);

	boundingPoints[4] = Vector3(xmax, ymin, zmax);
	boundingPoints[5] = Vector3(xmin, ymax, zmax);
	boundingPoints[6] = Vector3(xmax, ymax, zmax);
	boundingPoints[7] = Vector3(xmin, ymin, zmax);

	radius = (Vector3(xmax, ymax, zmax) - Vector3(xmin, ymin, zmin)).Length() / 2.0f;

	objectLoaded = true;
	return true;

}

void   Model::loadMaterials(string filename)
{
	ifstream istr(filename.data());

	if (!istr)
		return;

	/*char path[256];
	strcpy_s(path, filename.data());

	for (int i = (int)filename.length();path[i] != '\\'&& path[i] != '/';i--)
		path[i] = 0;*/

	Material *material = NULL;

	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);

		string firstWord;
		newLine >> firstWord;

		if (firstWord == "newmtl")
		{
			material = new Material;

			materials.push_back(material);

			newLine >> material->name;
		}
		else if (firstWord == "illum")
		{
			newLine >> material->illum;
		}
		else if (firstWord == "Ka")
		{
			newLine >> material->Ka[0]>> material->Ka[1]>> material->Ka[2];
		}
		else if (firstWord == "Kd")
		{
			newLine >> material->Kd[0] >> material->Kd[1] >> material->Kd[2];
		}
		else if (firstWord == "Ks")
		{
			newLine >> material->Ks[0] >> material->Ks[1] >> material->Ks[2];
		}
		else if (firstWord == "K")
		{
			newLine >> material->Ke[0] >> material->Ke[1] >> material->Ke[2];
		}
		else if (firstWord == "Ns")
		{
			newLine >> material->shininess;
		}
		else if (firstWord == "d" || firstWord == "Tr")
		{
			newLine >> material->alpha;
		}
		else if ( firstWord == "Tf")
		{
			Vector3 color;
			newLine >> color.x>> color.y>> color.z;
			material->alpha = (color.x + color.y + color.z) / 3.0f;
		}
		else if (firstWord == "map_Ka")
		{
			string filename;
			newLine >> filename;
			//Texture *map = new Texture( string(path) + filename,filename);
			Texture *map = new Texture( filename, filename);
			material->ambientMap = map;
		}
		else if (firstWord == "map_Kd")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, filename);
			material->diffuseMap = map;
		}
		else if (firstWord == "map_Ks")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(filename, filename);
			material->specularMap = map;
		}
		else if (firstWord == "map_Ke")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, filename);
			material->emissionMap = map;
		}
		else if (firstWord == "map_Ns")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, filename);
			material->shininessMap = map;
		}
		else if (firstWord == "map_d")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture( filename, filename);
			material->transparencyMap = map;
		}
		else if (firstWord == "map_Bump")
		{
			string filename;
			newLine >> filename;
			Texture *map = new Texture(filename, filename);
			material->bumpMap = map;
		}
	}
}

float	Model::getRadius(void)
{
	return radius;
}
Vector3   Model::getCenter(void)
{
	return center;
}

string  Model::getPath(void)
{
	return filename;
}