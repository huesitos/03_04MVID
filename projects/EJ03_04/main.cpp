#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <engine/input.hpp>
#include <engine/window.hpp>
#include <engine/shader.hpp>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {

	float vertices[] = {
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = {
		0, 2, 1
	};

	// create vertext array and buffers
	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	// bind vertex array and buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// assign attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	glBindVertexArray(0); // VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

	return VAO;
}

void render(uint32_t VAO, const Shader& shader) {
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();
	glm::vec3 color = glm::vec3(0.4f, 0.3f, 0.5f);
	shader.set("addColor", color * (float)(glfwGetTime() / 5));

	glm::vec3 moveInX = glm::vec3(0.01f, 0.0f, 0.0f);
	shader.set("addMovement", moveInX * (float)(glfwGetTime()));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int main(int argc, char* argv[]) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;

	const uint32_t VAO = createVertexData(&VBO, &EBO);
	const Shader shader("../projects/EJ03_04/vertex.vs", "../projects/EJ03_04/fragment.fs");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();

		render(VAO, shader);

		window->frame();
	}

	// delete everything from memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}