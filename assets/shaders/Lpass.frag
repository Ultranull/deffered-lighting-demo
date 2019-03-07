#version 420 core
out vec4 color;

in vec2 uv;


uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedospec;

struct Light {
	vec3 pos;
	vec3 color;
	float quad;
};

const int NUM_LIGHTS=100;
uniform Light lights[NUM_LIGHTS];

uniform vec3 veiwPos;

void main(){
	vec3 fragpos=texture2D(position,uv).xyz;
	vec3 normal=texture2D(normal,uv).xyz;
	vec3 diffuse=texture2D(albedospec,uv).xyz;

	vec3 lighting=diffuse *.001;
	for(int i=0;i<NUM_LIGHTS;i++){
		vec3 lightdir=normalize(vec3(lights[i].pos)-fragpos);
		vec3 diff= max(dot(normal,lightdir),0)*diffuse*vec3(lights[i].color);

		float dist=length(vec3(lights[i].pos)-fragpos);
		lighting+=diff/(1+dist*dist*lights[i].quad);
	}
	color=vec4(lighting,1);
}