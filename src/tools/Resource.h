#pragma once

#include <map>
#include <string>
#include <stdio.h>

#include "GL/glew.h"

#include"ShaderProgram.h"

class Resource {
	typedef std::pair<std::string, GLuint> Texture;
	std::map<std::string, GLuint> textures;

	std::map<std::string, Shader> shaders;

public:
	std::string path = "assets/", texturePath = "textures/", shaderPath = "shaders/";
	Resource(){}

	GLuint addTexture(std::string name, const char *tex);
	void addTextures(std::string name, const char *tar, int sub_width, int sub_height, int ir, int ic);
	GLuint getTexture(std::string name);
	GLint bindTexture(std::string name,GLuint sample);

	Shader addShader(std::string name);
	Shader getShader(std::string name);

	GLuint LoadGLTexture(const char *filename);
	GLuint LoadGLsubTexture(const char *filename, int sub_x, int sub_y, int sub_width, int sub_height);

	void setPath(std::string texturePath, std::string shaderPath, std::string path);
	std::string getShaderPath(std::string file);

	void cleanup();

	static Resource& getInstance() {
		static Resource instance;
		return instance;
	}
	Resource(Resource const&) = delete;
	void operator=(Resource const&) = delete;

};
