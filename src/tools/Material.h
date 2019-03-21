#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>

struct Texture {

	GLuint id;

	GLuint target;
	GLuint FILTER;
	GLuint WRAP;
	GLuint internalformat, format;


	int width, height;

	Texture(GLuint target, GLuint internalformat, GLuint format,
			GLuint FILTER= GL_NEAREST, GLuint WRAP= GL_CLAMP_TO_EDGE):
	target(target),FILTER(FILTER),WRAP(WRAP),internalformat(internalformat),format(format){//omfg this is chaos
		glGenTextures(1, &id);
	}
	Texture(GLuint target):
		Texture(target, GL_RGBA, GL_RGBA){}

	void load();
	void setSize(int w,int h);

	void bind();
	GLint activate(GLenum texture);
	void cleanup();


	static void unbind(GLuint target = GL_TEXTURE_2D);
};

struct Material {
	glm::vec3 ambient;
	Texture diffuse;
	glm::vec3 specular;
	glm::vec3 color;
	float shininess;
};

