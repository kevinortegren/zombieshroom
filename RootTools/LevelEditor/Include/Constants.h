#pragma once

#include <glm/glm.hpp>
#include <string>
#include <iostream>
using namespace std;
const int g_maxSceneObjects = 3000;
const int g_maxMeshes = 400;
const int g_maxCameras = 15;
const int g_maxLights = 40;

const int g_maxVerticesPerMesh = 30000;
const int g_maxNameLength = 30;
const int g_shortMaxNameLength = 15;
const int g_maxPathLength = 150;
const int g_maxNrOfFlags = 2;
const int g_maxLocators = 50;
const string g_savepath = "C:/Users/BTH/Documents/Output/Assets/"; 
const string g_textureLookPath = "C:/ZombieShroom/LevelEditor/Assets/Textures/";

const string g_levelName = "CaveTestV1";

const string g_textureR = "RED";
const string g_textureG = "GREEN";
const string g_textureB = "BLUE";

const string emptyString = "";

const int g_maxMessages = g_maxMeshes;
//textureBawlsheit
const int g_MaxResolution = 512 * 512 *4;
const int g_tileFactor = 24;
const int g_maxPaintTextures = 10;

const int g_maxMegaMeshes = 2;
const int g_maxVerticesPerMegaMesh = 100000;

const string g_GrassDiffuse = "grass";
const string g_GrassTranslucency = "grass_translucency";
const string g_GrassBillboard = "grass_billboard";
const float g_Ambient = 0.0f;

struct UpdateMessage
{
	char name[g_maxNameLength];
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

struct Flags
{
	bool _Transparent, _Static, _SpawnPoint, _Particle, _Water, _Hazard, _AbilitySpawnPoint, _NoRender, _Grass;
	int _PaintStatus; // -1: False, 0: Painting, 1: Painted

	Flags()
	{
		_Transparent = false;
		_Static = false;
		_SpawnPoint = false;
		_Particle = false;
		_Water = false;
		_Hazard = false;
		_AbilitySpawnPoint = false;
		_NoRender = false;
		_Grass = false;
		_PaintStatus = -1;
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
	Flags flags;

	Transform()
	{
		index = -1;
		position = glm::vec3(0,0,0);
		rotation = glm::vec4(0,0,0,0);
		scale = glm::vec3(0,0,0);
		rotPivot = glm::vec3(0,0,0);
		scalePivot = glm::vec3(0,0,0);
	}
};

struct Mesh
{
	Transform transformation;
	int nrOfVertices;
	glm::vec3 vertex[g_maxVerticesPerMesh];
	glm::vec2 UV[g_maxVerticesPerMesh];
	glm::vec3 normal[g_maxVerticesPerMesh];
	glm::vec3 tangent[g_maxVerticesPerMesh];
	glm::vec3 binormal[g_maxVerticesPerMesh];
	int MaterialID; //Added so that a mesh can get a material on 3d viewers side.
	char materialName[g_maxNameLength];
	char modelName[g_maxNameLength];
	int paintIndex;	//ska s�tta om -1 s� har den ingen.

	Mesh()
	{
		paintIndex = -1;
		MaterialID = -1;
		nrOfVertices = 0;
	}
};

struct MegaMesh
{
	Transform transformation;
	int nrOfVertices;
	glm::vec3 vertex[g_maxVerticesPerMegaMesh];
	glm::vec2 UV[g_maxVerticesPerMegaMesh];
	glm::vec3 normal[g_maxVerticesPerMegaMesh];
	glm::vec3 tangent[g_maxVerticesPerMegaMesh];
	glm::vec3 binormal[g_maxVerticesPerMegaMesh];
	int MaterialID; //Added so that a mesh can get a material on 3d viewers side.
	char materialName[g_maxNameLength];
	char modelName[g_maxNameLength];
	int paintIndex;	//ska s�tta om -1 s� har den ingen.

	MegaMesh()
	{
		paintIndex = -1;
		MaterialID = -1;
		nrOfVertices = 0;
	}
};

struct Material
{
	char materialName[g_maxNameLength];
	char texturePath[g_maxPathLength];
	char normalPath[g_maxPathLength];
	char specularPath[g_maxPathLength];
	char glowPath[g_maxPathLength];
	char translucencePath[g_maxPathLength];
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
	//glm::vec3 direction;
	float Intensity;

	Light()
	{
		memcpy(LightType, emptyString.c_str(), g_maxNameLength);
		color = glm::vec4(0,0,0,0);
		//glm::vec3 direction;
		Intensity = 0.0f;
	}
};

struct WorldData
{
	Light AmbientLight;
	Light DirectionalSun;
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

struct Counters
{
	int NumberOfMeshes;
	int NumberOfLights;
	int NumberOfCameras;
	int NumberOfMaterials;
	int NumberOfLocators;

	int NumberOfMessages;
	int NumberOfPaintTextures;
	int export;

	Counters()
	{
		NumberOfMeshes = 0;
		NumberOfLights = 0;
		NumberOfCameras = 0;
		NumberOfMaterials = 0;
		NumberOfLocators = 0;
		NumberOfMessages = 0;
		NumberOfPaintTextures = 0;
		export = 0;
	}
};