#pragma once
#include <GL/glew.h>

#include <vector>


class Buffer{
	GLuint type;
	size_t typeSize;
	size_t length;	
public:
	GLuint id;

	Buffer(GLuint t);
	Buffer();
	~Buffer();

	void unbind();

	void bind();

	void cleanup();

	template<typename T>
	void setData(std::vector<T> &data, GLenum usage);
	template<typename T>
	void setData(size_t size,GLenum usage);

	void setSubData(GLintptr offset, GLsizeiptr size,const void *data);

	void bindPointer(GLuint loc, GLuint size, GLenum tp, GLuint stride, void* offset, GLuint div);

	void bindPointer(GLuint loc, GLuint size, GLenum tp, GLuint stride = -1, void* offset = 0);
	void bindPointer(GLuint loc, GLuint size, GLenum tp, void* offset = 0, GLuint div=0);
	void bindPointerDiv(GLuint div, GLuint loc, GLuint size, GLenum tp, GLuint stride = -1);

	size_t getLength();
	size_t getTypeSize();

};
template<typename T>
inline void Buffer::setData(std::vector<T>& data, GLenum usage) {
	typeSize = sizeof(T);
	length = data.size();
	glBufferData(type, length * typeSize, &data[0], usage);
}
template<typename T>
inline void Buffer::setData(size_t size, GLenum usage) {
	typeSize = sizeof(T);
	length = size;
	glBufferData(type, length * typeSize, NULL, usage);
}

/*

* 
* lamp.getVAO()->bind();
* Buffer *ibo=lamp.getVAO()->getBuffer("instance");
* ibo->setSubData(sizeof(Light)*(index)+offsetof(Light, pos), sizeof(vec3), &lights[index].pos    );
* ibo->setSubData(sizeof(Light)*(index)+offsetof(Light, color), sizeof(vec3), &lights[index].color);
* lamp.getVAO()->unbind();
* 
* 
* VertexArray *vao=lamp.getVAO();
* vao->updateData("intance",
*	sizeof(Light)*(index)+offsetof(Light, pos), sizeof(vec3), &lights[index].pos);
* vao->unbind();
* 
* vao=VertexArray();
* Buffer *vbo=vao.bindBuffer<Vertex>("vertexes",GL_ARRAY_BUFFER);
* vbo->setData(vertices, GL_STATIC_DRAW);
* vbo->bindPointer(0, 3, GL_FLOAT, (void*)offsetof(Vertex, pos));
* vbo->bindPointer(1, 3, GL_FLOAT, (void*)offsetof(Vertex, color));
* vbo->bindPointer(2, 2, GL_FLOAT, (void*)offsetof(Vertex, UV));
* 
* Buffer *ebo=vao.bindBuffer<unsigned int>("indecies",GL_ELEMENT_ARRAY_BUFFER);
* ebo->setData(indices, GL_STATIC_DRAW);
* 
* 
* */

