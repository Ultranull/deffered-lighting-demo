#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::mat4 view;
	glm::mat4 projection;

	float FOV = 45.f;
	float speed = 3.f;
	float mouseSpeed = 0.005f;
	double xpos, ypos;
	float hangle = 0, vangle = 0;
	float orbitDist = 3;

public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 u);

	void apply(GLFWwindow *window, float delta);
	void orbit(GLFWwindow *window, float delta,glm::vec3 target);
	glm::mat4 P();
	glm::mat4 V();
	void perspective(GLFWwindow *window, float FOV, float near, float far);
	void orthographic(float left, float right, float bottom, float top, float near, float far);

	glm::vec3 getDirection();
	glm::vec3 getPosition();
};

