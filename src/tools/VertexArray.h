#pragma once

#include <GL/glew.h>
#include <map>
#include <string>
#include <memory>

#include "Buffer.h"

class VertexArray{
	GLuint id;
	std::map<std::string, Buffer*> buffers;
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	void cleanup();

	template<typename T> 
	Buffer* bindBuffer(std::string name, GLenum type);

	void updateData(std::string name, GLintptr offset, GLsizeiptr size, void *data);

	Buffer* getBuffer(std::string name);

};

