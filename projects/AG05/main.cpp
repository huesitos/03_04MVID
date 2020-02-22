#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <engine/input.hpp>
#include <engine/window.hpp>
#include <engine/shader.hpp>
#include <engine/texture.hpp>
#include <engine/geometry/geometry.hpp>
#include <engine/geometry/quad.hpp>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

void render(const Geometry& geometry, const Shader& shader, Texture& tex1, Texture& tex2) {
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();

	tex1.use(shader, "tex1", 0);
	tex2.use(shader, "tex2", 1);

	geometry.render();	
}

int main(int argc, char* argv[]) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader shader("../projects/AG05/vertex.vs", "../projects/AG05/fragment.fs");
	const Quad quad(1.0f);

	Texture tex1("../assets/textures/bricks.jpg", Texture::Format::RGB);
	Texture tex2("../assets/textures/dark_bricks.jpg", Texture::Format::RGB);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();

		render(quad, shader, tex1, tex2);

		window->frame();
	}

	return 0;
}