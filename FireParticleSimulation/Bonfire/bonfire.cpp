
#include "bonfire.h"
#include "../OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <vector>

Bonfire::Bonfire(
	GLuint particle_program, 
	GLuint model_id, 
	Camera* camera,
	glm::vec3 position, 
	GLuint log_texture, 
	GLuint chair_texture,
	GLuint torch_texture,
	GLuint fire_particle_texture)
{
	this->camera = camera;
	this->particle_program = particle_program;
	this->model_id = model_id;
	this->position = position;
	this->log_texture = log_texture;
	this->chair_texture = chair_texture;
	this->torch_texture = torch_texture;
	this->fire_particle_texture = fire_particle_texture;
}

//Place the logs and rotate them in the right direction
void Bonfire::create_bonfire()
{

	int angle_step = 30;
	float inwards_angle = 30;
	float z_rot = 0;
	float x_rot = -30;
	float xrot[12] = { 30, 20, 10, 0, -10, -20, -30, -20, -10, 0, 10, 20};//These rotation values point the cylinders inwards towards a point slightly crookedly
	float zrot[12] = { 0, -10, -20, -30, -20, -10, 0, 10, 20, 30, 20, 10};//to give a more realistic man-made type look
	int count = 0;
	for (int angle = 0; angle < 360; angle += angle_step)
	{
		GLfloat x = cos(angle * DEG2RAD) * 3.5;
		GLfloat z = sin(angle* DEG2RAD) * 3.5;

		logs.push_back(new Cylinder(model_id, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), log_texture, 20));
		
		int index = angle / 30;
		z_rot = xrot[index];
		x_rot = zrot[index];
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(position.x + x, position.y, position.z + z));
		transform = glm::rotate(transform, z_rot, glm::vec3(0, 0, 1));
		transform = glm::rotate(transform, x_rot, glm::vec3(1, 0, 0));
		transform = glm::scale(transform, glm::vec3(1, 3, 1));
		log_transformations.push_back(transform);
		count++;
	}
}

void Bonfire::create_log_chairs() {
	int chair_count = 2; // Number of chairs
	float chair_radius = 30.f; // Distance from the bonfire to the chairs
	float chair_size = 5.f;
	float angle_step = 360.0f / chair_count; // Equally spaced chairs

	for (int i = 0; i < chair_count; ++i) {
		GLfloat x = cos(i * angle_step * DEG2RAD) * chair_radius;
		GLfloat z = sin(i * angle_step * DEG2RAD) * chair_radius - 15.f;
		GLfloat y = (i % 2 == 0) ? -5.f : 4.f; // Alternate height of the chair

		this->chairs.push_back(new Cylinder(model_id, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), this->chair_texture, 20));

		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(position.x + x, position.y + y, position.z + z));
		//rotate the logs to be parallel to the ground
		transform = glm::rotate(transform, 90.0f * DEG2RAD, glm::vec3(1, 0, 0));
		// rotate logs to be lower to the left of the bonfire
		// transform = glm::rotate(transform, glm::radians(-180.0f), glm::vec3(0, 1, 0));

		transform = glm::scale(transform, glm::vec3(chair_size, chair_size, chair_size));
		chair_transformations.push_back(transform);
	}
}

void Bonfire::create_torches() {
	int torch_count = 4; // Number of torches
	float torch_radius = 40.0f; // Distance from the bonfire to the torches
	float angle_step = 360.0f / torch_count; // Equally spaced torches

	for (int i = 0; i < torch_count; ++i) {
		GLfloat x = cos(i * angle_step * DEG2RAD) * torch_radius;
		GLfloat z = sin(i * angle_step * DEG2RAD) * torch_radius;

		// Create a new Cylinder object representing a torch
		this->torches.push_back(new Cylinder(model_id, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), this->torch_texture, 20));
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(position.x + x, position.y, position.z + z));
		transform = glm::scale(transform, glm::vec3(1.0f, 4.5f, 1.0f)); // Scale the torch to make it tall and thin
		torch_transformations.push_back(transform);

		// Create a new FireParticleEffect object representing the fire on top of the torch
		glm::vec3 firePosition = glm::vec3(position.x + x, position.y + 27.0f, position.z + z); // Adjust the y-coordinate as needed
		TorchParticleEffect* fire_particle_effect = new TorchParticleEffect(this->fire_particle_texture, firePosition);
		fire_particle_effect->create(this->particle_program);
		this->torches_fire.push_back(fire_particle_effect);
	}
}

//Renders the whole bonfire
void Bonfire::render_bonfire()
{
	for (int i = 0; i < logs.size(); i++)
	{
		logs[i]->Render(log_transformations[i]);
	}
}

//Renders the bonfire chairs
void Bonfire::render_log_chairs() {
	for (int i = 0; i < chairs.size(); i++)
	{
		chairs[i]->Render(chair_transformations[i]);
	}
}

//Render the torches
void Bonfire::render_torches()
{
	for (int i = 0; i < torches.size(); i++)
	{
		torches[i]->Render(torch_transformations[i]);
	}
}

//Render the torches fire
void Bonfire::render_torches_fire_particles()
{
	for (int i = 0; i < torches_fire.size(); i++)
	{
		torches_fire[i]->drawParticles(this->camera->GetProjectionMatrix(), this->camera->GetViewMatrix());
	}
}

void Bonfire::init()
{
	create_bonfire();
	create_log_chairs();
	create_torches();
}

void Bonfire::render()
{
	render_bonfire();
	render_log_chairs();
	render_torches();
}

void Bonfire::render_particles()
{
	render_torches_fire_particles();
}
