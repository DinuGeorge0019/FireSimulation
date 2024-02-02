#ifndef SMOKE_PARTICLE_EFFECT_H_
#define SMOKE_PARTICLE_EFFECT_H_

#include "../particle.h"
#include "../../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../ParticleEffect/particle_effect.h"

#include <glm/glm.hpp>

class SmokeParticleEffect : public ParticleEffect
{

public:
	SmokeParticleEffect(GLuint particle_texture, glm::vec3 pos);
	~SmokeParticleEffect();

	void create(GLuint program, Camera* camera = nullptr) override;
	void drawParticles(glm::mat4 projection_matrix, glm::mat4 view_matrix) override;
};


#endif /* SMOKE_PARTICLE_EFFECT_H_ */