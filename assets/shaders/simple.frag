#version 330 core
out vec4 fragcolor;

uniform float time;
uniform vec3 color;

void main(){
	fragcolor=vec4(color,1);
}