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

	/*glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)   );
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color) );
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV)    );
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);*/
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

	/*glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);*/
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
	//glBindVertexArray(vao);
	vaObject.bind();
	glDrawElements(method, indices.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	vaObject.unbind();
	
}

void Mesh::renderVertices(GLuint method) {
	//glBindVertexArray(vao);
	vaObject.bind();
	glDrawArrays(method, 0, vertices.size());
	//glBindVertexArray(0);
	vaObject.unbind();
}

void Mesh::cleanup() {
	vaObject.cleanup();

	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);
}
