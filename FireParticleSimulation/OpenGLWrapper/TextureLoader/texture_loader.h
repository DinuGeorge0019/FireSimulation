#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include "../GlfwWrapper/wrapper_glfw.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class TextureLoader
{
private:
	GLuint program;
public:
	TextureLoader(GLuint prog);
	GLuint Load(const char* path);
	GLuint LoadBump(const char* path);
};


#endif /* TEXTURE_LOADER_H_*/
