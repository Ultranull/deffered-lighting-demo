#version 420 core

layout(location = 0) in vec3 vertpos;
layout(location = 2) in vec2 vertuv;
layout(location = 3) in vec3 vertnorm;
layout(location = 6) in vec3 offset;

out vec2 uv;
out vec3 normal;
out vec4 fragpos;

layout(std140,binding=0)uniform camera{
	mat4 projection;
	mat4 view;
};


//uniform mat4 view;
//uniform mat4 projection;


void main(){
	vec4 pos=vec4(vertpos+offset,1);
	gl_Position=projection*view*pos;
	uv=vertuv;
	normal=vertnorm;
	fragpos=pos;
}

