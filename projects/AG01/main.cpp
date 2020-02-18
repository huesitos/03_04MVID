#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height) {
	glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	// glfw fue inicializada

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "40MID", nullptr, nullptr);
	if (!window) {
		std::cout << "Error Creating Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error Initializing GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, onChangeFrameBufferSize);

	glClearColor(0.5f, 0.5f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		handleInput(window);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); // siempre cerrar glfw al terminar

	return 0;
}