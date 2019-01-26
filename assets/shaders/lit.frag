#version 330 core

in vec2 uv;
in vec3 normal;

uniform float time;

uniform sampler2D diffuse;

void main(){
    
    float diff = max(dot(normal, vec3(-1,0,0)), 0.0)+
				 max(dot(normal, vec3(1,0,0)), 0.0);
	gl_FragColor=texture2D(diffuse,uv)*diff;
}