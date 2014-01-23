#pragma once

#include <glm/glm.hpp>
#include <string>
#include <iostream>
using namespace std;
const int g_maxSceneObjects = 1000;
const int g_maxMeshes = 300;
const int g_maxCameras = 15;
const int g_maxLights = 10;

const int g_maxVerticesPerMesh = 30000;
const int g_maxNameLength = 30;
const int g_shortMaxNameLength = 15;
const int g_maxPathLength = 150;
const int g_maxNrOfFlags = 2;
const int g_maxLocators = 20;
const string g_savepath = "C:/Users/BTH/Documents/Output/Assets/";

const string g_levelName = "myTestLevel";

const string g_textureR = "RED";
const string g_textureG = "GREEN";
const string g_textureB = "BLUE";

const int g_maxMessages = 30;
//textureBawlsheit
const int g_MaxResolution = 512 * 512 *4;
const int g_tileFactor = 4;
const int g_maxPaintTextures = 2;

struct UpdateMessage
{
	char name[g_shortMaxNameLength];
	int updateID, removeID;

	bool updateTransform;
	bool updateShape;

	UpdateMessage()
	{
		updateID = -1;
		removeID = -1;
		updateTransform = false;
		updateShape = false;
	}
};

struct Transform
{
	int index;
	char name[g_maxNameLength];
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
	glm::vec3 rotPivot;
	glm::vec3 scalePivot;
	glm::mat4x4 transform;
	char flags[g_maxNrOfFlags][g_shortMaxNameLength];
	int nrOfFlags;
};

struct Mesh
{
	Transform transformation;
	int nrOfVertices;
	glm::vec3 vertex[g_maxVerticesPerMesh];
	glm::vec2 UV[g_maxVerticesPerMesh];
	glm::vec3 normal[g_maxVerticesPerMesh];
	/*char texturePath[100];
	char normalPath[100];*/
	int MaterialID; //Added so that a mesh can get a material on 3d viewers side.
	//int indices[g_maxVerticesPerMesh*3];
	char materialName[g_maxNameLength];
	char modelName[g_maxNameLength];
	int paintIndex;	//ska sätta om -1 så har den ingen.

	Mesh()
	{
		paintIndex = -1;
	}
};

struct Material
{
	char materialName[g_maxNameLength];
	char texturePath[g_maxPathLength];
	char normalPath[g_maxPathLength];
	char specularPath[g_maxPathLength];
};

struct Camera
{
	Transform transformation;

	float verticalFieldOfView;
	float horizontalFieldOfView;
	float aspectRatio;
	float nearClippingPlane;
	float farClippingPlane;

	glm::vec3 upDirection;
	glm::vec3 rightDirection;
	glm::vec3 viewDirection;

	glm::vec3 eyePoint;

};

struct Light
{
	char LightType[g_maxNameLength];
	Transform transformation;
	glm::vec4 color;
	glm::vec3 direction;
	float Intensity;
};

struct Locator
{
	Transform transformation;
};

struct PaintTexture
{
	unsigned char Pixels[g_MaxResolution];
	int heigth, width;
	int tileFactor;

	char textureRed[g_maxNameLength];
	char textureGreen[g_maxNameLength];
	char textureBlue[g_maxNameLength];

	PaintTexture()
	{
		tileFactor = g_tileFactor;
		memcpy(textureRed, g_textureR.c_str(), g_maxNameLength);
		memcpy(textureGreen, g_textureG.c_str(), g_maxNameLength);
		memcpy(textureBlue, g_textureB.c_str(), g_maxNameLength);
	}
};