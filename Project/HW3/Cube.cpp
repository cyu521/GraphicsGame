#include "Cube.h"


void Cube::quad(int aint, int bint, int cint, int dint, vec4 verticesList[]){
	vec4 ai = verticesList[aint];
	vec4 bi = verticesList[bint];
	vec4 ci = verticesList[cint];
	vec4 di = verticesList[dint];
	vec3 a = vec3(ai.x, ai.y, ai.z);
	vec3 b = vec3(bi.x, bi.y, bi.z);
	vec3 c = vec3(ci.x, ci.y, ci.z);
	vec3 d = vec3(di.x, di.y, di.z);

	vec3  normal = normalize(cross(b - a, c - b));

	normals[Index] = normal;  points[Index] = a;
	vertexTextureLocations[Index] = vec2(0, 0);
	Index++;
	normals[Index] = normal;  points[Index] = b;
	vertexTextureLocations[Index] = vec2(1, 0); Index++;
	normals[Index] = normal;  points[Index] = c;
	vertexTextureLocations[Index] = vec2(1, 1); Index++;

	normal = normalize(cross(d - c, a - d));


	normals[Index] = normal;  points[Index] = c;
	vertexTextureLocations[Index] = vec2(1, 1); Index++;
	normals[Index] = normal;  points[Index] = d;
	vertexTextureLocations[Index] = vec2(0, 1); Index++;
	normals[Index] = normal;  points[Index] = a;
	vertexTextureLocations[Index] = vec2(0, 0); Index++;


}

void Cube::buildCube(vec4 verticesList[]){
	quad(1, 0, 3, 2, verticesList);  //front
	quad(2, 3, 7, 6, verticesList);  //right
	quad(3, 0, 4, 7, verticesList);  //bottom
	quad(6, 5, 1, 2, verticesList);  //top
	quad(4, 5, 6, 7, verticesList);  //back
	quad(5, 4, 0, 1, verticesList);  //left
}

void Cube::build(vec4 verticesList[]){

	buildCube(verticesList);
	//now load the shader and get the location of it's variables
	program = InitShader("vshader02_v150.glsl", "fshader02_v150.glsl");
	glUseProgram(program);


	// Create and initialize the buffers
	glGenBuffers(3, VBOs);
	glGenVertexArrays(3, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(normals)+sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), sizeof(vertexTextureLocations), vertexTextureLocations);




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

	GLuint vTex = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTex);
	glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)+sizeof(normals)));

	glGenTextures(2, texture);
	int TextureSize = 512;
	int TextureSize2 = 512;
	GLubyte *image0 = ppmRead("crate_texture.ppm", &TextureSize, &TextureSize);
	GLubyte *image1 = ppmRead("brks.ppm", &TextureSize2, &TextureSize2);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);  //move the data onto the GPU

	//set the texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureSize2, TextureSize2, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);  //move the data onto the GPU
	
	//set the texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0);
	color4 light_ambient(0, 0, 0, 1.0);
	color4 light_diffuse(1.0, 0.8f, 0, 1.0);
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

void Cube::draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn){


	glUseProgram(program);
	glBindVertexArray(VAOs[0]);

	glUniform4fv(glGetUniformLocation(program, "LightPosition1"), 1, cameraEye);
	glUniform1f(glGetUniformLocation(program, "Enabled1"), isFlashLightOn);

	glUniform4fv(glGetUniformLocation(program, "LightPosition2"), 1, sunPosition);
	GLuint modelMatrix_loc = glGetUniformLocation(program, "ModelMatrix");
	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "ModelView");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, projMatrix);


	glBindTexture(GL_TEXTURE_2D, texture[textureType]);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);


	glUniform1i(glGetUniformLocation(program, "textureID"), 0);


	glDrawArrays(GL_TRIANGLES, 0, NumCubeVertices);
}
Cube::Cube(vec4 verticesList[])
{
	build(verticesList);
}



Cube::~Cube()
{
	glDeleteBuffers(3, VBOs);
	glDeleteVertexArrays(3, VAOs);
	glDeleteTextures(2, texture);
}
