#include "FrameBuffer.h"



FrameBuffer::FrameBuffer():FrameBuffer(500,500){
}

FrameBuffer::FrameBuffer(GLuint w, GLuint h):width(w),height(h) {
	glGenFramebuffers(1, &id);
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, width, height);
}

void FrameBuffer::read(){
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
}

void FrameBuffer::draw(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
}

void FrameBuffer::addDepth(){
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::addTexture(std::string name, GLuint internalformat, GLenum format,GLenum attachment){
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLuint text;
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, text, 0);
	textures.insert(std::make_pair(name, text));
	buffers.push_back(attachment);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::drawBuffers(){
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (buffers.empty())
		glDrawBuffer(GL_NONE);
	else
		glDrawBuffers(buffers.size(), &buffers[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::getTexture(std::string name){
	return textures[name];
}

void FrameBuffer::check(){
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("fb failed");
		getchar();
		exit(-1);
	}
}

void FrameBuffer::cleanup(){
	glDeleteFramebuffers(1, &id);
	std::map<std::string, GLuint>::iterator textit = textures.begin();
	for (; textit != textures.end(); textit++) {
		GLuint texture = textit->second;
		glDeleteTextures(1, &texture);
	}
}

void FrameBuffer::bindDefualt(){
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
