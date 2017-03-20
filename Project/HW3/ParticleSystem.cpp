#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
	initializeParticles();
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::initializeParticles(){
	for (int i = 0; i < NUM_PARTICLES; i++){
		particles[i].mass = 1.0;
		for (int j = 0; j < 3; j++){
			particles[i].color[j] = ((float)rand() / RAND_MAX) - 1.0;
			particles[i].position[j] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
			particles[i].velocity[j] = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
		}
		particles[i].color.w = 1.0;
		particles[i].position.w = 1.0;
		particles[i].velocity.w = 0.0;
	}
}

float coef; //how strong it bounces back
void ParticleSystem::collision(int n){
	for (int i = 0; i<3; i++){
		if (particles[n].position[i] >= 1.0){
			particles[n].velocity[i] *= -coef;
			particles[n].position[i] = 1.0 - coef*(particles[n].position[i] - 1.0);
		}
		if (particles[n].position[i] <= -1.0){
			particles[n].velocity[i] *= -coef;
			particles[n].position[i] = -1.0 - coef*(particles[n].position[i] + 1.0);
		}
	}
}

GLuint VBO;
void ParticleSystem::draw(){
	glBindVertexArray(VAO);
	glUseProgram(program);

	for (int i = 0; i < NUM_PARTICLES; i++){
		particlePoints[i] = particles[i].position;
		particlePoints[i] = particles[i].color;
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);

	GLunit model_loc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_matrix);

	GLunit camera_loc = glGetUniformLocation(program, "camera_matrix");
	glUniformMatrix4fv(camera_loc, 1, GL_TRUE, camera_matrix);

	GLunit projection_loc = glGetUniformLocation(program, "projection_matrix");
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection_matrix);

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES)
}


float forces(int i, int j){
	if (j == 1) //only affect y direction
		return -1;
	else
		return 0;
}
void ParticleSystem::updateParticles(float dt){
	for (int i = 0; i < NUM_PARTICLES; i++){
		for (int j = 0; j<3; j++){
			particles[i].position[j] += dt*particles[i].velocity[j];
			particles[i].velocity[j] += dt*forces(i, j) / particles[i].mass;
		}
		collision(i);
	}
}
