
#include "../ParticleEffect/particle_effect.h"
#include "smoke_particle_effect.h"
#include "../particle.h"

#include <algorithm>
#include <glm/gtx/norm.hpp>


SmokeParticleEffect::SmokeParticleEffect(GLuint particle_texture, glm::vec3 pos)
	: ParticleEffect(particle_texture, pos)
{

}

SmokeParticleEffect::~SmokeParticleEffect()
{

}

void SmokeParticleEffect::create(GLuint program, Camera* camera)
{
	glGenVertexArrays(1, &this->vertex_array_id);
	glBindVertexArray(this->vertex_array_id);
	this->program_id = program;
	glUseProgram(this->program_id);

	this->particle_positions = new GLfloat[this->max_particles * 4];

	for (int i = 0; i<this->max_particles; i++){
		this->particles_container[i].life = -1.0f;
		this->particles_container[i].cameradistance = -1.0f;
	}

	glGenBuffers(1, &this->particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, this->max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	/* Define the uniform variables */
	this->defineUniforms();

	this->lastTime = glfwGetTime();
}


/* Update the particle animation and draw the particles */
void SmokeParticleEffect::drawParticles(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
	double currentTime = glfwGetTime();
	double delta = currentTime - this->lastTime;
	this->lastTime = currentTime;

	glBindVertexArray(this->vertex_array_id);

	glUniform1f(this->cull_value_id, 0.5);

	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	glm::vec3 CameraPosition(glm::inverse(view_matrix)[3]);
	glm::mat4 ViewProjectionMatrix = projection_matrix * view_matrix;

	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.005f*10000.0);

	for (int i = 0; i<newparticles; i++){
		int particleIndex = this->FindUnusedParticle();
		this->particles_container[particleIndex].life = 5; // This particle will live for 1 second.
		this->particles_container[particleIndex].pos = glm::vec3(0, 0, -10.0f);

		float spread = 3;
		glm::vec3 maindir = glm::vec3(0.0f, -10.0f, 0.0f);
		// This part creates the wavy effect the fire has
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
			);
		this->particles_container[particleIndex].pos += glm::vec3(randomdir.x + position.x, position.y, randomdir.z + position.z);
		this->particles_container[particleIndex].speed = glm::vec3(position.x, 3, position.z);
	}

	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i<this->max_particles; i++){

		Particle& p = this->particles_container[i]; // shortcut

		if (p.life > 0.0f){

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f){

				// Simulate simple physics : gravity only, no collisions
				p.pos += p.speed * ((float)delta);
				p.cameradistance = glm::length2(p.pos - CameraPosition);

				// Fill the GPU buffer
				this->particle_positions[4 * ParticlesCount + 0] = p.pos.x;
				this->particle_positions[4 * ParticlesCount + 1] = p.pos.y;
				this->particle_positions[4 * ParticlesCount + 2] = p.pos.z;

			}
			else{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;
		}
	}

	this->SortParticles();

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

	glBindBuffer(GL_ARRAY_BUFFER, this->particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, this->max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, this->particle_positions);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	glUseProgram(this->program_id);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->particle_texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(this->texture_id, 0);

	glUniformMatrix4fv(this->view_proj_matrix_id, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->particles_position_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_POINTS, 0, ParticlesCount);

	glDisable(GL_BLEND);
}
