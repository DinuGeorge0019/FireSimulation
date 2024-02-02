#ifndef FIRE_PARTICLE_EFFECT_H_
#define FIRE_PARTICLE_EFFECT_H_


#include "../../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "../particle.h"
#include "../../Camera/camera.h"
#include "../ParticleEffect/particle_effect.h"

#include <glm/glm.hpp>

class FireParticleEffect : public ParticleEffect
{

public:
	FireParticleEffect(GLuint particle_texture, glm::vec3 pos);
	~FireParticleEffect();

	void create(GLuint program, Camera* camera = nullptr) override;
	void drawParticles(glm::mat4 projection_matrix, glm::mat4 view_matrix) override;
};

#endif /* FIRE_PARTICLE_EFFECT_H_ */