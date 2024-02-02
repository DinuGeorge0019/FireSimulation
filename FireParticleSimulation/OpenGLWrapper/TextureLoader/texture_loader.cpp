
#include "texture_loader.h"
#include "stb_image.h"

#include <algorithm>

TextureLoader::TextureLoader(GLuint prog)
{
	program = prog;
}

//Loads a texture and returns the texture ID
GLuint TextureLoader::Load(const char* path)
{
	GLuint particle_texture_id;

	// load image
	int width, height, num_components;
	unsigned char* image_data = stbi_load(path, &width, &height, &num_components, 4);

	// Get the texture id
	glGenTextures(1, &particle_texture_id);

	// now generate texture

	// create Texture
	glBindTexture(GL_TEXTURE_2D, particle_texture_id);

	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	// and finally free image data
	stbi_image_free(image_data);

	return particle_texture_id;
}


//Used to load a secondary bump texture
GLuint TextureLoader::LoadBump(const char* path)
{
    GLuint texID = 0;
    int width, height, num_components;
    unsigned char* image_data = stbi_load(path, &width, &height, &num_components, STBI_rgb_alpha);
    if (image_data != NULL)
    {
        // Generate texture ID
        glGenTextures(1, &texID);

        // Bind the texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texID);

        // Set texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free the image data
        stbi_image_free(image_data);

        // Set the texture uniform in the shader
        int loc = glGetUniformLocation(program, "bump_texture");
        if (loc >= 0) glUniform1i(loc, 1);
    }
    else
    {
        printf("Texture loading failed: %s\n", stbi_failure_reason());
    }
    return texID;
}