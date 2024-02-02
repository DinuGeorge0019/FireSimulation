
#include "skybox.h"
#include "../OpenGLWrapper/TextureLoader/texture_loader.h"

#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

Skybox::Skybox(GLuint skyboxTexture, GLuint modelId)
{
	model_id = modelId;
	texID = skyboxTexture;
	GLfloat vertexPositions[] =
	{
		0.75f, 0, -0.75f,
		-0.75f, 0, -0.75f,
		-0.75f, 0, 0.75f,
		0.75f, 0, 0.75f,

	};

	//Texture coordinates for the skybox
	GLfloat textureCoords[] = {
		-4.0f, 0.0f,
		5.0f, 0.0f,
		5.0f, 2.0f,
		-4.0f, 2.0f
	};

	glGenBuffers(1, &position_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &texture_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//The skybox is a flat plane that sits at the top of the scene
void Skybox::Render()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glEnable(GL_BLEND);//Blending is used here since transparency needs to be taken into account and the skybox should be blended with the clear colour (black)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Top
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 200, 0));
	model = glm::scale(model, glm::vec3(30000, 1, 1000));
	
	glUniformMatrix4fv(model_id, 1, GL_FALSE, &model[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisable(GL_BLEND);
}

