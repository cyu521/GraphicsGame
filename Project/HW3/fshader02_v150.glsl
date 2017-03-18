#version 150 



in vec2 texCoord;  //get the interpolated texture location from the vertex shader

uniform sampler2D textureID;  //the texture unit to sample from
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL2;
in  vec3 fE;
in vec3 N;
in vec3 H;
in vec3 L;

out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 ModelMatrix;
uniform vec4 LightPosition1;
uniform float Enabled1;
uniform vec4 LightPosition2;
uniform float Shininess;

void main() 
{ 

	vec4 color1 = vec4(0,0,0,0); 
	
  if(Enabled1 == 1.0){
  //flashlight

	float dist = 0.5;
    
	vec4 ambient = AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd*DiffuseProduct;
    
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	vec4 specular = Ks*SpecularProduct;

	// discard the specular highlight if the light's behind the vertex
	if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	color1 = (ambient + diffuse + specular)*(1/pow(dist,2));
	}
	// Normalize the input lighting vectors
		vec3 N = normalize(fN);
		vec3 E = normalize(fE);
		vec3 L = normalize(fL2);

		vec3 H = normalize( L + E );
    
		vec4 ambient = AmbientProduct;

		float Kd = max(dot(L, N), 0.0);
		vec4 diffuse = Kd*DiffuseProduct;
    
		float Ks = pow(max(dot(N, H), 0.0), Shininess);
		vec4 specular = Ks*SpecularProduct;

		// discard the specular highlight if the light's behind the vertex
		if( dot(L, N) < 0.0 ) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
		}
		vec4 color2 = ambient + diffuse + specular;
    vec4 texColor = texture(textureID,texCoord);  //get the texture color at location textureCoord
    fColor = (color1+ color2)*texColor;
	
    fColor.a = 1.0;
} 

