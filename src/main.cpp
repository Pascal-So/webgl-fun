#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <random>
#include <turbotrack.hpp>
#include <sstream>


const int nr_vertices = 20000;

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

// const char* vertexSource =
// 	"#version 100                                 \n"
// 	"uniform mat4 viewmat;                     \n"
// 	"uniform mat4 projmat;                     \n"
// 	"attribute vec3 position;                     \n"
// 	"varying vec3 col;                            \n"
// 	"float v;                            \n"
// 	"void main() {                                \n"
// 	"  gl_Position = projmat * viewmat * vec4(position.xyz, 1.0);     \n"
// 	// "  gl_Position /= gl_Position.z;     \n"
// 	"  gl_PointSize = 1.;     \n"
// 	"  v = -0.4 * gl_Position.z + 1.2;     \n"
// 	"  col = vec3(v,v,v);                  \n"
// 	"}                                            \n";
// const char* fragmentSource =
// 	"#version 100                                 \n"
// 	"precision lowp float;                        \n"
// 	"varying vec3 col;                            \n"
// 	"void main() {                                \n"
// 	// "  gl_FragColor = vec4(1., 1., 1., 1.); \n"
// 	"  gl_FragColor = vec4(col, 1.); \n"
// 	"}                                            \n";

// void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
// 	const GLchar* message, const void* userParam) {
// 	//if (type != GL_DEBUG_TYPE_ERROR) return;
// 	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
// 		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
// 		type, severity, message);
// }
// glEnable(GL_DEBUG_OUTPUT);
// glDebugMessageCallback(MessageCallback, 0);

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

bool mouse_down = false;
Eigen::Vector2f last_mouse_pos;
bool last_mouse_pos_set = false;
Eigen::Quaternionf current_rot;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == 0) {
		switch (action) {
		case GLFW_PRESS: {
			mouse_down = true;
			last_mouse_pos_set = false;
			break;}
		case GLFW_RELEASE: {
			mouse_down = false;
			break;}
		}
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	int mindim = std::min(width, height);

	float x = 2 * (xpos - width  / 2) / mindim;
	float y = - 2 * (ypos - height / 2) / mindim;


	if (mouse_down) {
		Eigen::Vector2f new_mouse_pos(x, y);

		if (last_mouse_pos_set) {
			current_rot = turbotrack::mouse_move(last_mouse_pos, new_mouse_pos) * current_rot;
		}

		last_mouse_pos = new_mouse_pos;
		last_mouse_pos_set = true;
	}
}

void process_input(GLFWwindow* /* window */) {
}

// state
GLFWwindow* window;
unsigned int VAO;
unsigned int shaderProgram;
int viewmat_location, projmat_location;

Eigen::Matrix4f projection(float aspect, float near, float far, float d) {
	const float nmf = near - far;

	Eigen::Matrix4f m;
	m <<
		d / aspect, 0,                   0,                     0,
		         0, d,                   0,                     0,
		         0, 0,  (near + far) / nmf,  2 * near * far / nmf,
		         0, 0,                  -1,                     0;

	return m;
}

void iteration() {
	process_input(window);

	Eigen::Matrix4f viewmat;
	viewmat <<
		current_rot.toRotationMatrix(), Eigen::Vector3f::Zero(),
		0, 0, 0, 1;

	viewmat(2,3) = -2;

	glUniformMatrix4fv(viewmat_location, 1, GL_FALSE, viewmat.data());

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	Eigen::Matrix4f projmat = projection((float)width / height, 0.01, 100, 2.);
	glUniformMatrix4fv(projmat_location, 1, GL_FALSE, projmat.data());


	glClearColor(0.098f, 0.086f, 0.023f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, nr_vertices);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

const int window_init_width = 1000;
const int window_init_height = 800;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(window_init_width, window_init_height, "LearnOpenGL", NULL, NULL);
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

	current_rot = Eigen::Quaternionf(1, 0, 0, 0);

	glViewport(0, 0, window_init_width, window_init_height);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	glEnable(GL_PROGRAM_POINT_SIZE);

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
	std::vector<float> vertices (3 * nr_vertices);
	std::mt19937 gen {0};
	for (int i = 0; i < nr_vertices; ++i) {
		std::normal_distribution<float> d{0,0.3};
		const float x = d(gen);
		const float y = d(gen);
		vertices[3 * i + 0] = x;
		vertices[3 * i + 1] = y;
		vertices[3 * i + 2] = (x*x + y*y) / 3 - 0.2;
	}
	glBufferData(GL_ARRAY_BUFFER, 3 * nr_vertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	int attribute_location_position=glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(attribute_location_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attribute_location_position);
	viewmat_location = glGetUniformLocation(shaderProgram, "viewmat");
	projmat_location = glGetUniformLocation(shaderProgram, "projmat");
	glBindVertexArray(0);

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(iteration, 0, true);
	#else
		while(!glfwWindowShouldClose(window)) {
			iteration();
		}
	#endif

	glfwTerminate();
}
