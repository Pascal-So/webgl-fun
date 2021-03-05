#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "io.hpp"

class ShaderProgram {
	std::vector<GLuint> shaders;

public:
	GLuint program;

	void add_shader(const std::string &path, GLenum shader_type) {
		GLuint shader = glCreateShader(shader_type);
		const std::string shader_source = load_file(path);
		const char* shader_source_cstr= shader_source.c_str();

		glShaderSource(shader, 1, &shader_source_cstr, NULL);
		glCompileShader(shader);

		GLint shader_compile_success;
		char shader_info_log[1024];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compile_success);
		if(!shader_compile_success) {
			glGetShaderInfoLog(shader, 1024, NULL, shader_info_log);
			throw std::invalid_argument(std::string("compiling shader failed:\n") + shader_info_log);
		}

		shaders.push_back(shader);
	}

	void link() {
		program = glCreateProgram();

		for (auto shader : shaders)
			glAttachShader(program, shader);

		glLinkProgram(program);

		for (auto shader : shaders)
			glDeleteShader(shader);

		shaders.clear();

		GLint shader_compile_success;
		char shader_info_log[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &shader_compile_success);
		if(!shader_compile_success) {
			glGetProgramInfoLog(program, 1024, NULL, shader_info_log);
			throw std::invalid_argument(std::string("linking shader program failed:\n") + shader_info_log);
		}
	}

	void use() const {
		glUseProgram(program);
	}
};

#endif // SHADER_HPP
