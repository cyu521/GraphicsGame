#version 150 



in vec2 vTexCoord; 
out vec2 texCoord;  //pass through texture location to fragment shader
in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL2;
out  vec3 N;
out  vec3 H;
out  vec3 L;

uniform vec4 LightPosition1;
uniform vec4 LightPosition2;

uniform mat4 ModelMatrix;
uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
  
    fN = vNormal;
    fE = vPosition.xyz;
	
	  vec3 pos = (ModelView*ModelMatrix*vPosition).xyz;
	  vec3 lightPosInCam = (ModelView*LightPosition1).xyz;

	   L = normalize(lightPosInCam.xyz-pos);
	  float dist = 0.5;
	  vec3 E = normalize(vec3(0,0,0)-pos);
	   N = normalize(ModelView*ModelMatrix*vec4(vNormal,0)).xyz;
	   H = normalize(L+E);	

	fL2 = LightPosition2.xyz;
	
	if( LightPosition2.w != 0.0 ) {
		fL2 = LightPosition2.xyz - vPosition.xyz;
	}
    gl_Position = Projection*ModelView*ModelMatrix*vPosition;
	texCoord = vTexCoord; //pass through texture location to fragment shader
}
