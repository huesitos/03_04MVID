#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <engine/input.hpp>
#include <engine/window.hpp>
#include <engine/shader.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const float alphaDelta = 0.05f;
float alphaMask = 0.0f;

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;

		// right arrow key increases the alpha by the alpha delta
		// left arrow decreases it
		if (key.first == 262) { // right arrow
			alphaMask += alphaDelta;
			alphaMask = alphaMask > 1.0f ? 1.0f : alphaMask;
		}
		else if (key.first == 263) { // left arrow
			alphaMask -= alphaDelta;
			alphaMask = alphaMask < 0.0f ? 0.0f : alphaMask;
		}
	}
}

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {

	float vertices[] = {
		// vertex				color					texture
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // t r
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // t l
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // b l
		-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f  // b r
	};

	uint32_t indices[] = {
		0, 3, 1,
		1, 3, 2
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	glBindVertexArray(0); // VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

	return VAO;
}

uint32_t createTexture(const char* path) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

	if (data) {
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB,
			width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Error loading texture " << path << std::endl;
	}

	return texture;
}

void render(uint32_t VAO, const Shader& shader, uint32_t tex1, uint32_t tex2) {
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();

	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	shader.set("alphaMask", alphaMask);
	shader.set("tex1", 0);
	shader.set("tex2", 1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int main(int argc, char* argv[]) {
	Window* window = Window::instance();

	stbi_set_flip_vertically_on_load(true);

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;

	const uint32_t VAO = createVertexData(&VBO, &EBO);
	const Shader shader("../projects/EJ04_05/vertex.vs", "../projects/EJ04_05/fragment.fs");

	uint32_t tex1 = createTexture("../assets/textures/bricks.jpg");
	uint32_t tex2 = createTexture("../assets/textures/dark_bricks.jpg");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();

		render(VAO, shader, tex1, tex2);

		window->frame();
	}

	// delete everything from memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);

	return 0;
}