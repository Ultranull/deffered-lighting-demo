#include "Buffer.h"






 Buffer::Buffer(GLuint t) :type(t) {
	glGenBuffers(1, &id);
}

 Buffer::Buffer() {}
 Buffer::~Buffer() {}



 void Buffer::unbind() {
	glBindBuffer(type, 0);
}

 void Buffer::bind() {
	glBindBuffer(type, id);
}

 void Buffer::cleanup() {
	glDeleteBuffers(1, &id);
}

 void Buffer::setSubData(GLintptr offset, GLsizeiptr size, void * data) {
	glBufferSubData(type, offset, size, data);
}

 void Buffer::bindPointer(GLuint loc, GLuint size, GLenum tp, GLuint stride, void * offset, GLuint div) {
	glVertexAttribPointer(loc, size, tp, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(loc);
	glVertexAttribDivisor(loc, div);
}

 void Buffer::bindPointer(GLuint loc, GLuint size, GLenum tp, GLuint stride, void * offset) {
	stride = stride == -1 ? typeSize : stride;
	bindPointer(loc, size, tp, stride, offset, 0);
}

 void Buffer::bindPointer(GLuint loc, GLuint size, GLenum tp, void * offset) {
	bindPointer(loc, size, tp, typeSize, offset, 0);
}

 void Buffer::bindPointerDiv(GLuint div, GLuint loc, GLuint size, GLenum tp, GLuint stride) {
	stride = stride == -1 ? typeSize : stride;
	bindPointer(loc, size, tp, stride, (void*)0, div);
}

 size_t Buffer::getLength(){
	 return length;
 }

 size_t Buffer::getTypeSize(){
	 return typeSize;
 }
