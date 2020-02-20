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

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {

	float r = 0.8f;
	const int sides = 6;
	float vertices[(sides+1)*3];
	vertices[0] = 0.0f; // p1.x
	vertices[1] = 0.0f; // p2.y
	vertices[2] = 0.0f; // p3.z

	float angle = 315.0f; // drawing in clockwise direction
	for (int i = 3; i <= sides*3; i+=3) {
		vertices[i] = r * cosf(angle); //pi.x
		vertices[i+1] = r * sinf(angle); //pi.y
		vertices[i+2] = 0.0f; //pi.z
		angle -= 45;

		std::cout << "point: " << i << std::endl;
		std::cout << "x " << vertices[i] << " y " << vertices[i + 1] << " z " << vertices[i + 2] << std::endl;
		std::cout << "angle: " << angle << std::endl;
	}

	uint32_t indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 1
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO
	glBindVertexArray(0); // VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

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
		"    FragColor = vec4(0.7, 0.3, 0.4, 1.0);\n"
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

void render(uint32_t VAO, uint32_t program) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);
}

int main(int argc, char* argv[]) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;
	const uint32_t VAO = createVertexData(&VBO, &EBO);
	const uint32_t program = createProgram();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();

		render(VAO, program);

		window->frame();
	}

	// delete everything from memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	return 0;
}