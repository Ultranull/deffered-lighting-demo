#pragma once

#include<gl/glew.h>
#include<vector>
#include<map>
#include<string>

class FrameBuffer{
	GLuint id;
	GLuint width, height;
	std::map<std::string,GLuint> textures;
	std::vector<GLenum> buffers;

public:
	FrameBuffer();
	FrameBuffer(GLuint w, GLuint h);
	~FrameBuffer();
	
	void bind();
	void read();
	void draw();
	void addDepth();
	void addTexture(std::string name,GLuint internalformat,GLenum format, GLenum attachment);
	void drawBuffers();
	GLuint getTexture(std::string name);
	void check();

	inline GLuint getWidth() { return width; }
	inline GLuint getHeight() { return height; }

	void cleanup();
	static void bindDefualt();
};

