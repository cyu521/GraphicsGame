#pragma once
#include "Angel.h";
const int NUM_PARTICLES = 1000;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	struct particle{
		vec4 color;
		vec4 position;
		vec4 velocity;
		float mass;
	};
	mat4 modelMatrix;
	GLuint VAO;
	GLuint VBO;
	GLuint program;
	particle particles[NUM_PARTICLES];
	vec4 particlePoints[NUM_PARTICLES];
	vec4 particleColors[NUM_PARTICLES];
	void initializeParticles();
	void updateParticles(float dt);
	void collision(int n);
	void draw(mat4 viewMatrix, mat4 projMatrix);
	void drawParticles();
};

