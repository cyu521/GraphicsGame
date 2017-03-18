#include "SkyBox.h"


void SkyBox::quad(int aint, int bint, int cint, int dint, vec4 verticesList[]){
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

void SkyBox::buildSkyBox(vec4 verticesList[]){
	quad(1, 0, 3, 2, verticesList);  //front
	quad(2, 3, 7, 6, verticesList);  //right
	quad(3, 0, 4, 7, verticesList);  //bottom
	quad(6, 5, 1, 2, verticesList);  //top
	quad(4, 5, 6, 7, verticesList);  //back
	quad(5, 4, 0, 1, verticesList);  //left
}

void SkyBox::build(vec4 verticesList[]){

	buildSkyBox(verticesList);
	//now load the shader and get the location of it's variables
	program = InitShader("vshader03_v150.glsl", "fshader03_v150.glsl");
	glUseProgram(program);


	// Create and initialize the buffers
	glGenBuffers(3, VBOs);
	glGenVertexArrays(3, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(normals)+sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(normals), sizeof(vertexTextureLocations), vertexTextureLocations);




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

	glGenTextures(2, texture);
	int TextureSize = 512;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLubyte * skyTop = ppmRead("sky-top.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTop);
	GLubyte * skyBottom = ppmRead("sky-bottom.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBottom);
	GLubyte * skyRight = ppmRead("sky-right.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyRight);
	GLubyte * skyLeft = ppmRead("sky-left.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyLeft);
	GLubyte * skyFront = ppmRead("sky-front.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyFront);
	GLubyte * skyBack = ppmRead("sky-back.ppm", &TextureSize, &TextureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, TextureSize, TextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBack);

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

void SkyBox::draw(mat4 viewMatrix, mat4 projMatrix, vec4 sunPosition, vec4 cameraEye, float isFlashLightOn){

	vec4 skyboxEye = vec4(0, 0, 0, 1);

	glUseProgram(program);
	glBindVertexArray(VAOs[0]);

	glUniform4fv(glGetUniformLocation(program, "LightPosition1"), 1, cameraEye);
	glUniform1f(glGetUniformLocation(program, "Enabled1"), isFlashLightOn);

	GLuint modelMatrix_loc = glGetUniformLocation(program, "ModelMatrix");
	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "ModelView");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, projMatrix);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
	GLuint mapPos = glGetUniformLocation(program, "cubeMap");
	glUniform1i(mapPos, 0);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDrawArrays(GL_TRIANGLES, 0, NumSkyVertices);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
SkyBox::SkyBox(vec4 verticesList[])
{
	build(verticesList);
}



SkyBox::~SkyBox()
{
	glDeleteBuffers(3, VBOs);
	glDeleteVertexArrays(3, VAOs);
	glDeleteTextures(2, texture);
}
