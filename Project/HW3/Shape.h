#pragma once
#include "Angel.h"
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
class Shape
{
protected:
	GLuint program;
	GLuint VAOs[1];
	GLuint VBOs[1];
	mat4 modelmatrix;
	int textureType = 0;
public:

	virtual void draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn) = 0;
	mat4 getModelMatrix(){ return modelmatrix; }
	void setModelMatrix(mat4 modelMatrix) { modelmatrix = modelMatrix; }

	void setTextureType(){ 
		if (textureType == 0)
			textureType = 1;
		else
			textureType = 0;
	};
	static unsigned char* ppmRead(char* filename, int* width, int* height);
};

