#ifndef SHRUB_H_
#define SHRUB_H_

#include "../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../Cylinder/cylinder.h"

#include <vector>
#include <stack>

class Shrub
{
public:
	Shrub(GLuint modelID, glm::vec3 shrubPosition, glm::vec3 terrainNormal, GLuint texture);
	~Shrub();
	void Draw();
	void DrawShadows();
	void Generate(int level, int leaves);
private:
	void Initialise();
	void GenerateShrub(int level, int leaves);
	void AddBranch();
	void addBranch(int l);
	const char* lstring;
	std::stack<glm::mat4> pen;
	std::stack<glm::mat4> shadow_pen;
	std::vector<Cylinder*> parts;
	std::vector<glm::mat4> transformations;
	std::vector<glm::mat4> shadow_transformations;
	GLuint model_id;
	glm::vec3 terrain_normal;
	glm::vec3 position;
	GLfloat x, y = 0;
	//void Shrub::drawBranch(int l);
	GLuint texture_id;
	GLuint position_buffer_object, normals_buffer_object, texture_buffer_object;
};

#endif /* SHRUB_H_*/