#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 uv;
in vec3 normal;
in vec4 fragpos;


uniform sampler2D diffuse;

void main(){
	gPosition=fragpos.xyz;
	gNormal=normalize(normal);
	gAlbedoSpec.xyz=texture2D(diffuse,uv).xyz;
	gAlbedoSpec.a=0;
}