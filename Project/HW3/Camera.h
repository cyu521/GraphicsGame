#pragma once
#include "Angel.h"  //includes gl.h, glut.h and other stuff...
class Camera
{
public:
	mat4 view_matrix;
	mat4 proj;
	vec4 eye;
	vec4 u, v, n;

	double fov, aspect;

	double t = 1.0;
	double r = 1.0;
	bool mPerspective = false; // start with ortho
	void switchPerspective(double width, double height);
	void draw();
	void pitchUp();
	void pitchDown();
	void pitch(double angle);
	void yawClock();
	void yawCounter();
	void yaw(double angle);
	void rollClock();
	void rollCounter();
	void roll(double angle);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	Camera();
	~Camera();
};

