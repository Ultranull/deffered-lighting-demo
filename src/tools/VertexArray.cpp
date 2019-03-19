#include "VertexArray.h"




VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
}

 void VertexArray::bind() {
	glBindVertexArray(id);
}

 void VertexArray::unbind() {
	glBindVertexArray(0);
}

 void VertexArray::cleanup() {
	std::map<std::string, Buffer*>::iterator iter;
	for (iter = buffers.begin(); iter != buffers.end(); iter++) {
		Buffer *b = (*iter).second;
		b->cleanup();
		delete b;
	}
}

 void VertexArray::updateData(std::string name, GLintptr offset, GLsizeiptr size,const void * data) {
	Buffer *buf = buffers[name];
	bind();
	buf->bind();
	buf->setSubData(offset, size, data);
}

 Buffer * VertexArray::getBuffer(std::string name) {
	return buffers[name];
}

