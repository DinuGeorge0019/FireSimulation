#ifndef BONFIRE_H_
#define BONFIRE_H_

#include "../Cylinder/cylinder.h"
#include "../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../ParticleEffectContainer/TorchFireParticleEffect/torch_particle_effect.h"

#include "glm/gtc/matrix_transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Bonfire
{
private:
	GLuint particle_program;
	GLuint model_id;
	glm::vec3 position;

	Camera* camera;

	GLuint log_texture;
	std::vector<Cylinder*> logs;
	std::vector<glm::mat4> log_transformations;

	GLuint chair_texture;
	std::vector<Cylinder*> chairs;
	std::vector<glm::mat4> chair_transformations;

	GLuint torch_texture;
	std::vector<Cylinder*> torches;
	std::vector<glm::mat4> torch_transformations;

	GLuint fire_particle_texture;
	std::vector<TorchParticleEffect*> torches_fire;

	const float DEG2RAD = 3.14159 / 180;


	/* START PRIVATE FUNCTION DECLARATIONS*/
	void create_bonfire();
	void render_bonfire();
	void create_log_chairs();
	void render_log_chairs();
	void create_torches();
	void render_torches();
	void render_torches_fire_particles();
	/* END PRIVATE FUNCTION DECLARATIONS*/

public:
	Bonfire(GLuint particle_program, GLuint modelId, Camera* camera, glm::vec3 bonfirePosition, GLuint logTexture, GLuint chair_texture, GLuint torch_texture, GLuint fire_particle_texture);
	void init();
	void render();
	void render_particles();
};

#endif /* BONFIRE_H_ */
