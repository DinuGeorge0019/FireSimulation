#ifndef PARTICLE_EFFECT_H_
#define PARTICLE_EFFECT_H_


#include "../../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../particle.h"
#include "../../Camera/camera.h"

#include <glm/glm.hpp>

class ParticleEffect {

protected:
    GLuint particles_position_buffer;
	int max_particles = 2000;
	Particle particles_container[2000];
	int last_used_particle;
	GLfloat* particle_positions;
	int particles_count;
	double lastTime;

	glm::vec3 position;

	GLuint vertex_array_id;
	GLuint program_id;
	GLuint particle_texture;
	GLuint texture_id;
	GLuint view_proj_matrix_id;
	GLuint alpha_id;
	GLuint cull_value_id;

	int FindUnusedParticle();
	void SortParticles();
	void defineUniforms();
	void Setup();

public:
	ParticleEffect(GLuint particle_texture, glm::vec3 pos);
	~ParticleEffect();

	virtual void create(GLuint program, Camera* camera = nullptr) = 0;
	virtual void drawParticles(glm::mat4 projection_matrix, glm::mat4 view_matrix) = 0;
};

#endif /* PARTICLE_EFFECT_H_ */