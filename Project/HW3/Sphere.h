#pragma once
#include "Shape.h"
const int numSphereVertices = 1024 * 3 * 4;
class Sphere :
	public Shape
{
protected:
	vec4 points[numSphereVertices];
	vec4 normals[numSphereVertices];
	int Index = 0;
	int NumTimesToSubdivide = 5;

public:

	Sphere(vec4 verticesList[]);
	~Sphere();

	void draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn);
	void build(vec4 verticesList[]);

	void triangle(vec4 a, vec4 b, vec4 c);
	point4 unit(point4 p);
	void divide_triangle(vec4 a, vec4 b, vec4 c, int n);
	void tetrahedron(int count, vec4 v[]);

	mat4 modelMatrix;

};

