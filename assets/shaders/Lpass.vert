#version 330 core

layout(location = 0) in vec3 vertpos;
layout(location = 2) in vec2 vertuv;

out vec2 uv;


void main(){
	gl_Position=vec4(vertpos,1);
	uv=vertuv;
}

