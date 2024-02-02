#ifndef TORCH_PARTICLE_EFFECT_H_
#define TORCH_PARTICLE_EFFECT_H_

#include "../particle.h"
#include "../../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../ParticleEffect/particle_effect.h"

#include <glm/glm.hpp>

class TorchParticleEffect : public ParticleEffect
{

public:
	TorchParticleEffect(GLuint particle_texture, glm::vec3 pos);
	~TorchParticleEffect();

	void create(GLuint program, Camera* camera = nullptr) override;
	void drawParticles(glm::mat4 projection_matrix, glm::mat4 view_matrix) override;
};


#endif /* TORCH_PARTICLE_EFFECT_H_ */