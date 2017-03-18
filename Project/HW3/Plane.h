#pragma once
#include "Shape.h"
const int numPlaneVertices = 20*60;
class Plane :
	public Shape
{
protected:

	GLuint texture;
	vec4 points[numPlaneVertices];
	vec4 normals[numPlaneVertices];
	vec2 vertexTextureLocations[numPlaneVertices];
	int Index = 0;
public:
	Plane(vec4 v[]);
	~Plane();
	void draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn);
	void build(vec4 v[]);
	void triangle(vec4 ai, vec4 bi, vec4 ci, vec4 di);
	point4 unit(point4 p);
	void divide_triangle(vec4 a, vec4 b, vec4 c, int n);
	mat4 modelMatrix;
};

