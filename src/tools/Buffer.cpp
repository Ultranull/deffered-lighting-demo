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

 void Buffer::setSubData(GLintptr offset, GLsizeiptr size,const void * data) {
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

 void Buffer::bindPointer(GLuint loc, GLuint size, GLenum tp, void * offset,GLuint div) {
	bindPointer(loc, size, tp, typeSize, offset, div);
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

 UniformBuffer::UniformBuffer() :Buffer(GL_UNIFORM_BUFFER) {}

 void UniformBuffer::blockBinding(GLuint program, GLuint index, const char * name) {
	 GLuint loc = glGetUniformBlockIndex(program, name);
	 glUniformBlockBinding(program, loc, 0);
	 glBindBufferBase(GL_UNIFORM_BUFFER, 0, id);
 }
