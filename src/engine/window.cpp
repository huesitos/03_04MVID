#include "engine/window.hpp"
#include "engine/input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	glViewport(0, 0, width, height);
	std::cout << width << " " << height << std::endl;
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else {
		Input::instance()->addKey(key, action);
	}
}

Window::Window() {
	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		exit(-1);
	}
	// glfw fue inicializada

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(800, 600, "40MID", nullptr, nullptr);
	if (!window_) {
		std::cout << "Error Creating Window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error Initializing GLAD" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwSetFramebufferSizeCallback(window_, onChangeFrameBufferSize);
	glfwSetKeyCallback(window_, onKeyPress);
}

Window::~Window() {

}

Window* Window::instance() {
	static Window w;
	return &w;
}

bool Window::alive() const {
	return !glfwWindowShouldClose(window_);
}

void Window::frame() const {
	// clear all keys from the previous frame
	Input::instance()->clear();
	glfwSwapBuffers(window_);
	glfwPollEvents();
}