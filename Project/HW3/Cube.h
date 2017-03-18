#pragma once
#include "Shape.h"

using namespace std;

const int NumCubeVertices = 6 * 2 * 3;
class Cube :
	public Shape
{
protected:
	GLuint texture[2];
	vec4 points[NumCubeVertices];
	vec4 normals[NumCubeVertices];
	vec2 vertexTextureLocations[NumCubeVertices];
	int Index = 0;
	string textureFile;
public:
	Cube(vec4 verticesList[], string initTextureFile);
	void buildCube(vec4 verticesList[]);
	~Cube();
	void draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn);
	void build(vec4 verticesList[]);
	void quad(int a, int b, int c, int d, vec4 verticesList[]);
	mat4 modelMatrix;
};

