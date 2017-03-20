// fragment shading of sphere model

#include "Angel.h"
#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"
#include <vector>
#include "SkyBox.h"
#include "ParticleSystem.h"
#include <iostream>
#include <sstream>

using namespace std;

void SpecialInput(int key, int x, int y);

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;
ParticleSystem particleSystem;

Camera* cameras[2];
Camera* currentCam;
bool isCamera1 = true;
Shape* shapes[2];
std::vector<Shape * > cubes;
void sunMove();
vec4 sunPosition(3,100, 4,0);
int angle = 0;
float isFlashLightOn = 0.0;

//current cube v,
vec4 currentCubeCV[8];

float last_time, present_time;
void idle(){
	float dt;
	present_time = glutGet(GLUT_ELAPSED_TIME);
	dt = 0.001*(present_time - last_time);

	Particl

	last_time = present_time;
	glutPostRedisplay();
}

double zPos = -6;
void sunMove(int test){
	angle+=3;
	double radian = angle * 3.14 / 180.0;
	sunPosition = vec4(cos(radian)*20, sin(radian)*-4, 0, 0);//rotate around the plan

	glutPostRedisplay();
	glutTimerFunc(250, sunMove, 0);
}
bool startRotate = false;
bool isRandomOn = false;
int randomSelectedNum = -1;
//Set cube vetices based on xpos
void setCubeV(double xPos){
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
	for (int i = 0; i < 8; i++) {
		currentCubeCV[i] = cv[i];
	}
}
//create the cubes
void initCubes(){

	//create 5 cubes, 1.5 x pos apart. at -2.5, -1, 0.5, 1.5, 3
	for (int i = 0; i < 5; i++){
		double xPos = -2.5 + i*1.5;
		setCubeV(xPos);

		string file = to_string(i + 1)+ ".ppm";

		cubes.push_back(new Cube(currentCubeCV, file));
	}
}
//clear the cubes and init 
void resetCubes(int value){

	for (int i = 0; i < cubes.size(); i++){
		cubes[i]->close();
	}
	cubes.clear();
	initCubes();
	startRotate=false;

	printf("Cubes are reset\n");
}
//only show the cube that is selected
void removeNotSelectedCube(){
	if (randomSelectedNum != -1){
		isRandomOn = false;
		Shape * selectedCube = cubes[randomSelectedNum];

		for (int i = 0; i < cubes.size(); i++){
			if (i != randomSelectedNum)
				cubes[i]->close();
		}
		cubes.clear();
		cubes.push_back(selectedCube);

		glutTimerFunc(3000, resetCubes, 0);
		randomSelectedNum = -1;
	}
}
//randomly select a cube and change textures and light
void randomSelect(int value){
	randomSelectedNum = rand() % cubes.size();
	printf("%d", randomSelectedNum);
	cubes.at((int) randomSelectedNum)->setTextureType();
}

void tryUsersInput(int cubeSelection){
	int cubeArraySelection = cubeSelection - 1; // Cube number is one higher than cube index

	if (randomSelectedNum == cubeSelection){
		removeNotSelectedCube();
		printf("Congratz, You Selected the correct Cube\n");
	}
	else{

		printf("Wrong Cube Selected,  Try again\n");
	}
}


//start moving the cubes up and down and start the game
void startGame(int value){
	if (startRotate){
		mat4 t1(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, zPos), vec4(0, 0, 0, 1));
		mat4 t2(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, -zPos), vec4(0, 0, 0, 1));
		for (int i = 0; i < cubes.size(); i++){
			double theta = ((double)rand());
			cubes[i]->setModelMatrix(t1*RotateX(theta)*t2);
		}

		glutPostRedisplay();
		glutTimerFunc(250, startGame, 0);
	}

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

	initCubes();


	currentCam = cameras[0] = new Camera();
	cameras[1] = new Camera();

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
	for (int i = 0; i < cubes.size(); i++){
		double theta = ((double)rand());
		cubes[i]->draw(mat4(view_matrix), mat4(proj), sunPosition, currentCam->eye, isFlashLightOn);
	}
	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	//TODO  add number key that matches randomSelectedNum
	Camera* cam = cameras[0];
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'a':
	case 'A':
		if (isCamera1)
			currentCam = cameras[1];
		else
			currentCam = cameras[0];
		isCamera1 = !isCamera1;
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
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
		tryUsersInput((int) key - 49); // -48 to get back key value rather than ASCII
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

		if (isCamera1)
			cam->moveUp();
		printf("up arrow pressed\n");
		break;
	case GLUT_KEY_RIGHT:

		if (isCamera1)
			cam->moveRight();
		printf("right arrow pressed\n");
		break;
	case GLUT_KEY_DOWN:
		if (isCamera1)
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

void mouse(GLint button, GLint state, GLint x, GLint y){
	//create object on mouse release
	if (GLUT_UP == state){
		startRotate = !startRotate;
		glutTimerFunc(250, startGame, 0);
		//we are rotating and we didn't start random Feature
		if (startRotate && !isRandomOn){
			isRandomOn = true;
			glutTimerFunc(5000, randomSelect, 0);
		}
	}
}
//----------------------------------------------------------------------------


void close(){
	shapes[0]->close();
	shapes[1]->close();

	for (int i = 0; i < cubes.size(); i++){
		cubes[i]->close();
	}
}
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

	glutCreateWindow("Graphics Final Project");

	//initialize glew if necessary (don't need to on Macs)
#ifndef __APPLE__
	GLenum err = glewInit();
#endif

	init();

	//glutTimerFunc(10, sunMove, 0);
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutSpecialFunc(SpecialInput);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutWMCloseFunc(close);
	glutMainLoop();
	return 0;
}
