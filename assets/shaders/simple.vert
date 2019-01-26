#version 330 core

layout(location = 0) in vec3 vertpos;
layout(location = 3) in vec3 vertnorm;

out vec3 normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	gl_Position=projection*view*model*vec4(vertpos,1);
	normal=vertnorm;
}

