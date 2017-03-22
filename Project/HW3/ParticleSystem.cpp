#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	program = InitShader("vShaderParticle.glsl", "fShaderParticle.glsl");

	modelMatrix = Translate(0, 1, -4);
	initializeParticles();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::initializeParticles(){
	for (int i = 0; i < NUM_PARTICLES; i++){
		particles[i].mass = 1.0;
		for (int j = 0; j < 3; j++){
			particles[i].color[j] = 1.0 * ((float)rand() / RAND_MAX);;
			particles[i].position[j] = 0;
			particles[i].velocity[j] = 10.0 * ((float)rand() / RAND_MAX) - 1.0;
		}

		particles[i].color.w = 1.0;
		particles[i].position.y = 30.0;
		particles[i].position.w = 1.0;
		particles[i].velocity.w = 0.0;
	}

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glUseProgram(program);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePoints) + sizeof(particlePoints), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor_loc = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor_loc);
	glVertexAttribPointer(vColor_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(particlePoints)));
}

float coef = .01; //how strong it bounces back
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
void ParticleSystem::draw(mat4 viewMatrix, mat4 projMatrix){
	glBindVertexArray(VAO);
	glUseProgram(program);

	for (int i = 0; i < NUM_PARTICLES; i++){
		particlePoints[i] = particles[i].position;
		particleColors[i] = particles[i].color;
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);
	GLuint model_loc = glGetUniformLocation(program, "ModelMatrix");
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelMatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "ModelView");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, viewMatrix);

	//GLuint viewMatrix_loc = glGetUniformLocation(program, "camera_matrix");
	//glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, viewMatrix);

	GLuint projection_loc = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projMatrix);

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
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
