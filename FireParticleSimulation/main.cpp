
#include <lib3ds/file.h>

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <random>

#include "OpenGLWrapper/TextureLoader/texture_loader.h"
#include "OpenGLWrapper/GlfwWrapper/wrapper_glfw.h"
#include "ParticleEffectContainer/FireParticleEffect/fire_particle_effect.h"
#include "ParticleEffectContainer/SmokeParticleEffect/smoke_particle_effect.h"
#include "Camera/camera.h"
#include "Skybox/skybox.h"
#include "Bonfire/bonfire.h"
#include "Terrain/terrain_object.h"
#include "ForestFloor/forest_floor.h"
#include "ForestFloor/instanced_forest.h"

/* Define buffer object indices */
GLuint elementbuffer;

GLuint program, particle_program;		/* Identifier for the shader prgoram */
GLuint vao;								/* Vertex array (Containor) object. This is the index of the VAO that will be the container for our buffer objects */

GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, applyLightingID, instancedID, is_shadowID, flickerID, skyboxID, skyboxViewID, brightnessID, bumpID, bumpTexture;


terrain_object*			terrain;
FireParticleEffect*		fire_particle_effect;
SmokeParticleEffect*	smoke_particle_effect;
SmokeParticleEffect*	sparks_particle_effect;
InstancedForest* 		iforest;
ForestFloor* 			forest_floor;
Bonfire* 				bonfire;
Camera*					camera;
Skybox*					skybox;


GLuint frames;
GLuint bumpVal;
GLuint brightnessVal;


