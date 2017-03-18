#include "Camera.h"


Camera::Camera()
{

	vec4 at = vec4(0, 3, -4, 1);		//translate
	vec4 eye = vec4(0.0, 2.0, 2.0, 1.0);			// (0,2,0)
	vec4   up = vec4(0.0, 1.0, 0.0, 1.0);			//y axis

	 n = normalize(eye - at);
	 u = normalize(cross(up, n));
	 v = up;


	 view_matrix = LookAt(eye, at, up);

	  fov = 65.0;
	  aspect = 512 / 512;
	 proj = Perspective(fov, aspect, 1.0, 100);



}

void Camera::draw(){
	vec4 at = eye - n;
	vec4 up = v;
	view_matrix = LookAt(eye, at, up);
}

void Camera::pitchUp(){
	pitch(3);
}
void Camera::pitchDown(){
	pitch(-3);

}
void Camera::pitch(double angle){
	double a = angle*  3.14 / 180;
	u = normalize(cross(v, n));
	n = sin(a)*v + cos(a)*n;
	v = cos(a)*v - sin(a)*n;


}
void Camera::yawClock(){
	yaw(3);
}
void Camera::yawCounter(){
	yaw(-3);

}
void Camera::yaw(double angle){
	double a = angle*  3.14 / 180;
	v = normalize(cross(n, u));
	n= -sin(a)*u + cos(a)*n;
	u = cos(a)*u + sin(a)*n;
}

void Camera::rollClock(){
	roll(3);
}
void Camera::rollCounter(){
	roll(-3);

}
void Camera::roll(double angle){
	double a = angle*  3.14 / 180.0;
	u = cos(a)*u - sin(a)*v;
	v = sin(a)*u + cos(a)*v;
}

void Camera::moveUp(){
	eye -= n;
}
void Camera::moveDown(){
	eye += n;
}
void Camera::moveLeft(){
	eye -= u;
}
void Camera::moveRight(){
	eye += u;
}
Camera::~Camera()
{
}
