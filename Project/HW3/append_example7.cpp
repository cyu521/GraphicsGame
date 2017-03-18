// fragment shading of sphere model

#include "Angel.h"
#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"
#include "SkyBox.h"
void SpecialInput(int key, int x, int y);

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;
Camera* cameras[2];
Camera* currentCam;
bool isCamera1 = true;
Shape* shapes[7];
void sunMove();
vec4 sunPosition(1,0,0,0);
int angle = 0;
float isFlashLightOn = 0.0;



void sunMove(int test){
	angle+=3;
	double radian = angle * 3.14 / 180.0;
	sunPosition = vec4(cos(radian)*20, sin(radian)*-4, 0, 0);//rotate around the plan

	mat4 t1(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, -6), vec4(0, 0, 0, 1));
	mat4 t2(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 6), vec4(0, 0, 0, 1));

	double theta = ((double)rand());
	shapes[2]->setModelMatrix(t1*RotateX(theta)*t2);
	theta = ((double)rand());
	shapes[3]->setModelMatrix(t1*RotateX(theta)*t2);

	theta = ((double)rand());
	shapes[4]->setModelMatrix(t1*RotateX(theta)*t2);
	theta = ((double)rand());
	shapes[5]->setModelMatrix(t1*RotateX(theta)*t2);
	theta = ((double)rand());
	shapes[6]->setModelMatrix(t1*RotateX(theta)*t2);

	glutPostRedisplay();
	glutTimerFunc(250, sunMove, 0);
}

