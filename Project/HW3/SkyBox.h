#pragma once
#include "Shape.h"
const int NumSkyVertices = 6 * 2 * 3;
class SkyBox :
	public Shape
{
protected:
	GLuint texture[2];
	vec4 points[NumSkyVertices];
	vec4 normals[NumSkyVertices];
	vec2 vertexTextureLocations[NumSkyVertices];
	int Index = 0;
public:
	SkyBox(vec4 verticesList[]);
	void buildSkyBox(vec4 verticesList[]);
	~SkyBox();
	void draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn);
	void build(vec4 verticesList[]);
	void quad(int a, int b, int c, int d, vec4 verticesList[]);
	mat4 modelMatrix;
};

