#version 420 core

layout(location = 0) in vec3 vertpos;
layout(location = 3) in vec3 vertnorm;
layout(location = 6) in vec3 offset;
layout(location = 7) in vec3 instcolor;

out vec3 normal;
out vec3 lampcolor;

layout(std140,binding=0)uniform camera{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

mat4 translate(vec3 pos){
	return (mat4(
		vec4(1,0,0,0),
		vec4(0,1,0,0),
		vec4(0,0,1,0),
		vec4(pos,1)
	));
}

void main(){
	gl_Position=projection*view*translate(offset)*model*vec4(vertpos,1);
	lampcolor=instcolor;
	normal=vertnorm;
}

