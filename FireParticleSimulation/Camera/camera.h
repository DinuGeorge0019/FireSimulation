#ifndef CAMERA_H_
#define CAMERA_H_

#include "../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"

#include "glm/gtc/matrix_transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 lookAtPosition;

	GLuint viewID;
	GLuint projectionID;

	GLfloat x_rotation;
	GLfloat y_rotation;
	GLfloat aspect_ratio;
public:
	Camera(GLuint viewID, GLuint projectionID, glm::vec3 startPosition, glm::vec3 startLookAtPosition);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetSkyboxView();
	GLfloat GetAspectRatio();
	void SetAspectRatio(GLfloat aspectRatio);
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBackward();
	void RotateLeft();
	void RotateRight();
	void Update();
	void Lookat(glm::vec3 position);
	glm::vec3 GetLookAtPosition();
	glm::vec3 GetCameraPosition();
};

#endif /* CAMERA_H_*/