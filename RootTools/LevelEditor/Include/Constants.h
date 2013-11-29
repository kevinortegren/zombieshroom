#pragma once

#include <glm/glm.hpp>
#include <string>
#include <iostream>
using namespace std;
const int g_maxSceneObjects = 1000;
const int g_maxMeshes = 50;
const int g_maxCameras = 15;
const int g_maxLights = 10;

const int g_maxVerticesPerMesh = 10000;

struct Transform
{
	char name[30];
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct Mesh
{
	Transform transformation;
	int nrOfVertices;
	glm::vec3 vertex[g_maxVerticesPerMesh];
	glm::vec2 UV[g_maxVerticesPerMesh];
	glm::vec3 normal[g_maxVerticesPerMesh];
	string texturePath;
	string normalPath;
	int indices[g_maxVerticesPerMesh*3];
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
	Transform transformation;
};

