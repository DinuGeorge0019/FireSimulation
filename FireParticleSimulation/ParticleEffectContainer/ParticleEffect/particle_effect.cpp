

#include "particle_effect.h"
#include "../particle.h"

#include <algorithm>
#include <glm/gtx/norm.hpp>

ParticleEffect::ParticleEffect(GLuint particle_texture, glm::vec3 pos)
{
	this->last_used_particle = 0;
	this->particle_texture = particle_texture;
	this->position = pos;
}


ParticleEffect::~ParticleEffect()
{

}

void ParticleEffect::SortParticles() {
	std::sort(&this->particles_container[0], &this->particles_container[this->max_particles]);
}


void ParticleEffect::defineUniforms()
{
	glBindVertexArray(this->vertex_array_id);
	glUseProgram(this->program_id);
	this->view_proj_matrix_id = glGetUniformLocation(this->program_id, "VP");
	this->alpha_id = glGetUniformLocation(this->program_id, "alpha");
	this->cull_value_id = glGetUniformLocation(this->program_id, "cull_value");
	// fragment shader
	this->texture_id = glGetUniformLocation(this->program_id, "myTextureSampler");
}


int ParticleEffect::FindUnusedParticle()
{
	for (int i = this->last_used_particle; i < this->max_particles; i++) {
		if (this->particles_container[i].life < 0) {
			this->last_used_particle = i;
			return i;
		}
	}

	for (int i = 0; i < this->last_used_particle; i++) {
		if (this->particles_container[i].life < 0) {
			this->last_used_particle = i;
			return i;
		}
	}
	return 0; // All particles are taken, override the first one
}

