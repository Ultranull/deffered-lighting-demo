#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

const GLuint FORWARD = GLFW_KEY_W;//FIX! move to input class
const GLuint BACKWARD = GLFW_KEY_S;
const GLuint STRAFERIGHT = GLFW_KEY_D;
const GLuint STRAFELEFT = GLFW_KEY_A;
const GLuint JUMP = GLFW_KEY_SPACE;
const GLuint CROUCH = GLFW_KEY_LEFT_SHIFT;


Camera::Camera() {
	position = vec3(0, 0, 0);
	direction = vec3(0, 0, 0);
}
Camera::Camera(vec3 pos, vec3 dir, vec3 u) :position(pos), direction(dir), up(u) {
	projection = glm::perspective(radians(FOV), 4.f / 3.f, .1f, 100.f);
	view = lookAt(position, position + direction, up);
}

void Camera::apply(GLFWwindow *window, float delta) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);//FIX! all input should be called from input class
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	hangle += mouseSpeed * float(width / 2 - xpos);//theta
	vangle += mouseSpeed * float(height / 2 - ypos);//phi

	float pi = radians(180.f);

	direction = vec3(cos(vangle)*sin(hangle),
		sin(vangle),
		cos(vangle)*cos(hangle));
	vec3 right(sin(hangle - pi / 2.f),
		0,
		cos(hangle - pi / 2.f));
	up = cross(right, direction);
	vec3 front = cross(vec3(0,1,0), right);

	if (glfwGetKey(window, FORWARD) == GLFW_PRESS) {//FIX! all input should be called from input class
		position += front * delta * speed;
	}
	if (glfwGetKey(window, BACKWARD) == GLFW_PRESS) {
		position -= front * delta * speed;
	}
	if (glfwGetKey(window, STRAFERIGHT) == GLFW_PRESS) {
		position += right * delta * speed;
	}
	if (glfwGetKey(window, STRAFELEFT) == GLFW_PRESS) {
		position -= right * delta * speed;
	}
	if (glfwGetKey(window, JUMP) == GLFW_PRESS) {
		position += vec3(0,1,0) * delta * speed;
	}
	if (glfwGetKey(window, CROUCH) == GLFW_PRESS) {
		position -= vec3(0, 1, 0) * delta * speed;
	}

	float ratio = width / (float)height;
	view = lookAt(position, position + direction, up);
}
void Camera::orbit(GLFWwindow *window, float delta,vec3 target) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);//FIX! all input should be called from input class
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (glfwGetKey(window, FORWARD) == GLFW_PRESS) {//FIX! all input should be called from input class
		orbitDist -=  delta * speed;
	}
	if (glfwGetKey(window, BACKWARD) == GLFW_PRESS) {
		orbitDist +=  delta * speed;
	}

	hangle += mouseSpeed * float(width / 2 - xpos);
	vangle += mouseSpeed * float(height / 2 - ypos);

	float pi = radians(180.f);

	position = vec3(cos(vangle)*sin(hangle),
					 sin(vangle),
					 cos(vangle)*cos(hangle))*orbitDist + target;
	direction = (position - target)/ orbitDist;
	vec3 right(sin(hangle - pi / 2.f),
			   0,
			   cos(hangle - pi / 2.f));
	up = cross(right, direction);

	float ratio = width / (float)height;
	view = glm::lookAt(
		position, 
		target,
		up  
	);


}
void Camera::perspective(GLFWwindow *window,float FOV,float near,float far) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	projection=glm::perspective(radians(FOV), width / (float)height, near, far);
}

void Camera::orthographic(float left, float right, float bottom, float top, float near, float far) {
	projection = glm::ortho(left,right,bottom,top,near,far);
}
mat4 Camera::P() {
	return projection;
}

mat4 Camera::V() {
	return view;
}

vec3 Camera::getDirection() {
	return direction;
}

vec3 Camera::getPosition() {
	return position;
}
