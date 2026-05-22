#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main() {
	std::cout << "Hello, OpenGL!" << std::endl;

	int success;
	char infoLog[512];

	/*
	//glm test
	glm::vec4 vec(1.0f, 2.0f, 1.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	vec = trans * vec;
	std::cout << "Transformed vec: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")" << std::endl;
	*/

	glfwInit();

	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if(window==NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		Shaders
	*/

	//compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
	const GLchar* vertShader = vertexShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	//catch vertex shader error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//compile fragment shader
	unsigned int fragmentShaders[2];

	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[0]);

	//catch fragment shader error
	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSrc2 = loadShaderSrc("assets/fragment_core2.glsl");
	const GLchar* fragShader2 = fragmentShaderSrc2.c_str();
	glShaderSource(fragmentShaders[1], 1, &fragShader2, NULL);
	glCompileShader(fragmentShaders[1]);

	//catch fragment shader error
	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//create shader program and link shaders
	unsigned int shaderPrograms[2];

	shaderPrograms[0] = glCreateProgram();
	
	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);

	//catch shader program linking error	
	glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	shaderPrograms[1] = glCreateProgram();

	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);

	//catch shader program linking error	
	glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);

	////vertex array
	//float vertices[] = {
	//	//rectangle
	//	0.5f, 0.5f, 0.0f, // top right
	//	-0.5f, 0.5f, 0.0f, // top left
	//	-0.5f, -0.5f, 0.0f, // bottom left
	//	0.5f, -0.5f, 0.0f, // bottom right
	//};
	//unsigned int indices[] = {
	//	0, 1, 2, // first triangle
	//	2, 3, 0  // second triangle
	//};

	float vertices[] = {
		//first triangle		//colours
		-0.5f,  -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.25f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		-0.1f,  -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
		//second triangle
		0.5f,   -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.25f,   0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.1f,   -0.5f, 0.0f,	0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		3, 4, 5  // second triangle
	};

	// VAO, VBO, EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind VAO
	glBindVertexArray(VAO);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set vertex attribute pointers
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUseProgram(shaderPrograms[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		// draw shapes
		glBindVertexArray(VAO);

		//first triangle
		glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//second triangle
		//glUseProgram(shaderPrograms[1]);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string loadShaderSrc(const char* filename) {
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);
	if(file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	} else {
		std::cout << "Failed to open shader file: " << filename << std::endl;
	}

	file.close();

	return ret;
}