#include "Mesh.h"

#include "Buffer.h"



void Mesh::bindmeshindexed() {

	vaObject = VertexArray();
	Buffer *vbuffer = vaObject.bindBuffer<Vertex>("vertexes", GL_ARRAY_BUFFER);
	vbuffer->setData(vertices, GL_STATIC_DRAW);
	vbuffer->bindPointer(0, 3, GL_FLOAT, (void*)offsetof(Vertex, pos));
	vbuffer->bindPointer(1, 3, GL_FLOAT, (void*)offsetof(Vertex, color));
	vbuffer->bindPointer(2, 2, GL_FLOAT, (void*)offsetof(Vertex, UV));
	vbuffer->bindPointer(3, 3, GL_FLOAT, (void*)offsetof(Vertex, normal));
	vbuffer->unbind();

	Buffer *ebuffer = vaObject.bindBuffer<unsigned int>("indices", GL_ELEMENT_ARRAY_BUFFER);
	ebuffer->setData(indices, GL_STATIC_DRAW);
	ebuffer->unbind();
}


void Mesh::bindmesh() {

	vaObject = VertexArray();
	Buffer *vbuffer = vaObject.bindBuffer<Vertex>("vertexes", GL_ARRAY_BUFFER);
	vbuffer->setData(vertices, GL_STATIC_DRAW);
	vbuffer->bindPointer(0, 3, GL_FLOAT, (void*)offsetof(Vertex, pos));
	vbuffer->bindPointer(1, 3, GL_FLOAT, (void*)offsetof(Vertex, color));
	vbuffer->bindPointer(2, 2, GL_FLOAT, (void*)offsetof(Vertex, UV));
	vbuffer->bindPointer(3, 3, GL_FLOAT, (void*)offsetof(Vertex, normal));
	vbuffer->unbind();
}

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<Vertex> t_verts, std::vector<unsigned int> t_ind):
										vertices(t_verts),indices(t_ind){
	bindmeshindexed();
}

Mesh::Mesh(std::vector<Vertex> t_verts) :
	vertices(t_verts), indices() {
	bindmesh();
}

void Mesh::render(GLuint method) {
	vaObject.bind();
	glDrawElements(method, indices.size(), GL_UNSIGNED_INT, 0);
	vaObject.unbind();
	
}

void Mesh::renderVertices(GLuint method) {
	vaObject.bind();
	glDrawArrays(method, 0, vertices.size());
	vaObject.unbind();
}

void Mesh::cleanup() {
	vaObject.cleanup();
}
