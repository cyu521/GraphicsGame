#version 150 



in vec3 texCoord;  //get the interpolated texture location from the vertex shader

uniform samplerCube cubeMap;  //the texture unit to sample from

out vec4 fColor;


void main() 
{ 

	vec4 color1 = vec4(1,1,1,0); 
	 vec4 texColor = texture(cubeMap,texCoord);  //get the texture color at location textureCoord
  
    fColor = (color1)*texColor;
	
    fColor.a = 1.0;
} 

