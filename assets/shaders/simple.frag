#version 330 core
out vec4 fragcolor;

uniform float time;

in vec3 lampcolor;

void main(){
	fragcolor=vec4(lampcolor,1);
}