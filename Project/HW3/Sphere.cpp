#include "Sphere.h"


void Sphere::triangle(vec4 ai, vec4 bi, vec4 ci){
	vec3 a = vec3(ai.x, ai.y, ai.z);
	vec3 b = vec3(bi.x, bi.y, bi.z);
	vec3 c = vec3(ci.x, ci.y, ci.z);

	vec3  normal = normalize(cross(b - a, c - b));

	normals[Index] = normal;  points[Index] = a;  Index++;
	normals[Index] = normal;  points[Index] = b;  Index++;
	normals[Index] = normal;  points[Index] = c;  Index++;

}
point4 Sphere::unit(point4 p){
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}
void Sphere::divide_triangle(vec4 a, vec4 b, vec4 c, int count){
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}
void Sphere::tetrahedron(int count, vec4 v[]){

	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}

void Sphere::build(vec4 verticesList[]){
	tetrahedron(NumTimesToSubdivide, verticesList);

	//now load the shader and get the location of it's variables
	program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	glUseProgram(program);

	// Create and initialize the buffers
	glGenBuffers(3, VBOs);
	glGenVertexArrays(3, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);




	GLuint modelMatrix_loc = glGetUniformLocation(program, "ModelMatrix");
	mat4 translate = Translate(0, 1, -4);
	setModelMatrix(translate);
	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelMatrix);


	//------------------------------------------------------------

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0);
	color4 light_ambient(1.0, 0.8f, 0, 1.0);
	color4 light_diffuse(0.5f, 0.2f, 0, 1.0);
	color4 light_specular(1.0, 1, 1, 1.0);

	color4 material_ambient(0, 0, 0.0, 1.0);
	color4 material_diffuse(1.0, 0.8f, 0, 1.0);
	color4 material_specular(1.0, 1, 1, 1.0);
	float  material_shininess = 100;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition1"),
		1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	glEnable(GL_DEPTH_TEST);


	//define color to use when color buffer is cleared
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Sphere::draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn){


	glUseProgram(program);
	glBindVertexArray(VAOs[0]);
	int test = 1;
	glUniform4fv(glGetUniformLocation(program, "LightPosition1"), 1, cameraEye);
	glUniform1f(glGetUniformLocation(program, "Enabled1"), isFlashLightOn);

	glUniform4fv(glGetUniformLocation(program, "LightPosition2"), 1, sunPosition);

	GLuint modelMatrix_loc = glGetUniformLocation(program, "ModelMatrix");
	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "ModelView");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, projMatrix);



	glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
}

Sphere::Sphere(vec4 verticesList[])
{
	build(verticesList);
}


Sphere::~Sphere()
{
	glDeleteBuffers(3, VBOs);
	glDeleteVertexArrays(3, VAOs);
}
