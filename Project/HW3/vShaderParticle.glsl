#version 150 



in vec4 vColor;
in vec4 vPosition;

out  vec4 color;

uniform mat4 ModelMatrix;
uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection*ModelView*ModelMatrix*vPosition;
  color = vColor; //pass through texture location to fragment shader
}