void
init()
{
/*
	point4 v[4] = {
		vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0)
	};
	shapes[0] = new Sphere(v);
	*/

	double dist = 50;
	vec4 sb[] = {
		vec4(dist, -dist, dist, 1.0),
		vec4(dist, dist, dist, 1.0),
		vec4(-dist, dist, dist, 1.0),
		vec4(-dist, -dist, dist, 1.0),
		vec4(dist, -dist, -dist, 1.0),
		vec4(dist, dist, -dist, 1.0),
		vec4(-dist, dist, -dist, 1.0),
		vec4(-dist, -dist, -dist, 1.0)

	};

	shapes[0] = new SkyBox(sb);

	vec4 pv[] = {

		vec4(0, -3, 0, 1),			//origin
		vec4(-20, -3, 0, 1),		//left center
		vec4(-20, -3, -20, 1),		//left far corner
		vec4(0, -3, -20, 1),		//center far
		//0,1,2   2,3,0  3rd quad
		vec4(20, -3, -20, 1),	//right far corner
		vec4(20,-3,0,1),		//right center
		//0,3,4    4,5,0
		vec4(20, -3, 20, 1), //right close corner
		vec4(0,-3,20,1),		//center close
		//0,5,6  6,7,0
		vec4(-20, -3, 20, 1)	//8 left close corner
		//0,7,8   8,1,0
	};
	shapes[1] = new Plane(pv);

	double xPos = -2;
	double zPos = -6;
	vec4 cv[] = {
		vec4(xPos, -0.5, zPos, 1.0),
		vec4(xPos, 0.5, zPos, 1.0),
		vec4(xPos - 1, 0.5, zPos, 1.0),
		vec4(xPos - 1, -0.5, zPos, 1.0),
		vec4(xPos, -0.5, zPos - 1, 1.0),
		vec4(xPos, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, -0.5, zPos - 1, 1.0)

	};

	shapes[2] = new Cube(cv);

	xPos = -0.5;
	vec4 cv1[] = {
		vec4(xPos, -0.5, zPos, 1.0),
		vec4(xPos, 0.5, zPos, 1.0),
		vec4(xPos - 1, 0.5, zPos, 1.0),
		vec4(xPos - 1, -0.5, zPos, 1.0),
		vec4(xPos, -0.5, zPos-1, 1.0),
		vec4(xPos, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, -0.5, zPos - 1, 1.0)

	};

	shapes[3] = new Cube(cv1);
	xPos = 1;
	vec4 cv2[] = {
		vec4(xPos, -0.5, zPos, 1.0),
		vec4(xPos, 0.5, zPos, 1.0),
		vec4(xPos - 1, 0.5, zPos, 1.0),
		vec4(xPos - 1, -0.5, zPos, 1.0),
		vec4(xPos, -0.5, zPos - 1, 1.0),
		vec4(xPos, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, -0.5, zPos - 1, 1.0)

	};

	shapes[4] = new Cube(cv2);


	xPos = 2.5;
	vec4 cv3[] = {
		vec4(xPos, -0.5, zPos, 1.0),
		vec4(xPos, 0.5, zPos, 1.0),
		vec4(xPos - 1, 0.5, zPos, 1.0),
		vec4(xPos - 1, -0.5, zPos, 1.0),
		vec4(xPos, -0.5, zPos - 1, 1.0),
		vec4(xPos, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, -0.5, zPos - 1, 1.0)

	};

	shapes[5] = new Cube(cv3);


	xPos = 4;
	vec4 cv4[] = {
		vec4(xPos, -0.5, zPos, 1.0),
		vec4(xPos, 0.5, zPos, 1.0),
		vec4(xPos - 1, 0.5, zPos, 1.0),
		vec4(xPos - 1, -0.5, zPos, 1.0),
		vec4(xPos, -0.5, zPos - 1, 1.0),
		vec4(xPos, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, 0.5, zPos - 1, 1.0),
		vec4(xPos - 1, -0.5, zPos - 1, 1.0)

	};

	shapes[6] = new Cube(cv4);

	currentCam = cameras[0] = new Camera();


}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);

	currentCam->draw();

	mat4 view_matrix = currentCam->view_matrix;
	mat4 proj = currentCam->proj;

	vec4 eye = currentCam->eye;
	vec4 n = currentCam->n;
	vec4 v = currentCam->v;

	mat4 skyBoxViewMatrix = LookAt(vec4(0,0,0,1), -n, v);
	shapes[0]->draw(skyBoxViewMatrix, mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[1]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[2]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[3]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[4]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[5]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	shapes[6]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	Camera* cam = cameras[0];
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 040: //space
		if (isFlashLightOn == 0.0)
			isFlashLightOn = 1.0;
		else
			isFlashLightOn = 0.0;
		printf("space");
		break;
	case 'X'://pitch Up
		if (isCamera1)
			cam->pitchUp();
		printf("X pressed\n");
		break;
	case 'x':
		if (isCamera1)
			cam->pitchDown();
		printf("x pressed\n");
		break;
	case 'C'://clockwise un plane

		if (isCamera1)
			cam->yawClock();
		printf("C pressed\n");
		break;
	case 'c'://counter

		if (isCamera1)
			cam->yawCounter();
		printf("c pressed\n");
		break;
	case 'Z'://clockwise uv plane

		if (isCamera1)
			cam->rollClock();
		printf("Z pressed\n");
		break;
	case 'z'://counter

		if (isCamera1)
			cam->rollCounter();
		printf("z pressed\n");
		break;
	case 't':
	case'T':
		shapes[2]->setTextureType();
		break;
	}
	glutPostRedisplay();
}
void SpecialInput(int key, int x, int y)
{
	Camera* cam = currentCam;
	switch (key)
	{
	case GLUT_KEY_LEFT:

		if (isCamera1)
			cam->moveLeft();
		printf("left arrow pressed\n");

		break;
	case GLUT_KEY_UP:

			cam->moveUp();
		printf("up arrow pressed\n");
		break;
	case GLUT_KEY_RIGHT:

		if (isCamera1)
			cam->moveRight();
		printf("right arrow pressed\n");
		break;
	case GLUT_KEY_DOWN:
			cam->moveDown();
		printf("down arrow pressed\n");
		break;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = -20.0, zFar = 20.0;

	GLfloat aspect = GLfloat(width) / height;
	GLfloat fov = 65.0;

	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		top /= aspect;
		bottom /= aspect;
	}
	if (isCamera1)
		currentCam->proj = Perspective(fov, aspect, 1.0, 100);
	glutPostRedisplay();
}
bool rayTriangleIntersect(
	const vec4 &orig4, const vec4 &dir4,
	const vec4 &v04, const vec4 &v14, const vec4 &v24,
	float &t, float &u, float &v)
{
	vec3 orig(orig4.x, orig4.y, orig4.z);
	vec3 dir(dir4.x, dir4.y, dir4.z);
	vec3 v0(v04.x, v04.y, v04.z);
	vec3 v1(v14.x, v14.y, v14.z);
	vec3 v2(v24.x, v24.y, v24.z);
	// compute plane's normal
	vec3 v0v1 = v1 - v0;
	vec3 v0v2 = v2 - v0;
	// no need to normalize
	vec3 N = cross(v0v1, v0v2); // N 
	float denom = dot(N, N);

	// Step 1: finding P

	// check if ray and plane are parallel ?
	float NdotRayDirection = dot(N, dir); 
	if (fabs(NdotRayDirection) < 1e-8) // almost 0 
		return false; // they are parallel so they don't intersect ! 

	// compute d parameter using equation 2
	float d = -dot(N, v0); 

	// compute t (equation 3)
	t = (dot(N, orig)  +d) / NdotRayDirection;
	// check if the triangle is in behind the ray
	if (t < 0) return false; // the triangle is behind 

	// compute the intersection point using equation 1
	vec3 P = orig + t * dir;

	// Step 2: inside-outside test
	vec3 C; // vector perpendicular to triangle's plane 

	// edge 0
	vec3 edge0 = v1 - v0;
	vec3 vp0 = P - v0;
	C = cross(edge0, vp0);
	if (dot(N, C) < 0) return false; // P is on the right side 

	// edge 1
	vec3 edge1 = v2 - v1;
	vec3 vp1 = P - v1;
	C = cross(edge1, vp1);
	if ((u = dot(N, C)) < 0)  return false; // P is on the right side 

	// edge 2
	vec3 edge2 = v0 - v2;
	vec3 vp2 = P - v2;
	C = cross(edge2, vp2);
	if ((v = dot(N, C)) < 0) return false; // P is on the right side; 

	u /= denom;
	v /= denom;

	return true; // this ray hits the triangle 
}

void mouse(GLint button, GLint state, GLint x, GLint y){
	//create object on mouse release
	if (GLUT_UP == state){
		double xpos = 2.0 * double(x / 512.0) - 1.0;
		double ypos = 1 - 2.0 * double(y / 512.0);
		glutPostRedisplay();
	}
}
//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{


	glutInit(&argc, argv);	//initialize glut
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#endif
	glutInitWindowSize(512, 512);	//set window size to be 512x512 pixels

	glutCreateWindow("Sphere");

	//initialize glew if necessary (don't need to on Macs)
#ifndef __APPLE__
	GLenum err = glewInit();
#endif

	init();

	glutTimerFunc(10, sunMove, 0);
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutSpecialFunc(SpecialInput);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}
