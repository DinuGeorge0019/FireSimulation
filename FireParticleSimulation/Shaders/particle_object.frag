#version 420 core

// Interpolated values from the vertex shaders
in vec2 textureCoord;
in vec4 position;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;
uniform mat4 VP;

void main(){
	vec4 pos = inverse(VP) * position;

	color = texture2D(myTextureSampler, textureCoord);

	if (color.w < 0.5)
	{
		discard; //this culling is used to get the particle shape
	}
	
	//color = vec4(1, 1-(pos.y + 12)/10, 0, 1); //colour determined based on distance of particle from terrain. yellow is nearer the terrain and orange/red is further away
}