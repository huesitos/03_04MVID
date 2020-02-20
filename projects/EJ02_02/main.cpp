#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <engine/input.hpp>
#include <engine/window.hpp>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

uint32_t createVertexData(
	std::vector<float> vertices,
	uint32_t* VBO) {
	
	// create vertext array and buffers
	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);

	// bind vertex array and buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * vertices.size(), 
		vertices.data(), 
		GL_STATIC_DRAW);

	// assign attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	glBindVertexArray(0); // VAO

	return VAO;
}

bool checkShader(uint32_t shader) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Error Compiling Shader " << infoLog << std::endl;
		return false;
	}
	return true;
}

bool checkProgram(uint32_t program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "Error Linking Program " << infoLog << std::endl;
		return false;
	}
	return true;
}

uint32_t createProgram() {
	// write shader source
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main() {\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(0.6, 0.6, 0.1, 1.0);\n"
		"}\0";

	// create both shaders before the program
	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	checkShader(vertexShader);

	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader);

	// create program
	const uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	checkProgram(program);

	// always delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void render(std::vector<uint32_t> vaos, uint32_t program) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	for (uint32_t vao : vaos) {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}	
}

int main(int argc, char* argv[]) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	std::vector<float> vertices = {
		0.0f, -0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f		
	};
	std::vector<float> vertices2 = {
		0.0f, -0.5f, 0.0f,
		-1.0f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f		
	};

	uint32_t VBO1, VBO2, EBO;
	const uint32_t VAO1 = createVertexData(vertices, &VBO1);
	const uint32_t VAO2 = createVertexData(vertices2, &VBO2);
	const uint32_t program = createProgram();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();

		std::vector<uint32_t> vaos = {VAO1, VAO2};
		render(vaos, program);

		window->frame();
	}

	// delete everything from memory
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);

	glDeleteProgram(program);

	return 0;
}