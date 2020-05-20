
#include <string>
#include <iostream>
#include <vector>

#include <glad/glad.h>

void LOG(const std::string&);

#ifndef GL_DEBUG_OUTPUT
inline void printOpenGlErrors(std::string _id) {
		bool hasError = false;
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			if (!hasError) {
				hasError = true;
				std::cout << "---- Print OpenGl Errors: " << _id << " ----" << std::endl;
			}
			switch (err) {
			case GL_INVALID_VALUE:
					std::cout << "Invalid Value";
					break;
			case GL_INVALID_OPERATION:
					std::cout << "Invalid Operation";
					break;
			case GL_OUT_OF_MEMORY:
					std::cout << "Out of Memory";
					break;
			case GL_INVALID_ENUM:
					std::cout << "Invalid Enum";
					break;
			case GL_STACK_OVERFLOW:
					std::cout << "Stack Overflow";
					break;
			case GL_STACK_UNDERFLOW:
					std::cout << "Stack Underflow";
					break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
					std::cout << "Invalid Framebuffer Operation";
					break;
            #ifdef GL_CONTEXT_LOST
			case GL_CONTEXT_LOST:
					std::cout << "Context Lost";
					break;
            #endif
			}
			std::cout << " (" << err << ")" << std::endl;
		}
		if (hasError) std::cout << "---- Finished ----" << std::endl;
}
#endif

class ShaderProgram {

	enum Status {
		success, failed, missing
	};

	const std::string id;

	GLint program = -1, compute = -1, vertex = -1, geom = -1, frag = -1;
#ifdef GL_COMPUTE_SHADER
	void print(std::string, Status, Status, Status, Status, Status, std::string);	
#else
    void print(std::string, Status, Status, Status, Status, std::string);
#endif
   
public:
	ShaderProgram(const std::string&);
    ShaderProgram();
	~ShaderProgram();

	bool printDebug = true;
    /*
    assumes the following:
    compute shader: [PATH_TO_FILE].comp
    vertex shader: [PATH_TO_FILE].ver
    geometry shader: [PATH_TO_FILE].geo
    fragment shader: [PATH_TO_FILE].frag
    */
    bool compileFromFile(const std::string&);
	bool compile(const char*, const char*, const char*, const char*);
	GLint getHandle();
	void bind();
	void unbind();
};
