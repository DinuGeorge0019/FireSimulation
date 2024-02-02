
#ifndef WRAPPER_GLFW_H_
#define WRAPPER_GLFW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class GLWrapper {
private:

	int width;
	int height;
	const char *title;
	double fps;
	void(*error_callback)(int error, const char* description);
	void(*renderer)();
	void(*reshape)(GLFWwindow* window, int w, int h);
	void(*keyCallBack)(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool running;
	GLFWwindow* window;

public:
	GLWrapper(int width, int height, const char *title);
	~GLWrapper();

	void setFPS(double fps) {
		this->fps = fps;
	}

	/* Callback registering functions */
	void setRenderer(void(*f)());
	void setReshapeCallback(void(*f)(GLFWwindow* window, int w, int h));
	void setKeyCallback(void(*f)(GLFWwindow* window, int key, int scancode, int action, int mods));
	void setErrorCallback(void(*f)(int error, const char* description));

	/* Shader load and build support functions */
	GLuint LoadShader(const char *vertex_path, const char *fragment_path);
	GLuint LoadShader(const char *vertex_path, const char *fragment_path, const char *geo_path);
	GLuint BuildShader(GLenum eShaderType, const std::string &shaderText);
	GLuint BuildShaderProgram(std::string vertShaderStr, std::string fragShaderStr);
	GLuint BuildShaderProgram(std::string vertShaderStr, std::string fragShaderStr, std::string geoShaderStr);
	std::string readFile(const char *filePath);

	int eventLoop();
	GLFWwindow* getWindow();
};

#endif /* WRAPPER_GLFW_H_ */



