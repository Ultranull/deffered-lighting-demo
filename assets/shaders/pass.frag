#version 330 core
out vec4 fragcolor;

in vec3 color;
in vec2 uv;
uniform float time;
uniform sampler2D passthrough;

void main(){
	vec2 raster=vec2(1,1)*32;
	vec2 id=fract(floor(uv*raster)/raster);

	vec3 col=texture(passthrough,uv).xyz;

	fragcolor=vec4(col,1);
}