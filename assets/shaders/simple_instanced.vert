#version 330 core

layout(location = 0) in vec3 vertpos;
layout(location = 1) in vec3 vertcolor;
layout(location = 2) in vec2 vertuv;
layout(location = 3) in vec3 vertnorm;
layout(location = 6) in vec3 offset;

out vec2 uv;
out vec3 normal;


uniform mat4 view;
uniform mat4 projection;


void main(){
	gl_Position=projection*view*vec4(vertpos+offset,1);
	uv=vertuv;
	normal=vertnorm;
}

