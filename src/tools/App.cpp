#include "App.h"




App::App(){
}


App::~App(){
	glfwTerminate();
}


void App::viewportinit(GLFWwindow *window) {
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}
App::App(int w, int h, const char *title) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize glfw\n");
		getchar();
		exit(-1);
	}

	window = glfwCreateWindow(w, h, title, NULL, NULL);
	if (window == nullptr) {
		fprintf(stderr, "Failed to init window\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to load glew\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	viewportinit(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window,onKey);
}

void App::mainLoop() {

	int frames = 0;
	double lastFrame = glfwGetTime();
	double lastFPS = glfwGetTime();

	while (running&&glfwWindowShouldClose(window)==0) {
		double currenttime = glfwGetTime();
		frames++;
		if (currenttime - lastFPS >= 1) {
			fps = frames;
			frames = 0;
			lastFPS += 1.0;
		}
		double delta = currenttime - lastFrame;
		lastFrame = currenttime;
		inputListener(delta);
		update(delta);
		render(delta);
		ticks += 1 * delta;
		glfwSwapBuffers(window);
		glfwPollEvents();
		viewportinit(window);
	}
	onClose();
}

void App::callback(int key, int scancode, int action, int mods) {
	keyfunc func = inputs[key];
	if (func != nullptr) {
		func(action, mods);
	}
}
void App::addInput(int key, keyfunc func) {
	inputs.insert(keyAction(key, func));
}

void App::start() {
	initGL();
	init();
	mainLoop();
}