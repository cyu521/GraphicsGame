#version 150 



in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec4 color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition1;
uniform float Enabled1;
uniform vec4 LightPosition2;

uniform float Shininess;

uniform mat4 ModelMatrix;
uniform mat4 Projection;

void main()
{


	vec4 color1 = vec4(0,0,0,0); 
	vec4 color2 = vec4(0,0,0,0); 

  if(Enabled1 == 1.0){
  
  //flashlight
  vec3 pos = (ModelView*ModelMatrix*vPosition).xyz;
  vec3 lightPosInCam = (ModelView*LightPosition1).xyz;

  vec3 L = normalize(lightPosInCam.xyz-pos);
  float dist = 0.5;
  vec3 E = normalize(-pos);
  vec3 N = normalize(ModelView*ModelMatrix*vec4(vNormal,0)).xyz;
  vec3 H = normalize(L+E);

	
    
	vec4 ambient = AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd*DiffuseProduct;
    
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	vec4 specular = Ks*SpecularProduct;

	// discard the specular highlight if the light's behind the vertex
	if( dot(L, N) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
	}
	color1 = (ambient + diffuse + specular);
  }
  
  
    vec3 fN = vNormal;
    vec3 fE = vPosition.xyz;
    vec3 fL = LightPosition2.xyz;

	if( LightPosition2.w != 0.0 ) {
		fL = LightPosition2.xyz - vPosition.xyz;
	}
	
		// Normalize the input lighting vectors
		vec3 N = normalize(fN);
		vec3 E = normalize(fE);
		vec3 L = normalize(fL);

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
		color2 = ambient + diffuse + specular;
	color = color1+color2;
	color.a = 1.0;
	

	

    gl_Position = Projection*ModelView*ModelMatrix*vPosition;

}
