#pragma once

#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 pos       = glm::vec3(0);
	glm::vec3 color     = glm::vec3(0);
	glm::vec2 UV        = glm::vec2(0);
	glm::vec3 normal    = glm::vec3(0);
	glm::vec3 tangent   = glm::vec3(0);
	glm::vec3 bitangent = glm::vec3(0);
	unsigned int boneID[4] = {201,201,201,201};
	float weights[4] = {0,0,0,0};
};
class Mesh {
	std::vector<unsigned int> indices;

	GLuint vbo;
	GLuint ebo;

	void bindmeshindexed();
	void bindmesh();
public:
	GLuint vao;
	std::vector<Vertex> vertices;

	Mesh();
	Mesh(std::vector<Vertex>);
	Mesh(std::vector<Vertex>,std::vector<unsigned int>);
	~Mesh() {}

	void render(GLuint);
	void renderVertices(GLuint);
	void cleanup();
};