void init(GLWrapper* glw) {

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Load and build the vertex and fragment shaders */
	try
	{
		particle_program = glw->LoadShader("Shaders/particle_object.vert", "Shaders/particle_object.frag", "Shaders/particle_geometry.glsl");
		program = glw->LoadShader("Shaders/vertexshader.vert", "Shaders/fragmentshader.frag");
	}
	catch (std::exception& e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	TextureLoader textureLoader = TextureLoader(program);

	//Load in textures using the stbi library
	GLuint skyboxTexture = textureLoader.Load("Assets/Textures/skybox.png");
	GLuint forestTexture = textureLoader.Load("Assets/Textures/forest_.png");
	GLuint forestBump = textureLoader.LoadBump("Assets/Textures/forest_bump.png");
	GLuint barkTexture = textureLoader.Load("Assets/Textures/pine.png");

	GLuint fireTexture = textureLoader.Load("Assets/Textures/fire_particle.png");
	GLuint smokeTexture = textureLoader.Load("Assets/Textures/smoke_particle.png");
	GLuint sparkTexture = textureLoader.Load("Assets/Textures/fire.png");
	GLuint burnedWoodTexture = textureLoader.Load("Assets/Textures/burned_wood_texture.png");
	GLuint logTexture = textureLoader.Load("Assets/Textures/log_texture.png");
	GLuint torchTexture = textureLoader.Load("Assets/Textures/torch_texture.png");


	//Load the models
	Lib3dsFile* treeModel = lib3ds_file_load("Assets/Lib3dsObjects/Tree1.3ds");
	if (!treeModel)
	{
		throw("Failed to load the tree model");
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
	applyLightingID = glGetUniformLocation(program, "applyLighting");
	instancedID = glGetUniformLocation(program, "instanced");
	is_shadowID = glGetUniformLocation(program, "is_shadow");
	flickerID = glGetUniformLocation(program, "flicker");
	skyboxID = glGetUniformLocation(program, "skybox");
	skyboxViewID = glGetUniformLocation(program, "skyboxView");
	bumpID = glGetUniformLocation(program, "is_bump");
	bumpTexture = glGetUniformLocation(program, "bump_texture");
	brightnessID = glGetUniformLocation(program, "brightness");


	//Skybox setup
	light_x = 0; light_y =4; light_z = -6.5f;
	skybox = new Skybox(skyboxTexture, modelID);
	camera = new Camera(viewID, projectionID, glm::vec3(0, 20, 20), glm::vec3(light_x, light_y + 10, light_z));

	////Terrain setup
	terrain = new terrain_object(modelID, forestTexture, forestBump);
	GLuint xSize = 200;
	GLuint zSize = 200;
	terrain->createTerrain(xSize, zSize, 200, 200);
	terrain->createObject();

	//Forest setup
	iforest = new InstancedForest(modelID, treeModel, barkTexture, terrain->vertices, 200, 200);
	iforest->CreateTreeVBO();
	iforest->Plant();

	////Forest floor
	forest_floor = new ForestFloor(modelID, is_shadowID, barkTexture, terrain->vertices, terrain->normals, xSize, zSize);
	forest_floor->Place();

	//Bonfire setup
	bonfire = new Bonfire(particle_program, modelID, camera, glm::vec3(0, -10, -10), burnedWoodTexture, logTexture, torchTexture, fireTexture);
	bonfire->init();

	//Particle effect setup
	fire_particle_effect = new FireParticleEffect(fireTexture, glm::vec3(0, -9.5, 0));
	fire_particle_effect->create(particle_program, camera);
	smoke_particle_effect = new SmokeParticleEffect(smokeTexture, glm::vec3(0, 5, 0));
	smoke_particle_effect->create(particle_program);
	sparks_particle_effect = new SmokeParticleEffect(sparkTexture, glm::vec3(0, 5, 0));
	sparks_particle_effect->create(particle_program);

	bumpVal = 1;
	brightnessVal = 700;

}


/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	// Define the light position and transform by the view matrix
	glm::vec4 lightpos = camera->GetViewMatrix() * glm::vec4(light_x, light_y + 20, light_z, 1);

	camera->Update();
	bonfire->render();
	glUniform1ui(brightnessID, brightnessVal);
	glUniform1ui(skyboxID, 1);
	glUniformMatrix4fv(skyboxViewID, 1, GL_FALSE, &camera->GetSkyboxView()[0][0]);
	skybox->Render();
	glUniform1ui(skyboxID, 0);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniform1ui(applyLightingID, 1);
	glUniform1ui(instancedID, 1);
	iforest->RenderTrees();
	glUniform1ui(instancedID, 0);
	glUniform1ui(bumpID, bumpVal);
	terrain->drawObject(0);
	glUniform1ui(bumpID, 0);
	forest_floor->Render();
	glUniform1ui(applyLightingID, 0);

	bonfire->render_particles();
	fire_particle_effect->drawParticles(camera->GetProjectionMatrix(), camera->GetViewMatrix());
	smoke_particle_effect->drawParticles(camera->GetProjectionMatrix(), camera->GetViewMatrix());
	sparks_particle_effect->drawParticles(camera->GetProjectionMatrix(), camera->GetViewMatrix());

	glUseProgram(0);
	frames++;
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	camera->SetAspectRatio(((float)w / 640.f * 4.f) / ((float)h / 480.f * 3.f));
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	switch (key) {

	case 'W':
		camera->MoveForward();
		break;
	case 'S':
		camera->MoveBackward();
		break;
	case 'A':
		camera->RotateLeft();
		break;
	case 'D':
		camera->RotateRight();
		break;
	case ' ':
		camera->MoveUp();
		break;
	case 'X':
		camera->MoveDown();
		break;
	case 'M':
		camera->Lookat(camera->GetLookAtPosition() + glm::vec3(0, 1, 0));
		break;
	case 'N':
		camera->Lookat(camera->GetLookAtPosition() + glm::vec3(0, -1, 0));
		break;
	case '0':
		bumpVal = 1;		
		break;
	case '9':
		bumpVal = 0;
		break;
	case '-':
	{
		if (brightnessVal > 500)
		{
			brightnessVal -= 25;//Decrease intensity of light to view less of the scene
		}
		break;
	}
	case '=':
	{
		if (brightnessVal < 2000)
		{
			brightnessVal += 25;//Increase intensity of light to view more of the scene
		}
		break;
	}

	default:
		/*defaul case brake*/
		break;
	
	}

}


/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper* glw = new GLWrapper(1024, 768, "Fire Simulation");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	init(glw);
	glw->eventLoop();
	delete(glw);
	return 0;
}

