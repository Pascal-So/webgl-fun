#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <cmath>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

const char* vertexSource =
	"#version 100                                 \n"
	"attribute vec3 position;                     \n"
	"varying vec3 col;                            \n"
	"void main() {                                \n"
	"  gl_Position = vec4(position.xyz, 1.0);     \n"
	"  col = vec3(position.xyz);                  \n"
	"}                                            \n";
const char* fragmentSource =
	"#version 100                                 \n"
	"precision lowp float;                        \n"
	"varying vec3 col;                            \n"
	"void main() {                                \n"
	"  gl_FragColor = vec4(col.x + 0.5, col.y + 0.5, 0.6, 1.); \n"
	"}                                            \n";

void error_callback(int /* error */, const char* description) {
	std::cerr << "Error: " << description << '\n';
}

void resize_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}
}

void process_input(GLFWwindow* /* window */) {
}

// state
GLFWwindow* window;
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
unsigned int VAO;
unsigned int shaderProgram;

std::chrono::time_point<std::chrono::steady_clock> time_start;

void iteration() {
	process_input(window);

	std::chrono::duration<double> time_elapsed = std::chrono::steady_clock::now() - time_start;

	glClearColor(0.2f, 0.3f, 0.3f+0.1f*std::sin(time_elapsed.count()), 1.0f);
	// glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 600, 600);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int  shader_compile_success;
	char shader_info_log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shader_compile_success);
	if(!shader_compile_success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, shader_info_log);
		std::cerr << "compiling vertex shader failed: " << shader_info_log << '\n';
		return 1;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shader_compile_success);
	if(!shader_compile_success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, shader_info_log);
		std::cerr << "compiling fragment shader failed: " << shader_info_log << '\n';
		return 1;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shader_compile_success);
	if(!shader_compile_success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, shader_info_log);
		std::cerr << "linking shader program failed: " << shader_info_log << '\n';
		return 1;
	}

	glUseProgram(shaderProgram);

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int attribute_location_position=glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(attribute_location_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attribute_location_position);

	glBindVertexArray(0);

	time_start = std::chrono::steady_clock::now();

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(iteration, 0, true);
	#else
		while(!glfwWindowShouldClose(window)) {
			iteration();
		}
	#endif

	glfwTerminate();
}
